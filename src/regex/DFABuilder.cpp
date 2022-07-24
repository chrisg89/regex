#include "DFABuilder.hpp"
#include "ThompsonConstruction.hpp"

#include <cassert> //todo try to replace with excpetion everywhere
#include <stack>

//TODO: need to create unit test file and move tests into it

namespace regex
{
    
DFA build(TokenStream regex, Alphabet alphabet)  //todo use alpha by ref
{
    auto preprocessed = InsertExplicitConcat(regex);
    auto postfix = InfixToPostfix(preprocessed);

    // Construct the NFA from the regular expression
    auto nfa = ThompsonConstruction(postfix, alphabet);

    // Construct the DFA from the NFA
    auto dfa = nfa.toDFA();

    return dfa;
}

TokenStream InsertExplicitConcat(TokenStream regex)  //TODO rename to insert???
{

    TokenStream processed;
    bool insert;

    while( true )
    {
        auto current = regex.get();
        auto next = regex.peek();
        insert = false;

        if(next.first == TokenType::eEOF)
        {
            processed.insert(current);
            break;
        }

        if(current.first == TokenType::eClosure)
        {
            if(next.first == TokenType::eClosure){
                insert = false;
            }
            else if(next.first == TokenType::eUnion){
                insert = false;
            }
            else if(next.first == TokenType::eOpenBracket){
                insert = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                insert = false;
            }
            else{
                insert = true;
            }
        }
        else if (current.first == TokenType::eUnion)
        {
            insert = false;
        }
        else if (current.first == TokenType::eOpenBracket)
        {
            insert = false;
        }
        else if (current.first == TokenType::eCloseBracket)
        {
            if(next.first == TokenType::eClosure){
                insert = false;
            }
            else if(next.first == TokenType::eUnion){
                insert = false;
            }
            else if(next.first == TokenType::eOpenBracket){
                insert = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                insert = false;
            }
            else{
                insert = true;
            }
        }
        else
        {
            if(next.first == TokenType::eClosure){
                insert = false;
            }
            else if(next.first == TokenType::eUnion){
                insert = false;
            }
            else if(next.first == TokenType::eOpenBracket){
                insert = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                insert = false;
            }
            else{
                insert = true;
            }
        }

        processed.insert(current);
        if(insert)
        {
            processed.insert(Token{TokenType::eConcat, '&'});
        }

    }

    return processed;
}

TokenStream InfixToPostfix(TokenStream infix)
{
    TokenStream postfix;
    std::stack<Token> stack;

    auto token = Token{};
    while( token = infix.get(), token.first != TokenType::eEOF )
    {

        if(token.first == TokenType::eOpenBracket)
        {
            stack.push(token);
        }

        else if(token.first == TokenType::eCloseBracket)
        {
            while(stack.top().first != TokenType::eOpenBracket)
            {
                postfix.insert(stack.top());
                stack.pop();
            }
            stack.pop();
        }

        else if(token.first == TokenType::eUnion || 
                token.first == TokenType::eConcat || 
                token.first == TokenType::eClosure)
        {
            while(!stack.empty())
            {
                if (stack.top().first == TokenType::eOpenBracket)
                    break;
                if (stack.top().first < token.first)
                    break;

                postfix.insert(stack.top());
                stack.pop();
            }
            stack.push(token);
        }

        else
        {
            postfix.insert(token);
        }

    }
    while(!stack.empty())
    {
        postfix.insert(stack.top());
        stack.pop();
    }

    return postfix;
}




} //namespace regex