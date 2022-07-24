
#pragma once 

#include "FA.hpp"
#include "DFA.hpp"

#include <vector>
#include <iostream>
#include <map>


namespace fa
{




using EpsilonClusureMap = std::map<StateId,std::vector<StateId>>;





class NFAState
{
public:
    NFAState(StateId id, bool isStart, bool isFinal);

    void addTransition(char input, StateId destination);

//private:
    StateId mId;
    bool mIsStart;
    bool mIsFinal;
    std::map<char, std::vector<StateId>> mTransitions;
};

class NFA
{
public:
    NFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(char input, StateId source, StateId destination);

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


} //namespace fa


