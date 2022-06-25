#include "NFA.hpp"
#include "Bimap.hpp"

#include <cassert>
#include <stack>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <deque>

namespace nfa
{

struct VectorHasher {
    int operator()(const std::vector<StateId> &V) const {
        int hash = V.size();
        for(auto &i : V) {
            hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};




using StateMapper = Bimap<std::vector<StateId>, StateId, VectorHasher>;  //TODO change to set<StateId>










State::State(StateId id, bool isStart, bool isFinal)
    : mId{id}
    , mIsStart{isStart}
    , mIsFinal{isFinal}
    , mTransitions{}
{}

void State::addTransition(char input, StateId destination)
{
    mTransitions[input].emplace_back(destination);
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

        for (auto const& [input, destinations] : state.mTransitions)
        {
            for (auto destination: destinations)
            {
                plantUML += std::to_string(state.mId);
                plantUML += " -> ";
                plantUML += std::to_string(destination);
                plantUML += " : ";
                plantUML += (input == kEpsilon ? std::string("null") : std::string(1, input)); //todo clean up
                plantUML += "\n";
            }
        }
    }
    plantUML += "@enduml\n";


    return plantUML;
}


DFA NFA::toDFA()
{
    // TODO: explain
    EpsilonNFAToNFAConversion();

    // TODO: explain
    auto dfa = NFAToDFAConversion();

    return dfa;

}

void NFA::EpsilonNFAToNFAConversion()
{
    NFA newNFA(mAlphabet);

    // This algorithm assumes only one final state
    assert(mFinalStates.size() == 1);

    // STEP1: Calculate the new states and insert
    // them into the new NFA
    for (auto state : mStates)
    {
        //TODO: can optimize this further?
        newNFA.addState(state.mIsStart, isReachableByEpsilonClosure(state.mId, mFinalStates[0]));
    }

    auto map = CreateEpsilonClosureMap();

    // STEP2: Calculate the new transitions and insert
    // them into the new NFA
    std::unordered_set<StateId> reachableByEpsilonClosureSet;
    for(auto character : mAlphabet)
    {
        for (auto state : mStates)
        {
            for (auto reachableByEpsilonClosure1 : map[state.mId])
            {
                for(auto destination : mStates[reachableByEpsilonClosure1].mTransitions[character])
                {
                    for (auto reachableByEpsilonClosure2 : map[destination])
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

        for (auto transition : mStates[state].mTransitions[kEpsilon])
        {
            auto adjacent = transition; //TODO rename transition to something better

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

    return false;
}

bool NFA::ContainsFinalState(const std::vector<StateId>& composite )
{
    for(auto state1 : composite)
    {
        for(auto state2 : mFinalStates)
        {
            if (state1 == state2)
            {
                return true;
            }
        }
    }
    return false;
}

DFA NFA::NFAToDFAConversion()
{

    DFA dfa(mAlphabet);
    std::deque<StateId> queue;
    StateMapper mapper;
    std::set<StateId> set;


    auto dfaState = dfa.addState(true, mStates[mStartState].mIsFinal);
    mapper.insert(dfaState, {mStartState});
    queue.push_back(dfaState);


    while(!queue.empty())
    {
        dfaState = queue.front();
        queue.pop_front();

        auto nfaStates = mapper.get(dfaState);

        for(auto character : mAlphabet)
        {

            set.clear();

            for(auto nfaState : nfaStates)
            {
                // calc  union of all dest states
                auto destinations = mStates[nfaState].mTransitions[character];
                std::copy(destinations.begin(), destinations.end(), std::inserter(set, set.end()));
            }

            std::vector<StateId> composite(set.begin(), set.end());  // call this union?
            auto newDfaState = StateId{kNullState};
            if(mapper.contains(composite))
            {
                newDfaState = mapper.get(composite);
            }
            else
            {
                newDfaState = dfa.addState(false, ContainsFinalState(composite));  //TODO calculate if final state
                mapper.insert(newDfaState, composite);
                queue.push_back(newDfaState);
            }

            dfa.addTransition(character, dfaState, newDfaState);
        }
    }

    return dfa;
}




} //namespace nfa

