
#pragma once

#include <string>

#include <deque>


namespace regex
{

constexpr uint8_t kPrioUnion = 0;
constexpr uint8_t kPrioConcat = 1;
constexpr uint8_t kPrioClosure = 2;

enum class TokenType{ 
    eUnion = kPrioUnion,
    eConcat = kPrioConcat,
    eClosure = kPrioClosure,
    eOpenBracket,
    eCloseBracket,
    eSymbol,
    eEOF
};

using Token = std::pair<TokenType, char>;

class TokenStream
{

public:

    TokenStream();
    Token peek();
    Token get();
    void insert(std::string regex); //TODO absorb this into constructor?
    void insert(Token token);
    std::string toString();

private:

    std::deque<Token> mTokens;
};


}