
#pragma once

#include "CodePoint.hpp"

#include <string>

#include <deque>


namespace regex
{

constexpr uint8_t kPrioUnion = 0;
constexpr uint8_t kPrioConcat = 1;
constexpr uint8_t kPrioClosure = 2;
constexpr uint8_t kPrioClosurePlus = 3;
constexpr uint8_t kPrioOptional = 4;

enum class TokenType{ 
    eUnion = kPrioUnion,
    eConcat = kPrioConcat,
    eClosure = kPrioClosure,
    eClosurePlus = kPrioClosurePlus,
    eOptional = kPrioOptional,
    eOpenBracket,
    eCloseBracket,
    eSymbol,
    eEOF
};

using Token = std::pair<TokenType, CodePointInterval>;

class TokenStream
{

public:

    TokenStream();
    TokenStream(std::string regex);
    Token peek();
    Token get();
    void insert(Token token);
    std::string toString();

private:

    std::deque<Token> mTokens;
};


}