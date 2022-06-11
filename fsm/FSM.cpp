
#include "iostream"
#include <cassert>
#include <algorithm>
#include "FSM.hpp"  //include before algo causes errors with #define epsilon char(0) maybe collision?

Transition::Transition(State& target, char input):
    mTarget{target},
    mInput{input}
{}

State::State(bool isFinal):
    mTransitions{},
    mId{++count},
    mIsFinal{isFinal}
{
    //std::cout << "hello from state (id=" << mId << ")" << std::endl;
}

int State::count=0;

void State::addTransitionTo(State& to, char input)
{
    mTransitions.emplace_back(to, input);
}

void State::addTransitionFrom(State& from, char input)
{
    from.addTransitionTo(*this, input);
}

int State::numTransitions()
{
    return mTransitions.size();
}

bool State::operator==(const State& other) {
    return (this->mId == other.mId);
}

bool State::operator!=(const State& other) {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const State& state)
{
    os << std::endl;
    os << "State " << state.mId << std::endl;
    os << "Transitions:" << state.mTransitions.size() << std::endl;

    for (auto t : state.mTransitions)
    {
        os << t.input() << "->" << t.targetState().mId << std::endl;
    }

    return os;
}

FSM::FSM():
    mStates{},
    mStartState{nullptr}
{
    std::cout << "hello from HSM!" << std::endl;
}

State& FSM::addState(bool isStart, bool isFinal )
{
    mStates.emplace_back(isFinal);

    if(isStart)
    {
        assert(mStartState == nullptr); //TODO: exception instead?
        mStartState = &mStates.back();
        
    }

    return mStates.back();
}


std::string FSM::toPlantUML()
{
    std::string plantUML = "";
    plantUML += "@startuml\n";
    plantUML += "hide empty description\n";

    plantUML += " [*] --> ";
    plantUML += std::to_string(mStartState->mId);
    plantUML += "\n";
    
    for (auto state = mStates.begin(); state != mStates.end(); ++state)
    {
        if(state->isFinal())
        {
            plantUML += std::to_string(state->mId);
            plantUML += " : Final\n";
        }

        for (auto transition = state->mTransitions.begin(); transition != state->mTransitions.end(); ++transition)
        {
            plantUML += std::to_string(state->mId);
            plantUML += " -> ";
            plantUML += std::to_string(transition->targetState().mId);
            plantUML += " : ";
            plantUML += (transition->input() == epsilon ? std::string("null") : std::string(1, transition->input())); //todo clean up
            plantUML += "\n";
        }
    }
    plantUML += "@enduml\n";

    return plantUML;
}

#include "stack"


bool FSM::isReachableIf(State& start, State& end, std::function<bool(char)> predicate)
{
    //DFS approach //TODO check performance against BFS later?

    std::stack<State*> stack;
    std::vector<int> visited;

    stack.push(&start);
    visited.push_back(start.mId);

    while(!stack.empty())
    {
        auto state = stack.top();
        stack.pop();

        for (auto t : state->mTransitions)
        {
            //todo put targte state into local to clean this up
            if(t.targetState() == end)
            {
                return true;
            }

            if(!predicate(t.input()))
            {
                continue;
            }

            if (std::find(visited.begin(), visited.end(), t.targetState().mId) == visited.end())
            {
                stack.push(&(t.targetState()));
                visited.push_back(t.targetState().mId);
            }
        }
    }

    return false;
}

State& FSM::startState()
{
    assert(mStartState != nullptr);
    return *mStartState;
}

bool FSM::run(std::string string)
{
    State* next;
    State* current;

    assert(mStartState != nullptr); //TODO: exception instead?

    current = mStartState;
    for(char& c : string) 
    {
        next = current->run(c);
        if(next!=nullptr)
        {
            current = next;
        }
        else
        {
            return false;
        }
    }

    if (current->isFinal())
    {
        return true;
    }
    else
    {
        return false;
    }

}

State* State::run(char c)
{
    for (auto transition : mTransitions)
    {
        if(c == transition.input())
        {
            State* next = &transition.targetState();
            std::cout << "State" << this->mId << " -> " 
                      << "State" << next->mId << " due to "
                      << "<" << c << ">" << std::endl;
            return next;
        }
    }

    return nullptr;

}

bool State::isFinal()
{
    return mIsFinal;
}

State& Transition::targetState()
{
    return mTarget;
}

char Transition::input()
{
    return mInput;
}







struct BlackBox
{
    BlackBox(State& entry, State& exit)
    : entry {entry}
    , exit {exit}
    {}

    State& entry;
    State& exit;
};

BlackBox buildSymbol(FSM& fsm, char c)
{
    auto& entry = fsm.addState(false, false);
    auto& exit = fsm.addState(false, false);
    entry.addTransitionTo(exit, c);
    return BlackBox(entry, exit);
}

