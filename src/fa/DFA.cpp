#include "DFA.hpp"

#include <cassert>


namespace fa
{

// TODO create generic state and then specialize it for NFA and DFA.
DFAState::DFAState(StateId id, bool isStart, bool isFinal)
    : mId{id}
    , mIsStart{isStart}
    , mIsFinal{isFinal}
    , mTransitions{}
{}

void DFAState::addTransition(char input, StateId destination)
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

std::string DFA::toPlantUML()
{

    //TODO: plantuml does an awful job of positioning the
    // states and its really hard to see useful details.
    // maybe remove this?
    std::string plantUML = "";

    plantUML += "@startuml\n";
    plantUML += "hide empty description\n";

    plantUML += "[*] --> ";
    plantUML += std::to_string(mStartState);
    plantUML += "\n";
    
    for (auto state : mStates)
    {
        if(state.mIsFinal)
        {
            plantUML += std::to_string(state.mId);
            plantUML += " : Final\n";
        }

        for (auto const& [input, destination] : state.mTransitions)
        {
            plantUML += std::to_string(state.mId);
            plantUML += " -> ";
            plantUML += std::to_string(destination);
            plantUML += " : ";
            plantUML += input;
            plantUML += "\n";
        }
    }
    plantUML += "@enduml\n";


    return plantUML;
}


} //namespace fa