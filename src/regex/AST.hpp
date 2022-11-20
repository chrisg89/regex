#pragma once

#include "Alphabet.hpp"
#include "CodePoint.hpp"

#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

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
        ss << std::hex << std::setfill ('0') << std::setw(8) << mCodePoint;
        str+= "\\U";
        str+= ss.str();
    }

    CodePoint mCodePoint;
};

class CharacterRange : public Node
{
public:
    CharacterRange(CodePoint start, CodePoint end)
    : mStart{start}
    , mEnd{end}
    {}

    CharacterRange(CodePoint codepoint)  
    : mStart{codepoint}
    , mEnd{codepoint}
    {}

    void eval() final
    {}

    void makeAlphabet(Alphabet& alphabet) final
    {
        alphabet.emplace_back(mStart, mEnd);
    }

    void print(std::string& str) final
    {

        std::stringstream ss;
        ss << "[";
        ss << "\\U" << std::hex << std::setfill ('0') << std::setw(8) << mStart;
        ss << "-";
        ss << "\\U" << std::hex << std::setfill ('0') << std::setw(8) << mEnd;
        ss << "]";

        str+= ss.str();

    }

    CodePoint mStart;
    CodePoint mEnd;
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
