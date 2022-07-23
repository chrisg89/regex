
#pragma once 

#include "FA.hpp"

#include <vector>
#include <map>
#include <string>
#include <unordered_map>


namespace fa
{

constexpr char kNullPartition = -1;
using PartitionId = int;
using ParitionMap = std::unordered_map<StateId,PartitionId>;  //todo change to vector?

class DFAState
{
public:
    DFAState(StateId id, bool isStart, bool isFinal);

    void addTransition(char input, StateId destination);

//private:
    StateId mId;
    bool mIsStart;
    bool mIsFinal;
    bool mIsDead;
    std::map<char, StateId> mTransitions;
};


class DFA
{
public:
    DFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(char input, StateId source, StateId destination);

    std::string serialize();

    bool run(std::string string);

    void minimizeDFA();

private:
    std::vector<DFAState> mStates;
    Alphabet mAlphabet; // Todo reference?

    StateId mStateCount; //TODO this should be an int
    StateId mStartState;
    std::vector<StateId> mFinalStates;

    bool checkEquivalence(ParitionMap paritionMap, StateId stateA, StateId stateB);
    void markDeadStates();
};


} //namespace fa


