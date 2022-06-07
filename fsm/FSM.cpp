#include "FSM.hpp"
#include "iostream"
#include <cassert>


Transition::Transition(State& target, char input):
    mTarget{target},
    mInput{input}
{}

State::State(bool isFinal):
    mTransitions{},
    mId{++count},
    mIsFinal{isFinal}
{
    std::cout << "hello from state (id=" << mId << ")" << std::endl;
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
    mCurrentState{nullptr},
    mStartState{nullptr}
{
    std::cout << "hello from HSM!" << std::endl;
}

State& FSM::addState(bool isStart, bool isFinal )
{
    mStates.emplace_back(isFinal);

    if(isStart)
    {
        assert(mStartState == nullptr);
        mStartState = &mStates.back();
        mCurrentState = &mStates.back();
        
    }

    return mStates.back();
}

State& FSM::startState()
{
    assert(mStartState != nullptr);
    return *mStartState;
}

State& FSM::currentState()
{
    assert(mCurrentState != nullptr);
    return *mCurrentState;
}

bool FSM::run(std::string string)
{
    State* nextState;

    mCurrentState = mStartState;
    
    assert(mCurrentState == mStartState);

    for(char& c : string) 
    {
        std::cout << c << std::endl;
        nextState = mCurrentState->run(c);
    
        if(nextState!=nullptr)
        {
            mCurrentState = nextState;
        }
        else
        {
            return false;
        }
    }

    if (mCurrentState->isFinal())
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
    std::cout << "@@@@@" << std::endl;
    std::cout << mTransitions.size() << std::endl;
    for (auto transition : mTransitions)
    {
        std::cout << "----" << std::endl;
        //if(c == transition.input())
        //{
        //    return &transition.targetState();
        //}
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