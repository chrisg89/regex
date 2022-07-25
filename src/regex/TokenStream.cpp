
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
            if(next == '(' || next == ')' || next == '*' || next == '|' || next == '\\' || next == '.' )
            {
                mTokens.emplace_back(TokenType::eSymbol, next);
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