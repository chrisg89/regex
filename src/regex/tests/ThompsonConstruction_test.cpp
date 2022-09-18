#include "catch.hpp"
#include "ThompsonConstruction.hpp"

#include <stdexcept>

namespace regex {

//TODO test each construction seperately
//TODO test top level seperately

namespace {

SCENARIO( "ThompsonConstruction", "[ThompsonConstruction]" ) 
{
    SECTION("Thomson construction method produces a epsilon-NFA")
    {
        /*
        GIVEN( "an alphabet of <a,b,c>: ") 
        {
            auto alphabet = fa::Alphabet{'a', 'b', 'c'};

            WHEN( "the nfa is built using Thompson construction building blocks for a(a|b|c)*a")
            {
                NFA nfa(alphabet);

                auto n1  = buildSymbol(nfa, Range{'a','a'});
                auto n2  = buildSymbol(nfa, Range{'b','b'});
                auto n3  = buildUnion(nfa, n1, n2);
                auto n4  = buildSymbol(nfa, Range{'c','c'});
                auto n5  = buildUnion(nfa, n3, n4);
                auto n6  = buildClosure(nfa, n5);
                auto n7  = buildSymbol(nfa, Range{'a','a'});
                auto n8  = buildConcatenation(nfa, n7, n6);
                auto n9  = buildSymbol(nfa, Range{'a','a'});
                auto n10 = buildConcatenation(nfa, n8, n9);
                
                auto start = nfa.addState(true, false);
                auto final = nfa.addState(false, true);

                nfa.addTransition(fa::kEpsilon, start, n10.entry);
                nfa.addTransition(fa::kEpsilon, n10.exit, final);

                THEN( "the correct epsilon nfa should be generated" ) 
                {
                    auto expectedNFA = std::string(
                        "0 : Start = false | Final = false\n"
                        "0 -> 1 : 97\n"
                        "1 : Start = false | Final = false\n"
                        "1 -> 5 : null\n"
                        "2 : Start = false | Final = false\n"
                        "2 -> 3 : 98\n"
                        "3 : Start = false | Final = false\n"
                        "3 -> 5 : null\n"
                        "4 : Start = false | Final = false\n"
                        "4 -> 0 : null\n"
                        "4 -> 2 : null\n"
                        "5 : Start = false | Final = false\n"
                        "5 -> 9 : null\n"
                        "6 : Start = false | Final = false\n"
                        "6 -> 7 : 99\n"
                        "7 : Start = false | Final = false\n"
                        "7 -> 9 : null\n"
                        "8 : Start = false | Final = false\n"
                        "8 -> 4 : null\n"
                        "8 -> 6 : null\n"
                        "9 : Start = false | Final = false\n"
                        "9 -> 8 : null\n"
                        "9 -> 11 : null\n"
                        "10 : Start = false | Final = false\n"
                        "10 -> 8 : null\n"
                        "10 -> 11 : null\n"
                        "11 : Start = false | Final = false\n"
                        "11 -> 14 : null\n"
                        "12 : Start = false | Final = false\n"
                        "12 -> 13 : 97\n"
                        "13 : Start = false | Final = false\n"
                        "13 -> 10 : null\n"
                        "14 : Start = false | Final = false\n"
                        "14 -> 15 : 97\n"
                        "15 : Start = false | Final = false\n"
                        "15 -> 17 : null\n"
                        "16 : Start = true | Final = false\n"
                        "16 -> 12 : null\n"
                        "17 : Start = false | Final = true\n");

                    REQUIRE(nfa.serialize() == expectedNFA);
                }
            }
        }
        */
    }

}

} //namespace
} //namespace regex 