#include "DFA.hpp"

#include <cassert>

namespace dfa
{

// TODO create generic state and then specialize it for NFA and DFA.
State::State(StateId id, bool isStart, bool isFinal)
    : mId{id}
    , mIsStart{isStart}
    , mIsFinal{isFinal}
    , mTransitions{}
{}

void State::addTransition(char input, StateId destination)
{
    mTransitions[input] = destination;
}

DFA::DFA(Alphabet alphabet)
    : mStates{}
    , mStateCount{0}
    , mStartState{kNullState}
    , mFinalStates{}
    , mAlphabet{alphabet}
{}

StateId DFA::addState(bool isStart, bool isFinal)
{
    if(isStart)
    {
        assert(mStartState == kNullState); //TODO replace with exception
        mStartState = mStateCount;
    }

    if(isFinal)
    {
        mFinalStates.emplace_back(mStateCount);
    }

    mStates.emplace_back(mStateCount, isStart, isFinal);

    return mStateCount++;
}

void DFA::addTransition(char input, StateId source, StateId destination)
{
    mStates[source].addTransition(input, destination);
}




} //namespace dfa