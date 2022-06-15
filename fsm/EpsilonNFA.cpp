
#include "EpsilonNFA.hpp"

#include <stack>
#include <algorithm>

namespace epsilon_nfa
{

EpsilonNFA::EpsilonNFA(Alphabet alphabet)
    :parent(alphabet)
{}


EpsilonNFA EpsilonNFA::toNFA()
{
    EpsilonNFA nfa(mAlphabet);

    step1(nfa);
    step2(nfa);

    return nfa;
}

void EpsilonNFA::step1(EpsilonNFA& nfa)
{

}

void EpsilonNFA::step2(EpsilonNFA& nfa)
{

    auto map = CreateEpsilonClosureMap();
/*
    State* final;

    std::map<int,State*> stateMapping;

    // the constructed epsilon NFA should
    // only have one final state
    for (auto state : mStates)
    {
        if (state.isFinal())
        {
            final = &state;
            break;
        }
        // todo: add start and stop
        stateMapping[state.mId] = &nfa.addState(false, false);

    }

    

    for(auto character : mAlphabet)
    {
        for (auto state : mStates)
        {
            for (auto reachableByEpsilonClosure1 : map[state.mId])
            {
                auto target = reachableByEpsilonClosure1->run(character);

                if(target != nullptr)
                {
                    for (auto reachableByEpsilonClosure2 : map[target->mId])
                    {
                        //todo: remove insertaion of duplicates.
                        auto mappedStartState = stateMapping[state.mId];
                        auto mappedTargetState = stateMapping[target->mId];
                        mappedStartState->addTransitionTo(*mappedTargetState, character);

                        // populate tempunique container

                    }
                }
            }

            // transfer from unique container to nfa
        }
    }
*/
}

EpsilonClusureMap EpsilonNFA::CreateEpsilonClosureMap()
{

    EpsilonClusureMap map;

    for (auto first : mStates)
    {
        std::vector<State*> statesByEpsilonClosure;

        for (auto second : mStates)
        {
            if(isReachableByEpsilonClosure(first, second))
            {
                map[first.mId].push_back(&second);
            }
        }
    }

    return map;

}

bool EpsilonNFA::isReachableByEpsilonClosure(State& start, State& end)
{

    std::stack<State*> stack;
    std::vector<State*> visited;

    stack.push(&start);
    visited.push_back(&start);

    while(!stack.empty())
    {
        auto state = stack.top();
        stack.pop();

        for (auto transition : state->mTransitions)
        {
            if(transition.input() == epsilon)
            {
                auto adjacent = transition.targetState();

                if(adjacent == end)
                {
                    return true;
                }

                if (std::find(visited.begin(), visited.end(), &adjacent) == visited.end())
                {
                    stack.push(&adjacent);
                    visited.push_back(&adjacent);
                }
            }
        }
    }

    return false;
}


} //namespace epsilon_nfa