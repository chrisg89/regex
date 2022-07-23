#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Regex.hpp"

namespace regex{

namespace {

SCENARIO( "Empty", "[empty]" ) 
{

    
    {
        auto regex = Regex();
        regex.compile("a|b");
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("b"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("bb"));
        REQUIRE(!regex.match("ab"));
        REQUIRE(!regex.match("ba"));
        REQUIRE(!regex.match(""));
    }

    {
        auto regex = Regex();
        regex.compile("ab");
        REQUIRE(regex.match("ab"));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("b"));
        REQUIRE(!regex.match("ba"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("bb"));
        REQUIRE(!regex.match("abb"));
        REQUIRE(!regex.match("aba"));
        REQUIRE(!regex.match(""));
    }

    {
        auto regex = Regex();
        regex.compile("a*");
        REQUIRE(regex.match(""));
        REQUIRE(regex.match("a"));
        REQUIRE(regex.match("aa"));
        REQUIRE(regex.match("aaa"));
    }

    {
        auto regex = Regex();
        regex.compile("a*(b|c)");
        REQUIRE(regex.match("b"));
        REQUIRE(regex.match("ab"));
        REQUIRE(regex.match("aab"));
        REQUIRE(regex.match("aaab"));

        REQUIRE(regex.match("c"));
        REQUIRE(regex.match("ac"));
        REQUIRE(regex.match("aac"));
        REQUIRE(regex.match("aaac"));

        REQUIRE(!regex.match(""));
        REQUIRE(!regex.match("a"));
        REQUIRE(!regex.match("aa"));
        REQUIRE(!regex.match("aaa"));

        REQUIRE(!regex.match("aba"));
        REQUIRE(!regex.match("abc"));
        REQUIRE(!regex.match("aca"));
        REQUIRE(!regex.match("acb"));
    }

    {
        auto regex = Regex();
        regex.compile("((ab)|(cd))*e");
        REQUIRE(regex.match("e"));
        REQUIRE(regex.match("abe"));
        REQUIRE(regex.match("cde"));
        REQUIRE(regex.match("ababe"));
        REQUIRE(regex.match("cdcde"));
        REQUIRE(regex.match("abcde"));
        REQUIRE(regex.match("cdabe"));
    }

    {
        auto regex = Regex();
        regex.compile("A\\|\\|B");
        REQUIRE(regex.match("A||B"));
    }

    {
        auto regex = Regex();
        regex.compile("A\\*B");
        REQUIRE(regex.match("A*B"));
    }

    {
        auto regex = Regex();
        regex.compile("\\(1+2\\)");
        REQUIRE(regex.match("(1+2)"));
    }

    {
        auto regex = Regex();
        regex.compile("while|if|void|break");
        REQUIRE(regex.match("if"));
        REQUIRE(regex.match("while"));
        REQUIRE(regex.match("void"));
        REQUIRE(regex.match("break"));

    }



//TODO: unit test this stuff below.!

    //ab (correct)
    /*
    @startuml
hide empty description
[*] --> 4
0 -> 2 : a
0 -> 1 : a
1 -> 5 : b
1 -> 3 : b
2 -> 5 : b
2 -> 3 : b
3 : Final
4 -> 2 : a
4 -> 1 : a
5 : Final
@enduml
*/




    // a*b (correct)
    /*
    @startuml
hide empty description
[*] --> 6
0 -> 4 : a
0 -> 3 : a
0 -> 1 : a
0 -> 0 : a
1 -> 4 : a
1 -> 3 : a
1 -> 1 : a
1 -> 0 : a
1 -> 7 : b
1 -> 5 : b
2 -> 4 : a
2 -> 3 : a
2 -> 1 : a
2 -> 0 : a
2 -> 7 : b
2 -> 5 : b
3 -> 7 : b
3 -> 5 : b
4 -> 7 : b
4 -> 5 : b
5 : Final
6 -> 4 : a
6 -> 3 : a
6 -> 1 : a
6 -> 0 : a
6 -> 7 : b
6 -> 5 : b
7 : Final
@enduml
*/

// a|b
/*
@startuml
hide empty description
[*] --> 6
0 -> 7 : a
0 -> 5 : a
0 -> 1 : a
1 : Final
2 -> 7 : b
2 -> 5 : b
2 -> 3 : b
3 : Final
4 -> 7 : a
4 -> 5 : a
4 -> 1 : a
4 -> 7 : b
4 -> 5 : b
4 -> 3 : b
5 : Final
6 -> 7 : a
6 -> 5 : a
6 -> 1 : a
6 -> 7 : b
6 -> 5 : b
6 -> 3 : b
7 : Final
@enduml

*/
    
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
                auto tokenStream = TokenStream(goodRegex);

                WHEN( "the regex is validated")
                {
                    THEN( "the validator exits with success" ) 
                    {
                        REQUIRE(isValidRegex(tokenStream));
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

                auto tokenStream = TokenStream(badRegex);

                WHEN( "the regex is validated")
                {
                    THEN( "the validator exits with error" ) 
                    {
                        REQUIRE(!isValidRegex(tokenStream));
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
                    auto inputStream = TokenStream(input);
                    auto preprocessed = PreprocessRegex(inputStream);
                    REQUIRE(preprocessed.toString() == output);
                }
            }
        }
    }

    
    SECTION("Converting regex to postfix notation")
    {
        const auto [ input, output ] = GENERATE(table<std::string, std::string>
        ({
            { "ab", "ab&"},
            { "a|b", "ab|"},
            { "a*b", "a*b&"},
            { "(a)", "a"},
            { "((a))", "a"},
            { "(ab)", "ab&"},
            { "(a)*", "a*"},
            { "(a|b)", "ab|"},
            { "(a*b)", "a*b&"},
            { "(a)*b", "a*b&"},
            { "(a)(b)", "ab&"},
            { "(ab)*", "ab&*"},
            { "a**", "a**"},
            { "a|b|c", "ab|c|"},
            { "abc", "ab&c&"},
            { "a(b|c)", "abc|&"},
            { "a|b|c", "ab|c|"},
            { "aa|bb|cc", "aa&bb&|cc&|"},
        }));

        GIVEN( "the infix regex: " << input ) 
        {
            WHEN( "the regex is converted to postfix")
            {
                auto tokenStream = TokenStream(input);
                REQUIRE(isValidRegex(tokenStream));
                auto infix = PreprocessRegex(tokenStream);
                auto postfix = RegexInfixToPostfix(infix);

                THEN( "the conversion yields the correct postfix expression" ) 
                {
                    REQUIRE(postfix.toString() == output);
                }
            }
        }
    }

}

} // namespace 
} // namespace regex