
#include "Regex.hpp"  //include before algo causes errors with #define epsilon char(0) maybe collision?

#include <stack>
#include <map> 
#include <sstream>
#include <cassert> //todo try to replace with excpetion everywhere
#include <algorithm>

namespace regex
{

struct BlackBox
{
    BlackBox(StateId entry, StateId exit)
    : entry {entry}
    , exit {exit}
    {}

    StateId entry;
    StateId exit;
};

BlackBox buildSymbol(NFA& nfa, char c)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(c, entry, exit);
    return BlackBox(entry, exit);
}

BlackBox buildUnion(NFA& nfa, BlackBox& BB1, BlackBox& BB2)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(epsilon, entry, BB1.entry);
    nfa.addTransition(epsilon, entry, BB2.entry);
    nfa.addTransition(epsilon, BB1.exit, exit);
    nfa.addTransition(epsilon, BB2.exit, exit);
    return BlackBox(entry, exit);
}

BlackBox buildConcatenation(NFA& nfa, BlackBox& BB1, BlackBox& BB2)
{
    auto entry = BB1.entry;
    auto exit = BB2.exit;
    nfa.addTransition(epsilon, BB1.exit, BB2.entry);
    return BlackBox(entry, exit);
}

BlackBox buildKleeneStar(NFA& nfa, BlackBox& BB)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(epsilon, entry, BB.entry);
    nfa.addTransition(epsilon, entry, exit);
    nfa.addTransition(epsilon, BB.exit, BB.entry);
    nfa.addTransition(epsilon, BB.exit, exit);
    return BlackBox(entry, exit);
}

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
void regexToNFA(NFA& nfa, std::string regex)
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
                stack.push(buildUnion(nfa, op2, op1));
                break;
            }
            case '&':
            {   
                auto op1 = stack.top();
                stack.pop();
                auto op2 = stack.top();
                stack.pop();
                stack.push(buildConcatenation(nfa, op2, op1));
                break;
            }
            case '*':
            {
                auto op1 = stack.top();
                stack.pop();
                stack.push(buildKleeneStar(nfa, op1));
                break;
            }

            default:
            {
                stack.push(buildSymbol(nfa, c));
            }
        }
    }
    assert(stack.size()==1);

    // stack now contains the black box containing within it
    // the FSA equivalent of the regex. Only need to connect
    // the entry and exit states of the black box to the start
    // and end states.
    auto bb = stack.top();
    auto start = nfa.addState(true, false);
    auto end = nfa.addState(false, true);
    nfa.addTransition(epsilon, start, bb.entry);
    nfa.addTransition(epsilon, bb.exit, end);

    // TODO: need to add epsilon self-transition for each node?
    // or can be assumed inplicitly to optimize?

}

// todo: add this to unit test?
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


/*
regex.compile()
{
    //validate
    //proprocess
    //toPostfix
    //createEpsilonNFA
    //createNFA
    //createDFA
    //minimizeDFA
};
*/


} //namespace regex