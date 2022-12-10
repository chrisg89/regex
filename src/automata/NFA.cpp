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

// todo: This hasher is too expensive (especially on non-sequential containers such as sets). 
// Need to find another way to create a bi-map or replace the bi-map entirely
struct SetHasher {
    std::size_t operator()(const std::set<StateId> &V) const {
        auto hash = V.size();
        for(const auto i : V) {
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
    , mStartState{}
    , mFinalStates{}
    , mAlphabet{alphabet}
{}

StateId NFA::addState(bool isStart, bool isFinal)
{
    if(isStart)
    {
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

DFA NFA::makeDFA() const
{
    // Convert the NFA to a DFA
    auto dfa = buildDFA();

    // Minimize the DFA
    dfa.minimize();

    return dfa;
}

bool isReachableByEpsilonClosure(const EpsilonClusureMap& map, StateId source, StateId destination)
{
    auto& reachable = map.at(source);

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
            const auto state = stack.top();
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

void NFA::removeEpsilonTransitions()
{
    NFA newNFA(mAlphabet);

    const auto map = CreateEpsilonClosureMap();

    // STEP1: Calculate the new states and insert them into the new NFA

    // Thompson-Contruction should yield an epsilon-NFA with only one final state
    assert(mFinalStates.size() == 1);
    const auto finalState = mFinalStates.front();
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

DFA NFA::buildDFA() const
{
    using StateMapper = Bimap<std::set<StateId>, StateId, SetHasher>;

    DFA dfa(mAlphabet);
    std::deque<StateId> queue;
    StateMapper mapper;

    auto dfaState = dfa.addState(true, mStates.at(mStartState).IsFinal);
    mapper.insert(dfaState, {mStartState});
    queue.push_back(dfaState);

    while(!queue.empty())
    {
        dfaState = queue.front();
        queue.pop_front();

        const auto& nfaStates = mapper.get(dfaState);

        std::set<StateId> set;
        for(const auto c : mAlphabet)
        {
            set.clear();

            for(const auto nfaState : nfaStates)
            {
                const auto& transitions = mStates.at(nfaState).Transitions;
                if(transitions.count(c))
                {
                    const auto& destinations = transitions.at(c);
                    std::copy(destinations.begin(), destinations.end(), std::inserter(set, set.end()));
                }
            }

            StateId newDfaState;
            if(mapper.contains(set))
            {
                newDfaState = mapper.get(set);
            }
            else
            {
                const auto isFinal = std::any_of(set.begin(), set.end(), 
                    [this](auto stateId)
                    {
                        return mStates[stateId].IsFinal;
                    });
                newDfaState = dfa.addState(false, isFinal);
                mapper.insert(newDfaState, set);
                queue.push_back(newDfaState);
            }

            dfa.addTransition(c, dfaState, newDfaState);
        }
    }

    return dfa;
}

} //namespace automata

