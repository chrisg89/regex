#pragma once

#include "NFA.hpp"
#include "TokenStream.hpp"

namespace regex
{

using NFA = fa::NFA;
using StateId = fa::StateId;
using Alphabet = fa::Alphabet;


NFA ThompsonConstruction(TokenStream regex, Alphabet& alphabet);

struct BlackBox
{
    BlackBox(StateId entry, StateId exit);
    StateId entry;
    StateId exit;
};

BlackBox buildSymbol(NFA& nfa, char c);

BlackBox buildAny(NFA& nfa, Alphabet& alphabet);

BlackBox buildDigit(NFA& nfa, Alphabet& alphabet);

BlackBox buildNonDigit(NFA& nfa, Alphabet& alphabet);

BlackBox buildWhitespace(NFA& nfa, Alphabet& alphabet);

BlackBox buildNonWhitespace(NFA& nfa, Alphabet& alphabet);

BlackBox buildWordCharacter(NFA& nfa, Alphabet& alphabet);

BlackBox buildNonWordCharacter(NFA& nfa, Alphabet& alphabet);

BlackBox buildUnion(NFA& nfa, BlackBox& BB1, BlackBox& BB2);

BlackBox buildConcatenation(NFA& nfa, BlackBox& BB1, BlackBox& BB2);

BlackBox buildClosure(NFA& nfa, BlackBox& BB);

} //namespace regex