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
    : Id{id}
    , IsStart{isStart}
    , IsFinal{isFinal}
    , Transitions{}
{}

void NFAState::addTransition(InputType input, StateId destination)
{
    Transitions[input].emplace_back(destination);
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

std::string NFA::serialize() const
{
    std::string out = "";
    
    for (const auto& state : mStates)
    {
        out += std::to_string(state.Id);
        out += " : Start = ";
        out += (state.IsStart? "true" : "false");
        out += " | Final = ";
        out += (state.IsFinal ? "true" : "false");
        out += "\n";
        
        for (auto const& [input, destinations] : state.Transitions)
        {
            for (const auto destination: destinations)
            {
                out += std::to_string(state.Id);
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

bool isReachableByEpsilonClosure(EpsilonClusureMap& map, StateId source, StateId destination)
{
    auto& reachable = map[source];

    if (std::find(reachable.begin(), reachable.end(), destination) != reachable.end())
    {
        return true;
    }

    return false;
}

EpsilonClusureMap NFA::CreateEpsilonClosureMap() const
{
    EpsilonClusureMap map;

    for (const auto& source : mStates)
    {
        auto& reachable = map[source.Id];

        std::stack<StateId> stack;

        stack.push(source.Id);
        reachable.push_back(source.Id);

        while(!stack.empty())
        {
            auto state = stack.top();
            stack.pop();

            const auto& transitions = mStates.at(state).Transitions;
            if(transitions.count(kEpsilon))
            {
                for (const auto adjacent : transitions.at(kEpsilon))
                {
                    if (std::find(reachable.begin(), reachable.end(), adjacent) == reachable.end())
                    {
                        stack.push(adjacent);
                        reachable.push_back(adjacent);
                    }
                }
            }
        }
    }

    return map;
}

void NFA::EpsilonNFAToNFAConversion()
{
    NFA newNFA(mAlphabet);

    auto map = CreateEpsilonClosureMap();

    // STEP1: Calculate the new states and insert them into the new NFA

    // Thompson-Contruction should yield an epsilon-NFA with only one final state
    assert(mFinalStates.size() == 1);
    auto finalState = mFinalStates.front();
    for (const auto& state : mStates)
    {
        newNFA.addState(state.IsStart, isReachableByEpsilonClosure(map, state.Id, finalState));
    }

    // STEP2: Calculate the new transitions and insert them into the new NFA
    std::unordered_set<StateId> reachableByEpsilonClosureSet;
    for(const auto c : mAlphabet)
    {
        for (const auto& state : mStates)
        {
            for (const auto reachableByEpsilonClosure1 : map.at(state.Id))
            {
                for(const auto destination : mStates.at(reachableByEpsilonClosure1).Transitions[c])
                {
                    for (const auto reachableByEpsilonClosure2 : map.at(destination))
                    {
                        reachableByEpsilonClosureSet.insert(reachableByEpsilonClosure2);
                    }
                }
            }

            // insert into new nfa
            for (const auto reachableByEpsilon : reachableByEpsilonClosureSet)
            {
                newNFA.addTransition(c, state.Id, reachableByEpsilon);
            }
            reachableByEpsilonClosureSet.clear();
        }
    }

    //STEP3: replace old NFA with new NFA
    *this = std::move(newNFA);
}

bool NFA::ContainsFinalState(const std::vector<StateId>& composite )  //TODO clean up. Rename?
{
    for(const auto state1 : composite)
    {
        for(const auto state2 : mFinalStates)
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

    auto dfaState = dfa.addState(true, mStates.at(mStartState).IsFinal);
    mapper.insert(dfaState, {mStartState});
    queue.push_back(dfaState);

    while(!queue.empty())
    {
        dfaState = queue.front();
        queue.pop_front();

        auto nfaStates = mapper.get(dfaState);

        for(const auto c : mAlphabet)
        {
            set.clear();

            for(const auto nfaState : nfaStates)
            {
                // calc  union of all dest states
                auto destinations = mStates.at(nfaState).Transitions[c];
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

