#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "FSM.hpp"


//TODO backup on github

SCENARIO( "Empty", "[empty]" ) 
{
    SECTION("Validate Regex")
    {
        SECTION("Valid (Good) Regex")
        {
            auto goodRegex = GENERATE
            (
                "(a)",
                "((a))",
                "(a)(b)",
                "(a)*",
                "(a)|(b)",
                "(a)b",
                "(a*)",
                "a**",
                "a|b",
                "ab"
            );

            GIVEN( "a valid regex: " << goodRegex ) 
            {
                WHEN( "the regex is validated")
                {
                    THEN( "the validator exits with success" ) 
                    {
                        REQUIRE(isValidRegex(goodRegex));
                    }
                }
            }
        }

        SECTION("Invalid (Bad) Regex")
        {
            auto badRegex = GENERATE
            (
                "ab|",    // cant end in |
                "ab(",    // cant end in (
                "a(b",    // only one left bracket
                "a)b",    // only one right bracket
                "a)b(",   // right bracket before left bracket
                "(a(b)",  // mismatching number of brackets
                "a()b",   // () is invalid
                "a(*b)",  // (* is invalid
                "a(|b)",  // (| is invalid
                "a(b|)",  // |) is invalid
                "a|*b",   // |* is invalid
                "a||b"    // || is invalid
            );

            GIVEN( "an invalid regex: " << badRegex ) 
            {
                WHEN( "the regex is validated")
                {
                    THEN( "the validator exits with error" ) 
                    {
                        REQUIRE(!isValidRegex(badRegex));
                    }
                }
            }
        }
    }

    SECTION("Pre-processing inserts concatenation operator ")
    {
        const auto [ input, output ] = GENERATE(table<std::string, std::string>
        ({
            { "ab", "a&b"},
            { "a*", "a*"},
            { "a|b", "a|b"},
            { "a(b)", "a&(b)"},
            { "(a)", "(a)"},
            { "a*b", "a*&b"},
            { "a**", "a**"},
            { "a*|b", "a*|b"},
            { "a*(b)", "a*&(b)"},
            { "(a*)", "(a*)"},
            { "a|(b)", "a|(b)"},
            { "((a))", "((a))"},
            { "(a)b", "(a)&b"},
            { "(a)*", "(a)*"},
            { "(a)|b", "(a)|b"},
            { "(a)(b)", "(a)&(b)"}
        }));

        GIVEN( "the regex: " << input ) 
        {
            WHEN( "the regex is pre-processed")
            {
                THEN( "the concatenation operator is inserted correctly" ) 
                {
                    REQUIRE(PreprocessRegex(input) == output);
                }
            }
        }
    }

    SECTION("Converting regex to postfix notation")
    {
        // In this test, we want to verify that a
        // round trip from infix->postfix->infix
        // does not alter the original expression.
        // Unfortunatly, the conversion from 
        // postfix -> infix generates brackets
        // that make the comparision difficult.
        // But, with one additional conversion back
        // to postfix, we can compare the first
        // and second postfix expressions for equality. 
        // Such a test should provide good coverage.

        std::string infix1;
        std::string postfix1;
        std::string infix2;
        std::string postfix2;
        std::string preprocessed;

        auto regex = GENERATE
        (
            "abc*a|b",
            "((a))*|b",
            "(ab)(a|b)*(a|b)|a",
            "(ab*)a**|b",
            "(ab)|a",
            "a(a|b)a*aa"
        );

        REQUIRE(isValidRegex(regex));

        infix1 = PreprocessRegex(regex);
        //std::cout << "infix1: " << infix1 << std::endl;

        GIVEN( "the regex: " << infix1 ) 
        {
            WHEN("the infix expression is converted to postfix ")
            {
                postfix1 = RegexInfixToPostfix(infix1);
                //std::cout << "postfix1: " <<postfix1 << std::endl;

                AND_THEN("the postfix expression is converted to infix ")
                {
                    infix2 = RegexPostfixToInfix(postfix1);
                    //std::cout << "infix2: " << infix2 << std::endl;

                    AND_THEN("the infix expression is converted to postfix ")
                    {

                        postfix2 = RegexInfixToPostfix(infix2);
                        //std::cout << "postfix2: " <<postfix2 << std::endl;

                        THEN( "the first and second postfix expression are identical" ) 
                        {
                            REQUIRE((postfix1 == postfix2));
                        }
                    }
                }
            }
        }
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
        }
    }
}