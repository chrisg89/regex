
#pragma once 
#include <vector>
#include <map>
#include <string>

namespace dfa
{

using StateId = int;
using Alphabet = std::vector<char>;
constexpr char kNullState = -1;

class State
{
public:
    State(StateId id, bool isStart, bool isFinal);

    void addTransition(char input, StateId destination);

//private:
    StateId mId;
    bool mIsStart;
    bool mIsFinal;
    std::map<char, StateId> mTransitions;
};


class DFA
{
public:
    DFA(Alphabet alphabet);

    StateId addState(bool isStart, bool isFinal);

    void addTransition(char input, StateId source, StateId destination);

    std::string toPlantUML();

private:
    std::vector<State> mStates;
    Alphabet mAlphabet;

    StateId mStateCount;
    StateId mStartState;
    std::vector<StateId> mFinalStates;
};


} //namespace dfa


