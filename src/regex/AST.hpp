#pragma once

#include "Alphabet.hpp"
#include "CodePoint.hpp"

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

//TODO Replace reset()

namespace regex::ast
{

class Node;
using NodePtr = std::unique_ptr<Node>;

class Node
{
public:
    virtual void eval() = 0;
    virtual void print(std::string&) = 0;
    virtual void makeAlphabet(Alphabet&) = 0; 
};

class Alternative : public Node
{
public:
    Alternative(NodePtr& lhs, NodePtr& rhs)
    : mLeft {lhs.release()}
    , mRight {rhs.release()}
    {}

    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        mLeft->makeAlphabet(alphabet);
        mRight->makeAlphabet(alphabet);
    }

    void print(std::string& str) final
    {
        str+= "(";
        mLeft->print(str);
        str+= "|";
        mRight->print(str);
        str+= ")";
    }
     
    NodePtr mLeft;
    NodePtr mRight;
};

class Concatenation : public Node
{
public:
    Concatenation(NodePtr& lhs, NodePtr& rhs)
    : mLeft {lhs.release()}
    , mRight {rhs.release()}
    {}

    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        mLeft->makeAlphabet(alphabet);
        mRight->makeAlphabet(alphabet);
    }

    void print(std::string& str) final
    {
        str+= "(";
        mLeft->print(str);
        mRight->print(str);
        str+= ")";
    }
     
    NodePtr mLeft;
    NodePtr mRight;
};

class Quantifier : public Node
{
public:
    Quantifier(NodePtr& mInner, uint64_t min, uint64_t max, bool isMaxBounded)
    : mInner {mInner.release()}
    , mMin {min}
    , mMax {max}
    , mIsMaxBounded {isMaxBounded}
    {}

    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        mInner->makeAlphabet(alphabet);
    }

    void print(std::string& str) final
    {
        mInner->print(str);
        if(mIsMaxBounded)
        {
            str += "{" + std::to_string(mMin) + "," + std::to_string(mMax) + "}";
        }
        else
        {
            str += "{" + std::to_string(mMin) + "," + "}";
        }
    }

    uint64_t mMin;
    uint64_t mMax;
    bool mIsMaxBounded;     
    NodePtr mInner;
};

class Any : public Node
{
    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(kCodePointMin, kCodePointMax);
    }

    void print(std::string& str) final
    {
        str+= ".";
    }
};

class Epsilon : public Node
{
    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        /* Do nothing */
    }

    void print(std::string& str) final
    {
        str+= "";
    }
};

class Character : public Node
{
public:
    Character(CodePoint character)
    : mCodePoint{character}
    {}

    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(mCodePoint,mCodePoint);
    }

    void print(std::string& str) final
    {
        std::stringstream ss;
        ss << std::hex << std::setfill ('0') << std::setw(6) << mCodePoint;
        str+= "\\u";
        str+= ss.str();
    }

    CodePoint mCodePoint;
};

class CharacterClass : public Node
{
public:
    CharacterClass(std::vector<NodePtr>& codePointIntervals, bool isNegated)
    : mCodePointIntervals{std::move(codePointIntervals)}  //TODO rename mCodePointIntervals to CharacterClassItem
    , mIsNegated{isNegated}
    {}

    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        Alphabet temp;

        for (const auto& item : mCodePointIntervals )
        {
            item->makeAlphabet(temp);
        }

        if(mIsNegated)
        {
            negate(temp);
        }

        alphabet.insert(std::end(alphabet), std::begin(temp), std::end(temp));
    }



    void print(std::string& str) final
    {
        str+= "[";

        if(mIsNegated)
        {
            str+= "^";
        }

        for (const auto& item : mCodePointIntervals )
        {
            item->print(str);
        }
        str+= "]";
    }

    std::vector<NodePtr> mCodePointIntervals;
    bool mIsNegated;
};

class CharacterRange : public Node
{
public:
    CharacterRange(NodePtr& start, NodePtr& end)
    : mStart{std::move(start)}
    , mEnd{std::move(end)}
    {}

    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        //TODO dynamic_cast is typically considered a code smell. Need to refactor?
        auto start = dynamic_cast<Character&>(*mStart).mCodePoint;
        auto end = dynamic_cast<Character&>(*mEnd).mCodePoint;
        alphabet.emplace_back(start, end);
    }

    void print(std::string& str) final
    {
        mStart->print(str);
        str+= "-";
        mEnd->print(str);
    }

    NodePtr mStart;
    NodePtr mEnd;
};

class CharacterClassAnyWord : public Node
{
    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(0x30, 0x39);
        alphabet.emplace_back(0x41, 0x5A);
        alphabet.emplace_back(0x5F, 0x5F);
        alphabet.emplace_back(0x61, 0x7A);
    }

    void print(std::string& str) final
    {
        str+= "\\w";
    }
};

class CharacterClassAnyWordNegated : public Node
{
    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(kCodePointMin, 0x2F);
        alphabet.emplace_back(0x3A, 0x40);
        alphabet.emplace_back(0x5B, 0x5E);
        alphabet.emplace_back(0x60, 0x60);
        alphabet.emplace_back(0x7B, kCodePointMax);
    }

    void print(std::string& str) final
    {
        str+= "\\W";
    }
};

class CharacterClassAnyDigit : public Node
{
    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(0x30, 0x39);
    }

    void print(std::string& str) final
    {
        str+= "\\d";
    }
};

class CharacterClassAnyDigitNegated : public Node
{
    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(kCodePointMin, 0x2F);
        alphabet.emplace_back(0x3A, kCodePointMax);
    }

    void print(std::string& str) final
    {
        str+= "\\D";
    }
};

class CharacterClassAnyWhitespace : public Node
{
    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(0x09, 0x0D);
        alphabet.emplace_back(0x20, 0x20);
    }

    void print(std::string& str) final
    {
        str+= "\\s";
    }
};

class CharacterClassAnyWhitespaceNegated : public Node
{
    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(kCodePointMin, 0x08);
        alphabet.emplace_back(0x0E, 0x1F);
        alphabet.emplace_back(0x21, kCodePointMax);
    }

    void print(std::string& str) final
    {
        str+= "\\S";
    }
};

class AST
{
public:

    AST(NodePtr& node ) : mRoot{std::move(node)}
    {}

    std::string print()
    {
        std::string str;
        mRoot->print(str);
        return str;
    }

    Alphabet makeAlphabet()
    {
        Alphabet alphabet;
        mRoot->makeAlphabet(alphabet);
        DisjoinOverlap(alphabet, kCodePointMin, kCodePointMax);
        return alphabet;
    }

private:
    NodePtr mRoot;
};

} // namespace regex::ast
