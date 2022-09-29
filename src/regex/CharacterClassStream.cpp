
#include "CharacterClassStream.hpp"


#include <stdexcept>
#include <algorithm>

namespace regex
{

CodePoint ParseCodePointFixedLength(Utf8Iterator& it, const std::string& regex) // TODO const std::string& regex should be const Utf8Iterator& 
{
    CodePoint codepoint = 0;
    int count = 0;

    while(true)
    {
        if(it == regex.end() )
        {
            throw std::runtime_error("The token is incomplete");
        }

        if(!isxdigit(*it) )
        {
            throw std::runtime_error("The token is incomplete");
        }

        char value[2];
        value[0] = *it;
        value[1] = '\0';
        codepoint = codepoint << 4;
        codepoint |= strtol(value, NULL, 16);

        count++;
        if(count == 6)
        {
            break;
        }

        *it++;

    }

    if(codepoint < 0x1 || codepoint > 0x10FFFF)
    {
        throw std::runtime_error("Codepoint must be 0x000001-0x10FFFF (inclusive)");
    }
    return codepoint;
}

CodePoint ParseCodePointVariableLength(Utf8Iterator& it, const std::string& regex)
{
    CodePoint codepoint = 0;
    int count = 0;

    if(!isxdigit(*it) )
    {
        throw std::runtime_error("The token is incomplete");
    }

    while(true)
    {
        char value[2];
        value[0] = *it;
        value[1] = '\0';
        codepoint = codepoint << 4;
        codepoint |= strtol(value, NULL, 16);

        count++;
        *it++;

        if(it == regex.end() )
        {
            throw std::runtime_error("The token is incomplete");
        }

        if(count > 6 )
        {
            throw std::runtime_error("Codepoint must not exceed 6 digits");
        }

        if(*it == '}')
        {
            break;
        }
    }

    if(codepoint < 0x1 || codepoint > 0x10FFFF)
    {
        throw std::runtime_error("Codepoint must be 0x000001-0x10FFFF (inclusive)");
    }
    return codepoint;
}

CodePoint ParseCodePoint(Utf8Iterator& it, const std::string& regex)
{
    it++;

    if (it == regex.end())
    {
        throw std::runtime_error("The token is incomplete");
    }

    if(*it == '{')
    {
        it++;
        return ParseCodePointVariableLength(it, regex);
    }
    else
    {
        return ParseCodePointFixedLength(it, regex);
    }

}

CodePoint EscapeSequence(Utf8Iterator& it, const std::string& regex)
{
    it++;

    if (it == regex.end())
    {
        throw std::runtime_error("invalid escape sequence detected");
    }

    if(*it == 'u')       // Matches a specific Unicode code point
    {
        return ParseCodePoint(it, regex);
    }
    else if(*it == 'n')  // Character escape for line feed character
    {
        return('\n');
    }
    else if(*it == 'f')  // Character escape “form feed” control character (ASCII 0x0C)
    {
        return('\f');
    }
    else if(*it == 'r')  // Character escape for carriage return character
    {
        return('\r');
    }
    else if(*it == 't')  // Character escape for tab character
    {
        return('\t');
    }
    else if(*it == 'v')  // Character escape for “vertical tab” control character (ASCII 0x0B)
    {
        return('\v');
    }
    else if(*it == 'a')  // Character escape for “alert” or “bell” control character (ASCII 0x07)
    {
        return('\a');
    }
    else if(*it == 'b')  // Character escape for "backspace" control character (ASCII 0x08)
    {
        return('\b');
    }
    else if(*it == 'e')  // Character escape for "escape" control character (ASCII 0x1B)
    {
        return('\x1B');
    }
    else if(*it == '\\') // escape meta-character '\'
    {
        return('\\');
    }
    else if(*it == '^')  // escape meta-character '^'
    {
        return('^');
    }
    else if(*it == '[')  // escape meta-character '['
    {
        return('[');
    }
    else if(*it == ']')  // escape meta-character ']'
    {
        return(']');
    }
    else if(*it == '-')  // escape meta-character '-'
    {
        return('-');
    }
    else
    {
        throw std::runtime_error("invalid escape sequence detected");
    }
}


bool CharacterClassStream::parseShorthand(Utf8Iterator& it, const std::string& regex)
{
    if(*it == '\\')
    {
        auto next = std::next(it);

        if(it == regex.end())
        {
            throw std::runtime_error("invalid escape sequence detected");
        }

        if(*next == 'd')
        {
            // [0-9]
            it++;
            mTokens.emplace_back(CCTokenType::eInterval, 0x30, 0x39);
            return true;
        }
        else if(*next == 'D')
        {
            //[^0-9]
            it++;
            mTokens.emplace_back(CCTokenType::eInterval, kCodePointMin, 0x2F);
            mTokens.emplace_back(CCTokenType::eInterval, 0x3A, kCodePointMax);
            return true;
        }
        else if(*next == 's')
        {
            //[\r\n\t\f\v ]
            it++;
            mTokens.emplace_back(CCTokenType::eInterval, 0x09, 0x0D);
            mTokens.emplace_back(CCTokenType::eInterval, 0x20, 0x20);
            return true;
        }
        else if(*next == 'S')
        {
            //[^\r\n\t\f\v ]
            it++;
            mTokens.emplace_back(CCTokenType::eInterval, kCodePointMin, 0x08);
            mTokens.emplace_back(CCTokenType::eInterval, 0x0E, 0x1F);
            mTokens.emplace_back(CCTokenType::eInterval, 0x21, kCodePointMax); 
            return true;
        }
        else if(*next == 'w')
        {
            //[a-zA-Z0-9_]
            it++;
            mTokens.emplace_back(CCTokenType::eInterval, 0x30, 0x39);
            mTokens.emplace_back(CCTokenType::eInterval, 0x41, 0x5A);
            mTokens.emplace_back(CCTokenType::eInterval, 0x5F, 0x5F);
            mTokens.emplace_back(CCTokenType::eInterval, 0x61, 0x7A);
            return true;
        }
        else if(*next == 'W')
        {
            //[^a-zA-Z0-9_]
            it++;
            mTokens.emplace_back(CCTokenType::eInterval, kCodePointMin, 0x2F);
            mTokens.emplace_back(CCTokenType::eInterval, 0x3A, 0x40);
            mTokens.emplace_back(CCTokenType::eInterval, 0x5B, 0x5E);
            mTokens.emplace_back(CCTokenType::eInterval, 0x60, 0x60);
            mTokens.emplace_back(CCTokenType::eInterval, 0x7B, kCodePointMax);
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

CharacterClassStream::CharacterClassStream(Utf8Iterator& it, const std::string& regex)
    : mTokens{}
{
    parse(it, regex);

    if(mIsNegated)
    {
        negate();
    }
}

void CharacterClassStream::parse(Utf8Iterator& it, const std::string& regex)
{
    bool isFirst = true;

    while(true)
    {
        *it++;

        if(it == regex.end())
        {
            throw std::runtime_error("character class missing closing bracket");
        }

        if(*it == ']')
        {
            break;
        }

        if(*it == '^' && isFirst == true)
        {
            mIsNegated = true;
            continue;
        }

        if(parseShorthand(it, regex))
        {
            continue;
        }

        CodePoint start = (*it == '\\' ? EscapeSequence(it, regex) : *it);
        auto next = std::next(it);
        if(next != regex.end())
        {
            if(*next != '-')
            {
                mTokens.emplace_back(CCTokenType::eInterval, start, start);
            }
            else
            {
                it++;
                next = std::next(it);
                if(next != regex.end())
                {
                    if(*next == ']')
                    {
                        mTokens.emplace_back(CCTokenType::eInterval, start, start);
                        mTokens.emplace_back(CCTokenType::eInterval, '-', '-');
                    }
                    else
                    {
                        it++;
                        CodePoint end = (*it == '\\' ? EscapeSequence(it, regex) : *it);

                        if(end < start)
                        {
                            throw std::runtime_error("Character range is out of order");
                        }

                        mTokens.emplace_back(CCTokenType::eInterval, start, end);
                    }
                }
            }
        }
    

        isFirst = false;
    }
}

void CharacterClassStream::negate()
{
    std::sort(mTokens.begin(), mTokens.end(), [](const CCToken& a, const CCToken& b) { 
        return a.start < b.end;
    });

    std::deque<CCToken> swapped;
    std::swap(swapped, mTokens);
    CCToken front;

    // this needs to be large enough to prevent overflow errors
    unsigned long long int current = kCodePointMin;

    while( !swapped.empty() )
    {
        front = swapped.front();
        swapped.pop_front();

        if(front.start != kCodePointMin)
        {
            mTokens.emplace_back(CCTokenType::eInterval, current, front.start-1);
            current = front.end + 1;
        }
    }

    if(front.start< kCodePointMax)
    { 
        mTokens.emplace_back(CCTokenType::eInterval, current, kCodePointMax);
    }
}

CCToken CharacterClassStream::peek()
{
    if (mTokens.empty())
        return CCToken{CCTokenType::eEOF, 0, 0};

    return mTokens.front();
}

CCToken CharacterClassStream::get()
{
    if (mTokens.empty())
        return CCToken{CCTokenType::eEOF, 0, 0};

    auto token = mTokens.front();
    mTokens.pop_front();
    return token;
}

} //namespace regex