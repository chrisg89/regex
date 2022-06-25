#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "DFA.hpp"
#include "NFA.hpp"

namespace fa{
namespace{

//TODO backup on github

//TODO rename to PLantUml to serialize()

SCENARIO( "Empty", "[empty]" ) 
{


    //iteratre over all states and find all final states
    // assert only one final state
    //fsm.isReachable(star, ) // move to fsm unit test and create manual state machine
    
    //std::cout << fsm.toPlantUML() << std::endl << regex << std::endl;
}

SCENARIO( "API", "[api]" ) 
{

    NFA nfa{Alphabet{}};

    auto state1 = nfa.addState(true, false);
    auto state2 = nfa.addState(false, false);
    auto state3 = nfa.addState(false, true);

    nfa.addTransition('a', state1, state2);
    nfa.addTransition('b', state2, state3);

}

SCENARIO( "FSM used in Deterministic Finite Automata", "[DFA]" ) 
{
    SECTION("DFA that accepts binary numbers that are multiple of three")
    {
       GIVEN( "a DFA that accepts the language" ) 
       {
            /*
            FSM fsm;
            auto& stateA = fsm.addState(true, true);
            auto& stateB = fsm.addState(false, false);
            auto& stateC = fsm.addState(false, false);
            stateA.addTransitionTo(stateA, '0');
            stateA.addTransitionTo(stateB, '1');
            stateB.addTransitionTo(stateC, '0');
            stateB.addTransitionTo(stateA, '1');
            stateC.addTransitionTo(stateB, '0');
            stateC.addTransitionTo(stateC, '1');

            SECTION("Valid input (conforming to the language)")
            {
                auto string = GENERATE
                (
                    "0",     //0
                    "11",    //3
                    "110",   //6
                    "1001",  //9
                    "1100",  //12
                    "1111",  //15
                    "10010", //18
                    "10101", //21
                    "11000", //24
                    "11011", //27
                    "11110"  //30
                );

                WHEN( "the DFA is given input: " << string ) 
                {
                    THEN( "the input is accepted by the DFA" ) 
                    {
                        REQUIRE( fsm.run(string) );
                    }
                }
            }

            SECTION("Invalid input (non-conforming to the language)")
            {
                auto string = GENERATE
                (
                    "1",     //1
                    "10",    //2
                    "100",   //4
                    "101",   //5
                    "111",   //7
                    "1000",  //8
                    "1010",  //10
                    "1011",  //11
                    "1101",  //13
                    "1110",  //14
                    "10000", //16
                    "10001", //17
                    "10011", //19
                    "10100", //20
                    "10110", //22
                    "10111", //23
                    "11001", //25
                    "11010", //26
                    "11100", //28
                    "11101"  //29
                );

                WHEN( "the DFA is given input: " << string ) 
                {
                    THEN( "the input is accepted by the DFA" ) 
                    {
                        REQUIRE( !fsm.run(string) );
                    }
                }
            }

            */
        }
    }
}

} //namespace
} //namespace fa