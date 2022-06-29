#include "ThompsonConstruction.hpp"

namespace regex
{

BlackBox::BlackBox(StateId entry, StateId exit)
: entry {entry}
, exit {exit}
{}

BlackBox buildSymbol(NFA& nfa, char c)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(c, entry, exit);
    return BlackBox(entry, exit);
}

BlackBox buildUnion(NFA& nfa, BlackBox& BB1, BlackBox& BB2)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(fa::kEpsilon, entry, BB1.entry);
    nfa.addTransition(fa::kEpsilon, entry, BB2.entry);
    nfa.addTransition(fa::kEpsilon, BB1.exit, exit);
    nfa.addTransition(fa::kEpsilon, BB2.exit, exit);
    return BlackBox(entry, exit);
}

BlackBox buildConcatenation(NFA& nfa, BlackBox& BB1, BlackBox& BB2)
{
    auto entry = BB1.entry;
    auto exit = BB2.exit;
    nfa.addTransition(fa::kEpsilon, BB1.exit, BB2.entry);
    return BlackBox(entry, exit);
}

BlackBox buildClosure(NFA& nfa, BlackBox& BB)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(fa::kEpsilon, entry, BB.entry);
    nfa.addTransition(fa::kEpsilon, entry, exit);
    nfa.addTransition(fa::kEpsilon, BB.exit, BB.entry);
    nfa.addTransition(fa::kEpsilon, BB.exit, exit);
    return BlackBox(entry, exit);
}

} //namespace regex