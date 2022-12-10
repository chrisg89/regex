
#pragma once 

#include "Automata.hpp"
#include "DFA.hpp"

#include <map>
#include <vector>

namespace automata
{

using EpsilonClusureMap = std::map<StateId,std::vector<StateId>>;

class NFAState
{
public:
    NFAState(StateId id, bool isStart, bool isFinal);

    void addTransition(InputType input, StateId destination);

    const StateId Id;
    const bool IsStart;
    const bool IsFinal;
    std::map<InputType, std::vector<StateId>> Transitions;
};

class NFA
{
public:
    explicit NFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(InputType input, StateId source, StateId destination);

    DFA makeDFA() const;

    std::string serialize() const;
    
    void removeEpsilonTransitions();

private:

    EpsilonClusureMap CreateEpsilonClosureMap() const;

    DFA buildDFA() const;

    std::vector<NFAState> mStates;
    unsigned int mStateCount;
    StateId mStartState;
    std::vector<StateId> mFinalStates;
    Alphabet mAlphabet;
};

} //namespace automata


