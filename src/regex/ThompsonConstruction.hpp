#pragma once

#include "NFA.hpp"

namespace regex
{

using NFA = fa::NFA;
using StateId = fa::StateId;


struct BlackBox
{
    BlackBox(StateId entry, StateId exit);
    StateId entry;
    StateId exit;
};

BlackBox buildSymbol(NFA& nfa, char c);

BlackBox buildUnion(NFA& nfa, BlackBox& BB1, BlackBox& BB2);

BlackBox buildConcatenation(NFA& nfa, BlackBox& BB1, BlackBox& BB2);

BlackBox buildClosure(NFA& nfa, BlackBox& BB);

} //namespace regex