#pragma once

#include "NFA.hpp"
#include "TokenStream.hpp"
#include "Alphabet.hpp"

namespace regex
{

using NFA = fa::NFA;
using StateId = fa::StateId;

NFA ThompsonConstruction(TokenStream regex, Alphabet& alphabet);

struct BlackBox
{
    BlackBox(StateId entry, StateId exit);
    StateId entry;
    StateId exit;
};

BlackBox buildSymbol(NFA& nfa, Alphabet& alphabet, CodePointInterval& interval);

BlackBox buildUnion(NFA& nfa, BlackBox& BB1, BlackBox& BB2);

BlackBox buildConcatenation(NFA& nfa, BlackBox& BB1, BlackBox& BB2);

BlackBox buildClosure(NFA& nfa, BlackBox& BB);

BlackBox buildClosurePlus(NFA& nfa, BlackBox& BB);

BlackBox buildOptional(NFA& nfa, BlackBox& BB);

} //namespace regex