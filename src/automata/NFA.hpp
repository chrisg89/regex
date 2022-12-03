
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

//private:
    StateId mId;
    bool mIsStart;
    bool mIsFinal;
    std::map<InputType, std::vector<StateId>> mTransitions;
};

class NFA
{
public:
    NFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(InputType input, StateId source, StateId destination);

    DFA toDFA();

    std::string serialize();

private:

    void EpsilonNFAToNFAConversion();
    EpsilonClusureMap CreateEpsilonClosureMap();
    bool isReachableByEpsilonClosure(StateId source, StateId destination);

    DFA NFAToDFAConversion();
    bool ContainsFinalState(const std::vector<StateId>& composite );

    std::vector<NFAState> mStates;
    Alphabet mAlphabet;

    uint mStateCount;
    StateId mStartState;
    std::vector<StateId> mFinalStates;
};

} //namespace automata


