
#include "TokenStream.hpp"
#include "Utf8Iterator.hpp"
#include <sstream>
#include <stdexcept>

namespace regex
{

void insertWhitespace(std::deque<Token>& Tokens )
{
    Tokens.emplace_back(TokenType::eOpenBracket, CodePointInterval('(', '('));
    // horizontal tab (TAB), newline (LF), vertical tab (VT), feed (FF), carriage return (CR)
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x09, 0x0D));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));
    // space (SPC)
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x20, 0x20));
    Tokens.emplace_back(TokenType::eCloseBracket, CodePointInterval(')', ')'));
}

void insertNonWhitespace(std::deque<Token>& Tokens )
{
    Tokens.emplace_back(TokenType::eOpenBracket, CodePointInterval('(', '('));
    // newline (LF), vertical tab (VT), feed (FF), carriage return (CR)
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x00, 0x08));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));
    // space (SPC)
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x0E, 0x1F));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));
    // horizontal tab (TAB)
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x21, 0xFF));
    Tokens.emplace_back(TokenType::eCloseBracket, CodePointInterval(')', ')'));
}

void insertAny(std::deque<Token>& Tokens )
{
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x00, 0xFF));
}

void insertDigit(std::deque<Token>& Tokens )
{
    // 0-9
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x30, 0x39));
}

void insertNonDigit(std::deque<Token>& Tokens )
{
    Tokens.emplace_back(TokenType::eOpenBracket, CodePointInterval('(', '('));
    // horizontal tab (TAB), newline (LF), vertical tab (VT), feed (FF), carriage return (CR)
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x00, 0x2F));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));
    // space (SPC)
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x3A, 0xFF));
    Tokens.emplace_back(TokenType::eCloseBracket, CodePointInterval(')', ')'));
}

void insertWordCharacter(std::deque<Token>& Tokens )
{
    Tokens.emplace_back(TokenType::eOpenBracket, CodePointInterval('(', '('));

    // 0-9
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x30, 0x39));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));

    // A-Z
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x41, 0x5A));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));

    // _
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x5F, 0x5F));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));

    // a-z
    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x61, 0x7A));

    Tokens.emplace_back(TokenType::eCloseBracket, CodePointInterval(')', ')'));
}

void insertNonWordCharacter(std::deque<Token>& Tokens )
{
    Tokens.emplace_back(TokenType::eOpenBracket, CodePointInterval('(', '('));

    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x00, 0x2F));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));

    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x3A, 0x40));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));

    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x5B, 0x5E));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));

    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x60, 0x60));
    Tokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));

    Tokens.emplace_back(TokenType::eSymbol, CodePointInterval(0x7B, 0xFF));

    Tokens.emplace_back(TokenType::eCloseBracket, CodePointInterval(')', ')'));
}



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
            mTokens.emplace_back(TokenType::eOpenBracket, CodePointInterval('(', '('));
        }
        else if (*current == ')')
        {
            mTokens.emplace_back(TokenType::eCloseBracket, CodePointInterval(')', ')'));
        }
        else if (*current == '*')
        {
            mTokens.emplace_back(TokenType::eClosure, CodePointInterval('*', '*'));
        }
        else if (*current == '+')
        {
            mTokens.emplace_back(TokenType::eClosurePlus, CodePointInterval('+', '+'));
        }
        else if (*current == '?')
        {
            mTokens.emplace_back(TokenType::eOptional, CodePointInterval('?', '?'));
        }
        else if (*current == '|')
        {
            mTokens.emplace_back(TokenType::eUnion, CodePointInterval('|', '|'));
        }
        else if (*current == '.')
        {
            insertAny(mTokens);
        }
        else if(*current == '\\' )
        {
            //escape sequence detected
            current++;
            if(*current == '(' || *current == ')' || *current == '*' || *current == '+' || *current == '?' || *current == '|' || *current == '\\' || *current == '.' )
            {
                mTokens.emplace_back(TokenType::eSymbol, CodePointInterval(*current, *current));
            }
            else if(*current == 'd')
            {
                insertDigit(mTokens);
            }
            else if(*current == 'D')
            {
                insertNonDigit(mTokens);
            }
            else if(*current == 's')
            {
                insertWhitespace(mTokens);
            }
            else if(*current == 'S')
            {
                insertNonWhitespace(mTokens);
            }
            else if(*current == 'w')
            {
                insertWordCharacter(mTokens);
            }
            else if(*current == 'W')
            {
                insertNonWordCharacter(mTokens);
            }
            else if(*current == 'n')
            {
                mTokens.emplace_back(TokenType::eSymbol, CodePointInterval('\n','\n'));
            }
            else if(*current == 'f')
            {
                mTokens.emplace_back(TokenType::eSymbol, CodePointInterval('\f','\f'));
            }
            else if(*current == 'r')
            {
                mTokens.emplace_back(TokenType::eSymbol, CodePointInterval('\r','\r'));
            }
            else if(*current == 't')
            {
                mTokens.emplace_back(TokenType::eSymbol, CodePointInterval('\t','\t'));
            }
            else if(*current == 'v')
            {
                mTokens.emplace_back(TokenType::eSymbol, CodePointInterval('\v','\v'));
            }
            else
            {
                throw std::runtime_error("invalid escape sequence detected");
            }
        }
        else
        {
            mTokens.emplace_back(TokenType::eSymbol, CodePointInterval(*current,*current));
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
        string+=token.second.first; //TODO Add token to string??
    }
    return string;
}

Token TokenStream::peek()
{
    if (mTokens.empty())
        return Token{TokenType::eEOF, {' ', ' '}};

    return mTokens.front();
}


Token TokenStream::get()
{
    if (mTokens.empty())
        return Token{TokenType::eEOF, {' ', ' '}};

    auto token = mTokens.front();
    mTokens.pop_front();
    return token;
}

}