#include "catch.hpp"
#include "Parser.hpp"

#include <stdexcept>

namespace regex::ast{

namespace {

SCENARIO( "ThompsonConstruction", "[ThompsonConstruction]" ) 
{
    SECTION("Thomson construction method produces a epsilon-NFA")
    {
        const std::string regex = "a(a|b|c)*a";

        GIVEN("A regex" << regex) 
        {
            WHEN("The regex is parsed into an AST")
            {
                auto parser = parser::Parser(regex);
                auto ast = parser.parse();

                WHEN("The NFA is generated from the AST")
                {
                    auto nfa = ast.makeNFA(ast.makeAlphabet()); 

                    THEN("the correct epsilon nfa should be generated")
                    {
                        auto expected = std::string(
                        "0 : Start = false | Final = false\n"
                        "0 -> 1 : 1\n"
                        "1 : Start = false | Final = false\n"
                        "1 -> 2 : null\n"
                        "2 : Start = false | Final = false\n"
                        "2 -> 12 : null\n"
                        "2 -> 3 : null\n"
                        "3 : Start = false | Final = false\n"
                        "3 -> 14 : null\n"
                        "4 : Start = false | Final = false\n"
                        "4 -> 5 : 1\n"
                        "5 : Start = false | Final = false\n"
                        "5 -> 13 : null\n"
                        "6 : Start = false | Final = false\n"
                        "6 -> 7 : 2\n"
                        "7 : Start = false | Final = false\n"
                        "7 -> 11 : null\n"
                        "8 : Start = false | Final = false\n"
                        "8 -> 9 : 3\n"
                        "9 : Start = false | Final = false\n"
                        "9 -> 11 : null\n"
                        "10 : Start = false | Final = false\n"
                        "10 -> 6 : null\n"
                        "10 -> 8 : null\n"
                        "11 : Start = false | Final = false\n"
                        "11 -> 13 : null\n"
                        "12 : Start = false | Final = false\n"
                        "12 -> 4 : null\n"
                        "12 -> 10 : null\n"
                        "13 : Start = false | Final = false\n"
                        "13 -> 12 : null\n"
                        "13 -> 3 : null\n"
                        "14 : Start = false | Final = false\n"
                        "14 -> 15 : 1\n"
                        "15 : Start = false | Final = false\n"
                        "15 -> 17 : null\n"
                        "16 : Start = true | Final = false\n"
                        "16 -> 0 : null\n"
                        "17 : Start = false | Final = true\n");

                        REQUIRE(nfa.serialize() == expected);
                    }
                }
            }
        }
    }
}

} //namespace
} //namespace regex::ast