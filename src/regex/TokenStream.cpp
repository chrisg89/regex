
#include "TokenStream.hpp"
#include <sstream>
#include <stdexcept>

namespace regex
{

TokenStream::TokenStream()
    : mTokens{}
{}

TokenStream::TokenStream(std::string regex)
    : mTokens{}
{
    std::stringstream ss;
    ss << regex;

    while( ss.peek() != EOF )
    {
        char current = ss.get();
        
        if(current == '(')
        {
            mTokens.emplace_back(TokenType::eOpenBracket, '(');
        }
        else if (current == ')')
        {
            mTokens.emplace_back(TokenType::eCloseBracket, ')');
        }
        else if (current == '*')
        {
            mTokens.emplace_back(TokenType::eClosure, '*');
        }
        else if (current == '+')
        {
            mTokens.emplace_back(TokenType::eClosurePlus, '+');
        }
        else if (current == '?')
        {
            mTokens.emplace_back(TokenType::eOptional, '?');
        }
        else if (current == '|')
        {
            mTokens.emplace_back(TokenType::eUnion, '|');
        }
        else if (current == '.')
        {
            mTokens.emplace_back(TokenType::eAny, '.');
        }
        else if(current == '\\' )
        {
            //escape sequence detected
            char next = ss.get();
            if(next == '(' || next == ')' || next == '*' || next == '+' || next == '?' || next == '|' || next == '\\' || next == '.' )
            {
                mTokens.emplace_back(TokenType::eSymbol, next);
            }
            else if(next == 'd')
            {
                mTokens.emplace_back(TokenType::eDigit, 'd');
            }
            else if(next == 'D')
            {
                mTokens.emplace_back(TokenType::eNonDigit, 'D');
            }
            else if(next == 's')
            {
                mTokens.emplace_back(TokenType::eWhitespace, 's');
            }
            else if(next == 'S')
            {
                mTokens.emplace_back(TokenType::eNonWhitespace, 'S');
            }
            else if(next == 'w')
            {
                mTokens.emplace_back(TokenType::eWordCharacter, 'w');
            }
            else if(next == 'W')
            {
                mTokens.emplace_back(TokenType::eNonWordCharacter, 'W');
            }
            else if(next == 'n')
            {
                mTokens.emplace_back(TokenType::eNewLine, '\n');
            }
            else if(next == 'f')
            {
                mTokens.emplace_back(TokenType::eFormFeed, '\f');
            }
            else if(next == 'r')
            {
                mTokens.emplace_back(TokenType::eCarriageReturn, '\r');
            }
            else if(next == 't')
            {
                mTokens.emplace_back(TokenType::eHorizonalTab, '\t');
            }
            else if(next == 'v')
            {
                mTokens.emplace_back(TokenType::eVerticalTab, '\v');
            }
            else
            {
                throw std::runtime_error("invalid escape sequence detected");
            }
        }
        else
        {
            mTokens.emplace_back(TokenType::eSymbol, current);
        }
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