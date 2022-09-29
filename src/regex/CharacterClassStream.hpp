#pragma once

#include "CodePoint.hpp"
#include "Utf8Iterator.hpp"

#include <string>
#include <queue>

namespace regex
{

enum class CCTokenType{ 
    eInterval,
    eEOF
};

struct CCToken
{
    CCToken()
    : type {CCTokenType::eInterval}
    , start {0}
    , end {0}
    {}

    CCToken(CCTokenType type, CodePoint startEnd)
    : type {type}
    , start {startEnd}
    , end {startEnd}
    {}

    CCToken(CCTokenType type, CodePoint start, CodePoint end)
    : type {type}
    , start {start}
    , end {end}
    {}

    CCTokenType type;
    CodePoint start;
    CodePoint end;
};

class CharacterClassStream
{
public:
    CharacterClassStream(Utf8Iterator& it, const std::string& regex);
    CCToken peek();
    CCToken get();

private:
    std::deque<CCToken> mTokens;
    bool mIsNegated = false;

    void parse(Utf8Iterator& it, const std::string& regex);
    bool parseShorthand(Utf8Iterator& it, const std::string& regex);
    void negate();
};

} //namespace regex