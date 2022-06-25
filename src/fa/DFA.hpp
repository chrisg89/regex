
#pragma once 

#include "FA.hpp"

#include <vector>
#include <map>
#include <string>


namespace fa
{

class DFAState
{
public:
    DFAState(StateId id, bool isStart, bool isFinal);

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

    bool run(std::string string);

private:
    std::vector<DFAState> mStates;
    Alphabet mAlphabet;

    StateId mStateCount;
    StateId mStartState;
    std::vector<StateId> mFinalStates;
};


} //namespace fa


