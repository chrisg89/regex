
#include "TokenStream.hpp"
#include <sstream>
#include <cassert>


// TODO unit test this class

namespace regex
{

TokenStream::TokenStream()
    : mTokens{}
{}

void TokenStream::insert(Token token)
{
    mTokens.emplace_back(token);
}

void TokenStream::insert(std::string regex)
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
        else if (current == '|')
        {
            mTokens.emplace_back(TokenType::eUnion, '|');
        }
        else if(current == '\\' )
        {
            //escape sequence detected
            char next = ss.get();
            if(next == '(' || next == ')' || next == '*' || next == '|' || next == '\\')
            {
                mTokens.emplace_back(TokenType::eSymbol, next);
            }
            else
            {
                assert(false); //TODO invalid escape sequence throw exception
            }
        }
        else
        {
            mTokens.emplace_back(TokenType::eSymbol, current);
        }
    }
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