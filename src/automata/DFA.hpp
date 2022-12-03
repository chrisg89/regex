
#pragma once 

#include "Automata.hpp"

#include <vector>
#include <map>
#include <string>
#include <unordered_map>


namespace automata
{


using PartitionId = int;
using ParitionMap = std::unordered_map<StateId,PartitionId>;  //todo change to vector?
constexpr PartitionId kNullPartition = -1;

class DFAState
{
public:
    DFAState(StateId id, bool isStart, bool isFinal);

    void addTransition(InputType input, StateId destination);

//private:
    StateId mId;
    bool mIsStart;
    bool mIsFinal;
    bool mIsDead;
    std::map<InputType, StateId> mTransitions;
};


class DFA
{
public:

    DFA(){}; //TODO make pretty?
    DFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(InputType input, StateId source, StateId destination);

    std::string serialize();

    StateId step(StateId current, InputType input);
    StateId getFirst();
    bool isDeadState(StateId current);
    bool isFinalState(StateId current);

    void minimizeDFA();



private:
    std::vector<DFAState> mStates;
    Alphabet mAlphabet;

    uint mStateCount;
    StateId mStartState;
    std::vector<StateId> mFinalStates;

    bool checkEquivalence(ParitionMap paritionMap, StateId stateA, StateId stateB);
};

} //namespace automata


