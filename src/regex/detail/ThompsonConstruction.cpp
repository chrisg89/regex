#include "ThompsonConstruction.hpp"

#include <cassert>
#include <stack>

namespace regex
{

NFA ThompsonConstruction(TokenStream regex, Alphabet& alphabet)
{
    std::stack<BlackBox> stack;

    NFA nfa{alphabet};

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
        else if(token.first == TokenType::eAny)
        {
            stack.push(buildAny(nfa, alphabet));
        }
        else if(token.first == TokenType::eDigit)
        {
            stack.push(buildDigit(nfa, alphabet));
        }
        else if(token.first == TokenType::eNonDigit)
        {
            stack.push(buildNonDigit(nfa, alphabet));
        }
        else if(token.first == TokenType::eWhitespace)
        {
            stack.push(buildWhitespace(nfa, alphabet));
        }
        else if(token.first == TokenType::eNonWhitespace)
        {
            stack.push(buildNonWhitespace(nfa, alphabet));
        }
        else if(token.first == TokenType::eWordCharacter)
        {
            stack.push(buildWordCharacter(nfa, alphabet));
        }
        else if(token.first == TokenType::eNonWordCharacter)
        {
            stack.push(buildNonWordCharacter(nfa, alphabet));
        }
        else
        {
            stack.push(buildSymbol(nfa, token.second));
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

BlackBox buildSymbol(NFA& nfa, char c)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);
    nfa.addTransition(c, entry, exit);
    return BlackBox(entry, exit);
}

BlackBox buildDigit(NFA& nfa, Alphabet& alphabet)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);

    for (auto c : alphabet) 
    {
        if(isdigit(c))
        {
            nfa.addTransition(c, entry, exit);
        }
    }

    return BlackBox(entry, exit);
}

BlackBox buildNonDigit(NFA& nfa, Alphabet& alphabet)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);

    for (auto c : alphabet) 
    {
        if(!isdigit(c))
        {
            nfa.addTransition(c, entry, exit);
        }
    }

    return BlackBox(entry, exit);
}

BlackBox buildWhitespace(NFA& nfa, Alphabet& alphabet)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);

    for (auto c : alphabet) 
    {
        if(isspace(c))
        {
            nfa.addTransition(c, entry, exit);
        }
    }

    return BlackBox(entry, exit);
}

BlackBox buildNonWhitespace(NFA& nfa, Alphabet& alphabet)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);

    for (auto c : alphabet) 
    {
        if(!isspace(c))
        {
            nfa.addTransition(c, entry, exit);
        }
    }

    return BlackBox(entry, exit);
}

BlackBox buildWordCharacter(NFA& nfa, Alphabet& alphabet)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);

    for (auto c : alphabet) 
    {
        if(isalpha(c) || isdigit(c) || c == '_')
        {
            nfa.addTransition(c, entry, exit);
        }
    }

    return BlackBox(entry, exit);
}

BlackBox buildNonWordCharacter(NFA& nfa, Alphabet& alphabet)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);

    for (auto c : alphabet) 
    {
        if(!isalpha(c) && !isdigit(c) && c != '_')
        {
            nfa.addTransition(c, entry, exit);
        }
    }

    return BlackBox(entry, exit);
}

BlackBox buildAny(NFA& nfa, Alphabet& alphabet)
{
    auto entry = nfa.addState(false, false);
    auto exit = nfa.addState(false, false);

    for (auto c : alphabet) 
    {
        //TODO: some regex engines dont include new line in any.
        nfa.addTransition(c, entry, exit);
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