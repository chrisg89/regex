#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "FSM.hpp"


SCENARIO( "Empty", "[empty]" ) 
{


    SECTION("TODO preprocess")
    {
        //TODO unit test this
        std::cout << PreprocessRegex("abc*a|b") << std::endl;
    }

    SECTION("Validate Regex")
    {

        SECTION("Valid (Good) Regex")
        {
            auto goodRegex = GENERATE(
                //TODO add more here. look at matrix!
                "abc*a|b",
                "(a)(b)*c|d*"
            );

            WHEN( "given a the valid regex: " << goodRegex ) 
            {
                THEN( "the validator passes" ) 
                {
                    REQUIRE(isValidRegex(goodRegex));
                }
            }
        }

        SECTION("Invalid (Bad) Regex")
        {
            auto badRegex = GENERATE(

                "abc|",   // cant end in |
                "abc(",   // cant end in (
                "abc(a",  // only one left bracket
                "abc)a",  // only one right bracket
                "a)b(",   // right bracket before left bracket
                "a(b(c)", // mismatching number of brackets
                "a()c",    // () is invalid
                "ab(*c)",  // (* is invalid
                "ab(|c)",  // (| is invalid
                "ab(a|)c", // |) is invalid
                "ab|*c",   // |* is invalid
                "ab||a"    // || is invalid
            );

            WHEN( "given a the valid regex: " << badRegex ) 
            {
                THEN( "the validator fails" ) 
                {
                    REQUIRE(!isValidRegex(badRegex));
                }
            }
        }

















    }


    SECTION("TODO")
    {
        std::string infix1;
        std::string postfix1;
        std::string infix2;
        std::string postfix2;

        infix1 = "a&b&c*&a|b";
        std::cout << "infix1: " << infix1 << std::endl;

        postfix1 = RegexInfixToPostfix(infix1);
        std::cout << "postfix1: " <<postfix1 << std::endl;

        infix2 = RegexPostfixToInfix(postfix1);
        std::cout << "infix2: " << infix2 << std::endl;

        postfix2 = RegexInfixToPostfix(infix2);
        std::cout << "postfix2: " <<postfix2 << std::endl;

        // we cycle through infix/postfix twice because of parenthesis matching
        REQUIRE((postfix1 == postfix2));
    }


    SECTION("TODO")
    {
        std::string infix1;
        std::string postfix1;
        std::string infix2;
        std::string postfix2;

        infix1 = "a&(a|b|c)*&a";
        std::cout << "infix1: " << infix1 << std::endl;

        postfix1 = RegexInfixToPostfix(infix1);
        std::cout << "postfix1: " <<postfix1 << std::endl;

        infix2 = RegexPostfixToInfix(postfix1);
        std::cout << "infix2: " << infix2 << std::endl;

        postfix2 = RegexInfixToPostfix(infix2);
        std::cout << "postfix2: " <<postfix2 << std::endl;

        // we cycle through infix/postfix twice because of parenthesis matching
        REQUIRE((postfix1 == postfix2));
    }

    SECTION("TODO")
    {
        std::string infix1;
        std::string postfix1;
        std::string infix2;
        std::string postfix2;

        infix1 = "a&b&c|d&e|f|g|h&i";
        std::cout << "infix1: " << infix1 << std::endl;

        postfix1 = RegexInfixToPostfix(infix1);
        std::cout << "postfix1: " <<postfix1 << std::endl;

        infix2 = RegexPostfixToInfix(postfix1);
        std::cout << "infix2: " << infix2 << std::endl;

        postfix2 = RegexInfixToPostfix(infix2);
        std::cout << "postfix2: " <<postfix2 << std::endl;

        // we cycle through infix/postfix twice because of parenthesis matching
        REQUIRE((postfix1 == postfix2));
    }

    SECTION("TODO")
    {
        std::string infix1;
        std::string postfix1;
        std::string infix2;
        std::string postfix2;

        infix1 = "a**";
        std::cout << "infix1: " << infix1 << std::endl;

        postfix1 = RegexInfixToPostfix(infix1);
        std::cout << "postfix1: " <<postfix1 << std::endl;

        infix2 = RegexPostfixToInfix(postfix1);
        std::cout << "infix2: " << infix2 << std::endl;

        postfix2 = RegexInfixToPostfix(infix2);
        std::cout << "postfix2: " <<postfix2 << std::endl;

        // we cycle through infix/postfix twice because of parenthesis matching
        REQUIRE((postfix1 == postfix2));
    }
}

SCENARIO( "API", "[api]" ) 
{

    FSM fsm;

    auto& state1 = fsm.addState(true, false);
    auto& state2 = fsm.addState(false, false);
    auto& state3 = fsm.addState(false, true);

    REQUIRE((state1 == fsm.startState())); //TODO sm.startState() error if no start state specified
    REQUIRE((state2 != fsm.startState()));
    REQUIRE((state3 != fsm.startState()));

    REQUIRE((state1 == state1));
    REQUIRE((state1 != state2));
    REQUIRE((state1 != state3));

    REQUIRE( state1.numTransitions() == 0 );
    REQUIRE( state2.numTransitions() == 0 );
    REQUIRE( state3.numTransitions() == 0 );

    state1.addTransitionTo(state2, 'a');
    REQUIRE( state1.numTransitions() == 1 );
    REQUIRE( state2.numTransitions() == 0 );
    REQUIRE( state3.numTransitions() == 0 );

    state3.addTransitionFrom(state2, 'b');
    REQUIRE( state1.numTransitions() == 1 );
    REQUIRE( state2.numTransitions() == 1 );
    REQUIRE( state3.numTransitions() == 0 );

    std::cout << state1 << std::endl;
    std::cout << state2 << std::endl;
    std::cout << state3 << std::endl;

    REQUIRE( fsm.run("ab") );

}

SCENARIO( "FSM used in Deterministic Finite Automata", "[DFA]" ) 
{
    SECTION("DFA that accepts binary numbers that are multiple of three")
    {
       GIVEN( "a DFA that accepts the language" ) 
       {
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
                auto string = GENERATE(
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

                WHEN( "given valid input of: " << string ) 
                {
                    THEN( "the input is accepted by the DFA" ) 
                    {
                        REQUIRE( fsm.run(string) );
                    }
                }
            }

            SECTION("Invalid input (non-conforming to the language)")
            {
                auto string = GENERATE(
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

                WHEN( "given invalid input of: " << string ) 
                {
                    THEN( "the input is accepted by the DFA" ) 
                    {
                        REQUIRE( !fsm.run(string) );
                    }
                }
            }
        }
    }
}