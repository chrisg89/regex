
#include "TokenStream.hpp"
#include <sstream>
#include <cassert>


// TODO unit test this class

namespace regex
{

TokenStream::TokenStream()
    : mTokens{}
{}


void TokenStream::insert(std::string regex)
{

    std::stringstream ss;
    ss << regex;

    while( ss.peek() != EOF )
    {
        char current = ss.get();
        
        if(current == '(' || current == ')' || current == '*' || current == '|')
        {
            mTokens.emplace_back(TokenType::eControl, current);
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

Token TokenStream::peek()
{
    if (mTokens.size() > 1)
        return mTokens[1];

    return Token{TokenType::eNull, ' '};
}


Token TokenStream::get()
{
    auto token = mTokens.front();
    mTokens.pop_front();
    return token;
}

}