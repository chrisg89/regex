#include "ThompsonConstruction.hpp"

#include <cassert>
#include <stack>
#include <numeric>

namespace regex
{

using NfaAlphabet = fa::Alphabet;


NFA ThompsonConstruction(TokenStream regex, Alphabet& alphabet)
{
    std::stack<BlackBox> stack;

    auto nfaAlphabet = NfaAlphabet(alphabet.size());
    std::iota(std::begin(nfaAlphabet), std::end(nfaAlphabet), 0);
    auto nfa = NFA(nfaAlphabet);

    auto token = Token{};
    while( token = regex.get(), token.first != TokenType::eEOF )
    {
        if(token.first == TokenType::eUnion)
        {
            auto op1 = stack.top();
            stack.pop();
            auto op2 = stack.top();
            stack.pop();
            stack.push(buildUnion(nfa, op2, op1));
        }
        else if(token.first == TokenType::eConcat)
        {   
            auto op1 = stack.top();
            stack.pop();
            auto op2 = stack.top();
            stack.pop();
            stack.push(buildConcatenation(nfa, op2, op1));
        }
        else if(token.first == TokenType::eClosure)
        {
            auto op1 = stack.top();
            stack.pop();
            stack.push(buildClosure(nfa, op1));
        }
        else if(token.first == TokenType::eClosurePlus)
        {
            auto op1 = stack.top();
            stack.pop();
            stack.push(buildClosurePlus(nfa, op1));
        }
        else if(token.first == TokenType::eOptional)
        {
            auto op1 = stack.top();
            stack.pop();
            stack.push(buildOptional(nfa, op1));
        }
        else
        {
            stack.push(buildSymbol(nfa, alphabet, token.second));
        }

    }
    assert(stack.size()==1);

    // stack now contains the black box containing within it
    // the FSA equivalent of the regex. Only need to connect
    // the entry and exit states of the black box to the start
    // and end states.
    auto bb = stack.top();
    auto start = nfa.addState(true, false);
    auto end = nfa.addState(false, true);
    nfa.addTransition(fa::kEpsilon, start, bb.entry);
    nfa.addTransition(fa::kEpsilon, bb.exit, end);

    return nfa;
}

BlackBox::BlackBox(StateId entry, StateId exit)
: entry {entry}
, exit {exit}
{}

BlackBox buildSymbol(NFA& nfa, Alphabet& alphabet, CodePointInterval& interval)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);

    int index = 0;
    for (auto& c : alphabet)
    {
        if(isSubset(interval, c))
        {
            nfa.addTransition(index, entry, exit);
        }
        index++;
    }

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

BlackBox buildClosurePlus(NFA& nfa, BlackBox& BB)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(fa::kEpsilon, entry, BB.entry);
    nfa.addTransition(fa::kEpsilon, BB.exit, BB.entry);
    nfa.addTransition(fa::kEpsilon, BB.exit, exit);
    return BlackBox(entry, exit);
}

BlackBox buildOptional(NFA& nfa, BlackBox& BB)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(fa::kEpsilon, entry, BB.entry);
    nfa.addTransition(fa::kEpsilon, entry, exit);
    nfa.addTransition(fa::kEpsilon, BB.exit, exit);
    return BlackBox(entry, exit);
}

} //namespace regex