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

    /*
    {

        std::string regex("a|b");
        auto tokenStream = TokenStream(regex);
        auto preprocessed = PreprocessRegex(tokenStream);
        auto postfix = InfixToPostfix(preprocessed);
        auto nfa = regex::regexToNFA(postfix);

        //std::cout << nfa.toPlantUML();

        auto dfa = nfa.toDFA();

        auto plantNFA = nfa.toPlantUML();

        
        //TODO need to implement checks for other known NFAs as well
        auto expectedNFA = std::string(
            "@startuml\n"
            "hide empty description\n"
            "[*] --> 6\n"
            "0 -> 7 : a\n"
            "0 -> 5 : a\n"
            "0 -> 1 : a\n"
            "1 : Final\n"
            "2 -> 7 : b\n"
            "2 -> 5 : b\n"
            "2 -> 3 : b\n"
            "3 : Final\n"
            "4 -> 7 : a\n"
            "4 -> 5 : a\n"
            "4 -> 1 : a\n"
            "4 -> 7 : b\n"
            "4 -> 5 : b\n"
            "4 -> 3 : b\n"
            "5 : Final\n"
            "6 -> 7 : a\n"
            "6 -> 5 : a\n"
            "6 -> 1 : a\n"
            "6 -> 7 : b\n"
            "6 -> 5 : b\n"
            "6 -> 3 : b\n"
            "7 : Final\n"
            "@enduml\n");
        REQUIRE(plantNFA == expectedNFA);

        auto plantDFA = dfa.toPlantUML();
        auto expectedDFA= std::string(
            "@startuml\n"
            "hide empty description\n"
            "[*] --> 0\n"
            "0 -> 1 : a\n"
            "0 -> 2 : b\n"
            "1 : Final\n"
            "1 -> 3 : a\n"
            "1 -> 3 : b\n"
            "2 : Final\n"
            "2 -> 3 : a\n"
            "2 -> 3 : b\n"
            "3 -> 3 : a\n"
            "3 -> 3 : b\n"
            "@enduml\n");
        REQUIRE(plantDFA == expectedDFA);
        
    }
    */
    

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

SCENARIO( "NFA", "[empty]" ) 
{
    /*
    SECTION("Check if a node is reachable from another on a* (a-closure) ")
    {
        FSM fsm;
        auto& stateA = fsm.addState(true, false);
        auto& stateB = fsm.addState(false, false);
        auto& stateC = fsm.addState(false, false); 
        auto& stateD = fsm.addState(false, false);  

        stateA.addTransitionTo(stateB, 'a');
        stateB.addTransitionTo(stateC, 'a');
        stateB.addTransitionTo(stateD, 'b');
        stateC.addTransitionTo(stateD, 'a');

        std::function<bool(char)> aClosure = 
        [](char c)
        {
            return (c == 'a');
        };

        // State A -> others
        REQUIRE(fsm.isReachableIf(stateA, stateB, aClosure));
        REQUIRE(fsm.isReachableIf(stateA, stateC, aClosure));
        REQUIRE(fsm.isReachableIf(stateA, stateD, aClosure));

        // State B -> others
        REQUIRE(!fsm.isReachableIf(stateB, stateA, aClosure));
        REQUIRE(fsm.isReachableIf(stateB, stateC, aClosure));
        REQUIRE(fsm.isReachableIf(stateB, stateD, aClosure));

        // State C -> others
        REQUIRE(!fsm.isReachableIf(stateC, stateA, aClosure));
        REQUIRE(!fsm.isReachableIf(stateC, stateB, aClosure));
        REQUIRE(fsm.isReachableIf(stateC, stateD, aClosure));

        // State D -> others
        REQUIRE(!fsm.isReachableIf(stateD, stateA, aClosure));
        REQUIRE(!fsm.isReachableIf(stateD, stateB, aClosure));
        REQUIRE(!fsm.isReachableIf(stateD, stateC, aClosure));

    }
    */
}

} //namespace
} //namespace fa