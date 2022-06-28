
#pragma once

#include <string>

#include <deque>


namespace regex
{

enum class TokenType{ 
    eControl, 
    eSymbol,
    eNull // TODO: change to eEof?
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