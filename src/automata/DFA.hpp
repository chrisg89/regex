
#pragma once

#include "Automata.hpp"

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace automata
{

using PartitionId = unsigned int;
using ParitionMap = std::unordered_map<StateId, PartitionId>;

class DFAState
{
public:
    DFAState(StateId id, bool isStart, bool isFinal);

    void addTransition(InputType input, StateId destination);

    const StateId Id;
    const bool IsStart;
    const bool IsFinal;
    bool IsDead{ true };
    std::map<InputType, StateId> Transitions;
};

class DFA
{
public:
    explicit DFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(InputType input, StateId source, StateId destination);

    [[nodiscard]] std::string serialize() const;

    [[nodiscard]] StateId step(StateId current, InputType input) const;
    [[nodiscard]] StateId getStartState() const;
    [[nodiscard]] bool isDeadState(StateId current) const;
    [[nodiscard]] bool isFinalState(StateId current) const;

    void minimize();

private:
    std::vector<DFAState> mStates;
    unsigned int mStateCount{ 0 };
    StateId mStartState{};
    std::vector<StateId> mFinalStates;
    Alphabet mAlphabet;

    [[nodiscard]] bool checkEquivalence(const ParitionMap& paritionMap,
                                        StateId stateA,
                                        StateId stateB) const;
};

} // namespace automata
