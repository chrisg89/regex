#include "NFA.hpp"

#include <cassert>
#include <stack>
#include <algorithm>
#include <unordered_set>

namespace nfa
{

Transition::Transition(char input, StateId destination)
    : input{input}
    , destination{destination}
{}

State::State(StateId id, bool isStart, bool isFinal)
    : mId{id}
    , mIsStart{isStart}
    , mIsFinal{isFinal}
    , mTransitions{}
{}

void State::addTransition(char input, StateId destination)
{
    mTransitions.emplace_back(input, destination);
}

StateId State::next(char input)
{
    for (auto transition : mTransitions)
    {
        if(transition.input == input)
        {
            return transition.destination;
        }
    }

    return kNullState;
}

NFA::NFA(Alphabet alphabet)
    : mStates{}
    , mStateCount{0}
    , mStartState{kNullState}
    , mFinalStates{}
    , mAlphabet{alphabet}
{}

StateId NFA::addState(bool isStart, bool isFinal)
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

void NFA::addTransition(char input, StateId source, StateId destination)
{
    mStates[source].addTransition(input, destination);
}

std::string NFA::toPlantUML()
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

        for (auto transition : state.mTransitions)
        {
            plantUML += std::to_string(state.mId);
            plantUML += " -> ";
            plantUML += std::to_string(transition.destination);
            plantUML += " : ";
            plantUML += (transition.input == kEpsilon ? std::string("null") : std::string(1, transition.input)); //todo clean up
            plantUML += "\n";
        }
    }
    plantUML += "@enduml\n";

    return plantUML;
}


DFA NFA::toDFA()
{
    DFA dfa;

    // TODO: explain
    EpsilonNFAToNFAConversion();

    // TODO: explain
    NFAToDFAConversion();

    return dfa;

}

void NFA::EpsilonNFAToNFAConversion()
{
    NFA newNFA(mAlphabet);

    for (auto state : mStates)
    {
        newNFA.addState(state.mIsStart, state.mIsFinal);
    }

    auto map = CreateEpsilonClosureMap();

    // STEP1: Calculate the new state transitions replacing 
    // epsilon transitions and insert them into the new NFA
    std::unordered_set<StateId> reachableByEpsilonClosureSet;
    for(auto character : mAlphabet)
    {
        for (auto state : mStates)
        {
            for (auto reachableByEpsilonClosure1 : map[state.mId])
            {
                auto target = mStates[reachableByEpsilonClosure1].next(character);

                if(target != kNullState)
                {
                    for (auto reachableByEpsilonClosure2 : map[target])
                    {
                        reachableByEpsilonClosureSet.insert(reachableByEpsilonClosure2);
                    }
                }
            }

            // insert into new nfa
            for (auto reachableByEpsilon : reachableByEpsilonClosureSet)
            {
                newNFA.addTransition(character, state.mId, reachableByEpsilon);
            }
            reachableByEpsilonClosureSet.clear();
        }
    }

    // STEP2: Calculate new final states after removal of epsilon  
    // transitions and update states in the new NFA
    for (auto state : mStates)
    {
        if (isReachableByEpsilonClosure(state.mId, mFinalStates[0])) // TODO: optimize this... its very wasteful to recalc 
        {
            newNFA.mStates[state.mId].mIsFinal = true;
            newNFA.mFinalStates.push_back(state.mId);
        }
    }

    //STEP3: replace old NFA with new NFA
    *this = newNFA;  // TODO: should use move semantics here?
}

EpsilonClusureMap NFA::CreateEpsilonClosureMap()
{
    EpsilonClusureMap map;

    for (auto source : mStates)
    {
        for (auto destination : mStates)
        {
            if(isReachableByEpsilonClosure(source.mId, destination.mId))
            {
                map[source.mId].push_back(destination.mId);
            }
        }
    }

    return map;
}

//TODO: there are alot of optimizations I can make here. fill in closure map as I go
bool NFA::isReachableByEpsilonClosure(StateId source, StateId destination)
{
    std::stack<StateId> stack;
    std::vector<StateId> visited;

    if (source == destination)
    {
        return true;
    }

    stack.push(source);
    visited.push_back(source);

    while(!stack.empty())
    {
        auto state = stack.top();
        stack.pop();

        for (auto transition : mStates[state].mTransitions)
        {
            if(transition.input == kEpsilon)
            {
                auto adjacent = transition.destination;

                if(adjacent == destination)
                {
                    return true;
                }

                if (std::find(visited.begin(), visited.end(), adjacent) == visited.end())
                {
                    stack.push(adjacent);
                    visited.push_back(adjacent);
                }
            }
        }
    }

    return false;
}


void NFA::NFAToDFAConversion()
{

}

} //namespace nfa