BlackBox buildUnion(FSM& fsm, BlackBox& BB1, BlackBox& BB2)
{
    auto& entry = fsm.addState(false, false);
    auto& exit = fsm.addState(false, false);
    entry.addTransitionTo(BB1.entry, epsilon);
    entry.addTransitionTo(BB2.entry, epsilon);
    BB1.exit.addTransitionTo(exit, epsilon);
    BB2.exit.addTransitionTo(exit, epsilon);
    return BlackBox(entry, exit);
}

BlackBox buildConcatenation(FSM& fsm, BlackBox& BB1, BlackBox& BB2)
{
    auto& entry = BB1.entry;
    auto& exit = BB2.exit;
    BB1.exit.addTransitionTo(BB2.entry, epsilon);
    return BlackBox(entry, exit);
}

BlackBox buildKleeneStar(FSM& fsm, BlackBox& BB)
{
    auto& entry = fsm.addState(false, false);
    auto& exit = fsm.addState(false, false);
    entry.addTransitionTo(BB.entry, epsilon);
    entry.addTransitionTo(exit, epsilon);
    BB.exit.addTransitionTo(BB.entry, epsilon);
    BB.exit.addTransitionTo(exit, epsilon);
    return BlackBox(entry, exit);
}

#include <stack> 
#include <sstream>

int priority(char c)
{
    int prio;
    if(c == '*')
    {
        prio=3;
    }
    else if (c == '&' )
    {
        prio=2;
    }
    else if (c == '|')
    {
        prio=1;
    }
    else
    {
        assert(false);
    }

    return prio;
}

std::string PreprocessRegex(std::string regex)
{
    std::stringstream ss;

    ss << regex;
    std::string processed = "";
    bool insert;

    while( true )
    {
        char current = ss.get();
        char next = ss.peek();
        insert = false;

        if(next == EOF)
        {
            processed += current;
            break;
        }

        if(current == '*')
        {
            if(next == '*'){
                insert = false;
            }
            else if(next == '|'){
                insert = false;
            }
            else if(next == '('){
                insert = true;
            }
            else if(next == ')'){
                insert = false;
            }
            else{
                insert = true;
            }
        }
        else if (current== '|')
        {
            insert = false;
        }
        else if (current == '(')
        {
            insert = false;
        }
        else if (current == ')')
        {
            if(next == '*'){
                insert = false;
            }
            else if(next == '|'){
                insert = false;
            }
            else if(next == '('){
                insert = true;
            }
            else if(next == ')'){
                insert = false;
            }
            else{
                insert = true;
            }
        }
        else
        {
            if(next == '*'){
                insert = false;
            }
            else if(next == '|'){
                insert = false;
            }
            else if(next == '('){
                insert = true;
            }
            else if(next == ')'){
                insert = false;
            }
            else{
                insert = true;
            }
        }

        processed += current;
        if(insert)
        {
            processed += '&';
        }

    }

    return processed;
}

std::string RegexInfixToPostfix(std::string infix)
{
    std::string postfix = "";
    std::stack<char> stack;

    for(char& c : infix) 
    {
        switch (c)  
        {
            case '(':
            {
                stack.push(c);
                break;
            }

            case ')':
            {
                while(stack.top() != '(')
                {
                    postfix += stack.top();
                    stack.pop();
                }
                stack.pop();
                break;
            }

            case '|':
                [[fallthrough]];
            case '*':
                [[fallthrough]];
            case '&':
            {
                while(!stack.empty())
                {
                    if (stack.top() == '(')
                        break;
                    if (priority(stack.top()) < priority(c))
                        break;

                    postfix += stack.top();
                    stack.pop();
                }
                stack.push(c);
                break;
            }

            default:
            {
                postfix += c;
            }

        }
    }
    while(!stack.empty())
    {
        postfix += stack.top();
        stack.pop();
    }

    return postfix;
}

std::string RegexPostfixToInfix(std::string postfix)
{
    std::string infix;
    std::stack<std::string> stack;
    std::string temp;
    std::string op1;
    std::string op2;

    for(char& c : postfix) 
    {
        switch (c)  
        {
            case '|':
                [[fallthrough]];
            case '&':
            {   
                op1 = stack.top();
                stack.pop();
                op2 = stack.top();
                stack.pop();
                stack.push("(" + op2 + c + op1 + ")");
                break;
            }

            case '*':
            {
                op1 = stack.top();
                stack.pop();
                stack.push("(" + op1+c + ")");
                break;
            }

            default:
            {
                temp = c;
                stack.push(temp);
            }
        }
    }
    assert(stack.size()==1);
    infix = stack.top();
    return infix;
}

