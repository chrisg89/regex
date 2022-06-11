#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Regex.hpp"

namespace regex{

namespace {

SCENARIO( "Empty", "[empty]" ) 
{
       //FSM fsm;
    //std::string regex("(a|bd)");
    //regexToNFA(fsm, regex);

    //iteratre over all states and find all final states
    // assert only one final state
    //fsm.isReachable(star, ) // move to fsm unit test and create manual state machine
    
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


SCENARIO( "Regex", "[regex]" ) 
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

        //TODO this testing was not super useful and a bug slipped through
        // regarding operator prio. Need to test differently

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

} // namespace 
} // namespace regex