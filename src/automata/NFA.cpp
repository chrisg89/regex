#include "NFA.hpp"
#include "Bimap.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <set>
#include <stack>
#include <unordered_set>

namespace automata
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

NFAState::NFAState(StateId id, bool isStart, bool isFinal)
    : mId{id}
    , mIsStart{isStart}
    , mIsFinal{isFinal}
    , mTransitions{}
{}

void NFAState::addTransition(InputType input, StateId destination)
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
        assert(mStartState == kNullState);
        mStartState = mStateCount;
    }

    if(isFinal)
    {
        mFinalStates.emplace_back(mStateCount);
    }

    mStates.emplace_back(mStateCount, isStart, isFinal);

    return mStateCount++;
}

void NFA::addTransition(InputType input, StateId source, StateId destination)
{
    mStates.at(source).addTransition(input, destination);
}

std::string NFA::serialize()
{
    std::string out = "";
    
    for (auto state : mStates)
    {
        out += std::to_string(state.mId);
        out += " : Start = ";
        out += (state.mIsStart? "true" : "false");
        out += " | Final = ";
        out += (state.mIsFinal ? "true" : "false");
        out += "\n";
        
        for (auto const& [input, destinations] : state.mTransitions)
        {
            for (auto destination: destinations)
            {
                out += std::to_string(state.mId);
                out += " -> ";
                out += std::to_string(destination);
                out += " : ";
                out += (input == kEpsilon ? std::string{"null"} : std::to_string(input));
                out += "\n";
            }
        }
    }

    return out;
}

DFA NFA::toDFA()
{
    // The NFA built via Thompson-Construction contains epsilon
    // transitions and called an "Epsilon NFA". The Epsilon NFA
    // must be transformed by eliminating the epsilon
    EpsilonNFAToNFAConversion();

    // Next, convert the NFA to a DFA
    auto dfa = NFAToDFAConversion();

    // Finally, minimize the DFA
    dfa.minimizeDFA();

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
    for(auto c : mAlphabet)
    {
        for (auto state : mStates)
        {
            for (auto reachableByEpsilonClosure1 : map.at(state.mId))
            {
                for(auto destination : mStates.at(reachableByEpsilonClosure1).mTransitions[c])
                {
                    for (auto reachableByEpsilonClosure2 : map.at(destination))
                    {
                        reachableByEpsilonClosureSet.insert(reachableByEpsilonClosure2);
                    }
                }
            }

            // insert into new nfa
            for (auto reachableByEpsilon : reachableByEpsilonClosureSet)
            {
                newNFA.addTransition(c, state.mId, reachableByEpsilon);
            }
            reachableByEpsilonClosureSet.clear();
        }
    }

    //STEP3: replace old NFA with new NFA
    *this = std::move(newNFA);
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

        for (auto transition : mStates.at(state).mTransitions[kEpsilon])  //mTransitions[kEpsilon] will create empy vector. Could optimize
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

bool NFA::ContainsFinalState(const std::vector<StateId>& composite )  //TODO clean up. Rename?
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
    using StateMapper = Bimap<std::vector<StateId>, StateId, VectorHasher>;  //TODO change to set<StateId>

    DFA dfa(mAlphabet);
    std::deque<StateId> queue;
    StateMapper mapper;
    std::set<StateId> set;

    auto dfaState = dfa.addState(true, mStates.at(mStartState).mIsFinal);
    mapper.insert(dfaState, {mStartState});
    queue.push_back(dfaState);

    while(!queue.empty())
    {
        dfaState = queue.front();
        queue.pop_front();

        auto nfaStates = mapper.get(dfaState);

        for(auto c : mAlphabet)
        {
            set.clear();

            for(auto nfaState : nfaStates)
            {
                // calc  union of all dest states
                auto destinations = mStates.at(nfaState).mTransitions[c];
                std::copy(destinations.begin(), destinations.end(), std::inserter(set, set.end()));
            }

            std::vector<StateId> composite(set.begin(), set.end());  // TODO call this union?
            auto newDfaState = StateId{kNullState};
            if(mapper.contains(composite))
            {
                newDfaState = mapper.get(composite);
            }
            else
            {
                newDfaState = dfa.addState(false, ContainsFinalState(composite));
                mapper.insert(newDfaState, composite);
                queue.push_back(newDfaState);
            }

            dfa.addTransition(c, dfaState, newDfaState);
        }
    }

    return dfa;
}

} //namespace automata

