#include "catch.hpp"
#include "ThompsonConstruction.hpp"

#include <stdexcept>

namespace regex {

namespace {

SCENARIO( "ThompsonConstruction", "[ThompsonConstruction]" ) 
{
    // There is no easy way to unit test thompson construction
    // No unit test provided. However, the code below serves
    // as an example of how to use the algorithm

    NFA nfa(fa::Alphabet{'a', 'b', 'c'});

    //a(a|b|c)*a
    auto n1  = buildSymbol(nfa, 'a');
    auto n2  = buildSymbol(nfa, 'b');
    auto n3  = buildUnion(nfa, n1, n2);
    auto n4  = buildSymbol(nfa, 'c');
    auto n5  = buildUnion(nfa, n3, n4);
    auto n6  = buildClosure(nfa, n5);
    auto n7  = buildSymbol(nfa, 'a');
    auto n8  = buildConcatenation(nfa, n7, n6);
    auto n9  = buildSymbol(nfa, 'a');
    auto n10 = buildConcatenation(nfa, n8, n9);
    
    auto start = nfa.addState(true, false);
    auto final = nfa.addState(false, true);

    nfa.addTransition(fa::kEpsilon, start, n10.entry);
    nfa.addTransition(fa::kEpsilon, n10.exit, final);

}

} //namespace
} //namespace regex 