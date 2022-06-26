
#pragma once

#include <string>

#include <deque>


namespace regex
{

enum class TokenType{ 
    eControl, 
    eSymbol,
    eNull
};

using Token = std::pair<TokenType, char>;

class TokenStream
{

public:

    TokenStream();
    Token peek();
    Token get();
    void insert(std::string regex);

private:

    std::deque<Token> mTokens;
};


}