bool isValidRegex(std::string regex)
{
    std::stringstream ss(regex);
    bool valid = true;
    int leftBracketCount = 0;
    int rightBracketCount = 0;

    char current;
    char next;

    while( true )
    {
        current = ss.get();
        next = ss.peek();

        if(current == EOF || !valid)
        {
            if (rightBracketCount != leftBracketCount)
            {
                valid = false;
            }
            break;
        }

        if (current == '(')
        {
            leftBracketCount++;
            if(next == '('){
                valid = true;
            }
            else if(next == ')'){
                valid = false;
            }
            else if(next == '*'){
                valid = false;
            }
            else if(next == '|'){
                valid = false;
            }
            else if(next == EOF){
                valid = false;
            }
            else{
                valid = true;
            }
        }
        else if (current == ')')
        {
            rightBracketCount++;
            if(next == '('){
                valid = true;
            }
            else if(next == ')'){
                valid = true;
            }
            else if(next == '*'){
                valid = true;
            }
            else if(next == '|'){
                valid = true;
            }
            else if(next == EOF){
                valid = true;
            }
            else{
                valid = true;
            }
        }
        else if (current == '*')
        {
            if(next == '('){
                valid = true;
            }
            else if(next == ')'){
                valid = true;
            }
            else if(next == '*'){
                valid = true;
            }
            else if(next == '|'){
                valid = true;
            }
            else if(next == EOF){
                valid = true;
            }
            else{
                valid = true;
            }
        }
        else if (current == '|')
        {
            if(next == '('){
                valid = true;
            }
            else if(next == ')'){
                valid = false;
            }
            else if(next == '*'){
                valid = false;
            }
            else if(next == '|'){
                valid = false;
            }
            else if(next == EOF){
                valid = false;
            }
            else{
                valid = true;
            }
        }
        else
        {
            if(next == '('){
                valid = true;
            }
            else if(next == ')'){
                valid = true;
            }
            else if(next == '*'){
                valid = true;
            }
            else if(next == '|'){
                valid = true;
            }
            else if(next == EOF){
                valid = true;
            }
            else{
                valid = true;
            }
        }

        if (rightBracketCount > leftBracketCount)
        {
            valid = false;
        }
    }

    return valid;
}



// maybe change to return FSM by value? Would this break the 
// internal references? I think it would be a move...
void regexToNFA(FSM& fsm, std::string regex)
{
    if (!isValidRegex(regex))
        assert(false);
    
    regex = PreprocessRegex(regex);
    regex = RegexInfixToPostfix(regex);

    std::stack<BlackBox> stack;

    for(char& c : regex) 
    {
        switch (c)  
        {
            case '|':
            {
                auto op1 = stack.top();
                stack.pop();
                auto op2 = stack.top();
                stack.pop();
                stack.push(buildUnion(fsm, op2, op1));
                break;
            }
            case '&':
            {   
                auto op1 = stack.top();
                stack.pop();
                auto op2 = stack.top();
                stack.pop();
                stack.push(buildConcatenation(fsm, op2, op1));
                break;
            }
            case '*':
            {
                auto op1 = stack.top();
                stack.pop();
                stack.push(buildKleeneStar(fsm, op1));
                break;
            }

            default:
            {
                stack.push(buildSymbol(fsm, c));
            }
        }
    }
    assert(stack.size()==1);

    // stack now contains the black box containing within it
    // the FSA equivalent of the regex. Only need to connect
    // the entry and exit states of the black box to the start
    // and end states.
    auto bb = stack.top();
    auto& start = fsm.addState(true, false);
    auto& end = fsm.addState(false, true);
    start.addTransitionTo(bb.entry, epsilon);
    bb.exit.addTransitionTo(end, epsilon);

    // TODO: need to add epsilon self-transition for each node?
    // or can be assumed inplicitly to optimize?

}





/*
    //a(a|b|c)*a
    auto n1  = buildSymbol(fsm, 'a');
    auto n2  = buildSymbol(fsm, 'b');
    auto n3  = buildUnion(fsm, n1, n2);
    auto n4  = buildSymbol(fsm, 'c');
    auto n5  = buildUnion(fsm, n3, n4);
    auto n6  = buildKleeneStar(fsm, n5);
    auto n7  = buildSymbol(fsm, 'a');
    auto n8  = buildConcatenation(fsm, n7, n6);
    auto n9  = buildSymbol(fsm, 'a');
    auto n10 = buildConcatenation(fsm, n8, n9);
    
    auto& start = fsm.addState(true, false);
    auto& final = fsm.addState(false, true);

    start.addTransitionTo(n10.entry, epsilon);
    n10.exit.addTransitionTo(final, epsilon);

    std::cout << n8.exit << std::endl;

    return fsm;

*/