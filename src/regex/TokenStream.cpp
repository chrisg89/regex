
#include "TokenStream.hpp"
#include "Utf8Iterator.hpp"
#include <sstream>
#include <stdexcept>

namespace regex
{

TokenStream::TokenStream()
    : mTokens{}
{}

TokenStream::TokenStream(const std::string regex) //TODO this should be a reference
    : mTokens{}
{
    Utf8Iterator current = regex.begin();

    while( current != regex.end() )
    { 
        if(*current == '(')
        {
            mTokens.emplace_back(TokenType::eOpenBracket, '(');
        }
        else if (*current == ')')
        {
            mTokens.emplace_back(TokenType::eCloseBracket, ')');
        }
        else if (*current == '*')
        {
            mTokens.emplace_back(TokenType::eClosure, '*');
        }
        else if (*current == '+')
        {
            mTokens.emplace_back(TokenType::eClosurePlus, '+');
        }
        else if (*current == '?')
        {
            mTokens.emplace_back(TokenType::eOptional, '?');
        }
        else if (*current == '|')
        {
            mTokens.emplace_back(TokenType::eUnion, '|');
        }
        else if (*current == '.')
        {
            mTokens.emplace_back(TokenType::eAny, '.');
        }
        else if(*current == '\\' )
        {
            //escape sequence detected
            current++;
            if(*current == '(' || *current == ')' || *current == '*' || *current == '+' || *current == '?' || *current == '|' || *current == '\\' || *current == '.' )
            {
                mTokens.emplace_back(TokenType::eSymbol, *current);
            }
            else if(*current == 'd')
            {
                mTokens.emplace_back(TokenType::eDigit, 'd');
            }
            else if(*current == 'D')
            {
                mTokens.emplace_back(TokenType::eNonDigit, 'D');
            }
            else if(*current == 's')
            {
                mTokens.emplace_back(TokenType::eWhitespace, 's');
            }
            else if(*current == 'S')
            {
                mTokens.emplace_back(TokenType::eNonWhitespace, 'S');
            }
            else if(*current == 'w')
            {
                mTokens.emplace_back(TokenType::eWordCharacter, 'w');
            }
            else if(*current == 'W')
            {
                mTokens.emplace_back(TokenType::eNonWordCharacter, 'W');
            }
            else if(*current == 'n')
            {
                mTokens.emplace_back(TokenType::eSymbol, '\n');
            }
            else if(*current == 'f')
            {
                mTokens.emplace_back(TokenType::eSymbol, '\f');
            }
            else if(*current == 'r')
            {
                mTokens.emplace_back(TokenType::eSymbol, '\r');
            }
            else if(*current == 't')
            {
                mTokens.emplace_back(TokenType::eSymbol, '\t');
            }
            else if(*current == 'v')
            {
                mTokens.emplace_back(TokenType::eSymbol, '\v');
            }
            else
            {
                throw std::runtime_error("invalid escape sequence detected");
            }
        }
        else
        {
            mTokens.emplace_back(TokenType::eSymbol, *current);
        }
        current++;
    }
}

void TokenStream::insert(Token token)
{
    mTokens.emplace_back(token);
}

std::string TokenStream::toString()
{
    std::string string = "";
    for (auto const &token: mTokens) {  //TODO: does not print escape character
        string+=token.second;
    }
    return string;
}

Token TokenStream::peek()
{
    if (mTokens.empty())
        return Token{TokenType::eEOF, ' '};

    return mTokens.front();
}


Token TokenStream::get()
{
    if (mTokens.empty())
        return Token{TokenType::eEOF, ' '};

    auto token = mTokens.front();
    mTokens.pop_front();
    return token;
}

}