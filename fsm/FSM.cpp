#include "FSM.hpp"
#include <iostream>
#include <cassert> // todo try to replace with exceptions
#include <stack>

namespace fsm{

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
    //TODO: plantuml does an awful job of positioning the
    // states and its really hard to see useful details.
    // maybe remove this?
    std::string plantUML = "";
    #if 0 
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

    #endif
    return plantUML;
}




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
    // todo: change implementation to search for start state
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

} //namespace fsm