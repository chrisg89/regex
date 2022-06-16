
#pragma once 

#include "DFA.hpp"

#include <vector>
#include <iostream>
#include <map>

namespace nfa
{


//forward declarations
class State;
class Transition;
class NFA;

using DFA = dfa::DFA;
using StateId = int;
using Alphabet = std::vector<char>;
using EpsilonClusureMap = std::map<StateId,std::vector<StateId>>;


constexpr char kNullState = -1;
constexpr char kEpsilon = 0;

struct Transition
{
    Transition(char input, StateId destination);

    char input;
    StateId destination;
};

class State
{
public:
    State(StateId id, bool isStart, bool isFinal);

    void addTransition(char input, StateId destination);

//private:
    StateId mId;
    bool mIsStart;
    bool mIsFinal;
    std::map<char, std::vector<Transition>> mTransitions;
};

class NFA
{
public:
    NFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(char input, StateId source, StateId destination);

    DFA toDFA();

    std::string toPlantUML();

private:

    void EpsilonNFAToNFAConversion();
    EpsilonClusureMap CreateEpsilonClosureMap();
    bool isReachableByEpsilonClosure(StateId source, StateId destination);

    void NFAToDFAConversion();

    std::vector<State> mStates;
    Alphabet mAlphabet;

    StateId mStateCount;
    StateId mStartState;
    std::vector<StateId> mFinalStates;
    
};

} //namespace nfa


