
#pragma once 

#include "vector"
#include "string"
#include "list"
#include "functional"

#define epsilon char(0)

//forward declarations
class State;
class Transition;
class FSM;

//TODO template input?
class Transition
{
public:
    Transition(State& target, char input);

    State& targetState();
    char input();

private:
    State& mTarget;
    char mInput;
};


//TODO template input?
class State
{
public:
    State(bool isFinal);

    void addTransitionTo(State& to, char input);

    void addTransitionFrom(State& from, char input);

    int numTransitions();
    bool isFinal();

    bool operator==(const State& other);
    bool operator!=(const State& other);

    State* run(char c);

    friend std::ostream& operator<<(std::ostream& os, const State& state);

    int mId; // maybe create accessor? useful for plantUml
    std::vector<Transition> mTransitions; //maybe switch to map? // maybe make protected? useful for plantUML
private:
    
    
    bool mIsFinal;
    static int count;
};




class FSM
{
public:
    FSM();

    State& addState(bool isStart, bool isFinal);

    State& startState(); //maybe remove? only testing

    bool run(std::string string);

    std::string toPlantUML();

    // todo: make private
    // todo: change to use a predicate?
    bool isReachableIf(State& start, State& end, std::function<bool(char)>);

private:

    

    std::list<State> mStates;
    State* mStartState;


};

bool isValidRegex(std::string regex);
std::string PreprocessRegex(std::string regex);
std::string RegexInfixToPostfix(std::string regex);
std::string RegexPostfixToInfix(std::string postfix);
void regexToNFA(FSM& fsm, std::string regex);