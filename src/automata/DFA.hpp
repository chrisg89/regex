
#pragma once 

#include "Automata.hpp"

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace automata
{

using PartitionId = int;
using ParitionMap = std::unordered_map<StateId,PartitionId>;
constexpr PartitionId kNullPartition = -1;

class DFAState
{
public:
    DFAState(StateId id, bool isStart, bool isFinal);

    void addTransition(InputType input, StateId destination);

    const StateId Id;
    const bool IsStart;
    const bool IsFinal;
    bool IsDead;
    std::map<InputType, StateId> Transitions;
};

class DFA
{
public:

    explicit DFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(InputType input, StateId source, StateId destination);

    std::string serialize() const;

    StateId step(StateId current, InputType input) const;
    StateId getStartState() const;
    bool isDeadState(StateId current) const;
    bool isFinalState(StateId current) const;

    void minimize();

private:
    std::vector<DFAState> mStates;
    Alphabet mAlphabet;

    uint mStateCount;
    StateId mStartState;
    std::vector<StateId> mFinalStates;

    bool checkEquivalence(const ParitionMap& paritionMap, StateId stateA, StateId stateB) const;
};

} //namespace automata


