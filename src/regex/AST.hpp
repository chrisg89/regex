#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>
#include "CodePoint.hpp"

namespace regex::ast
{

//TODO prefix private members with m

class Node;

using NodePtr = std::unique_ptr<Node>;


class Node
{
public:
    virtual void eval() = 0;
    virtual void print(std::string&) = 0;

private:


};

class Alternative : public Node
{
public:
    Alternative(NodePtr& lhs, NodePtr& rhs)
    : left {lhs.release()}
    , right {rhs.release()}
    {}

private:

    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "(";
        left->print(str);
        str+= "|";
        right->print(str);
        str+= ")";
    }
     
    NodePtr left;
    NodePtr right;
};

class Concatenation : public Node
{
public:
    Concatenation(NodePtr& lhs, NodePtr& rhs)
    : left {lhs.release()}
    , right {rhs.release()}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "(";
        left->print(str);
        right->print(str);
        str+= ")";
    }
     
    NodePtr left;
    NodePtr right;
};

class KleeneStar : public Node
{
public:
    KleeneStar(NodePtr& inner)
    : inner {inner.release()}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        inner->print(str);
        str+= "*";
    }
     
    NodePtr inner;
};

class KleenePlus : public Node
{
public:
    KleenePlus(NodePtr& inner)
    : inner {inner.release()}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        inner->print(str);
        str+= "+";
    }
     
    NodePtr inner;
};

class Optional : public Node
{
public:
    Optional(NodePtr& inner)
    : inner {inner.release()}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        inner->print(str);
        str+= "?";
    }
     
    NodePtr inner;
};

class RangeQuantifier : public Node
{
public:
    RangeQuantifier(NodePtr& inner, uint64_t min, uint64_t max, bool isMaxBounded)
    : inner {inner.release()}
    , min {min}
    , max {max}
    , isMaxBounded {isMaxBounded}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        inner->print(str);
        if(min==max)
        {
            str += "{" + std::to_string(min) + "}";
        }
        else if(isMaxBounded)
        {
            str += "{" + std::to_string(min) + "," + std::to_string(max) + "}";
        }
        else
        {
            str += "{" + std::to_string(min) + "," + "}";
        }
    }

    uint64_t min;
    uint64_t max;
    bool isMaxBounded;
     
    NodePtr inner;
};

class Any : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= ".";
    }
};

class Epsilon : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "";
    }
};

class Character : public Node
{
public:
    Character(CodePoint character, bool escaped)
    : mCodePoint{character}
    , mEscaped{escaped}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        if(mEscaped)
        {
            std::stringstream ss;
            ss << std::hex << std::setfill ('0') << std::setw(6) << mCodePoint;
            str+= "\\u";
            str+= ss.str();
        }
        else
        {
            str+= mCodePoint;
        }
    }

    bool mEscaped;
    CodePoint mCodePoint;
};

class CharacterClass : public Node
{
public:
    CharacterClass(std::vector<NodePtr>& codePointIntervals, bool isNegated)
    : CodePointIntervals{std::move(codePointIntervals)}
    , Negated{isNegated}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "[";

        if(Negated)
        {
            str+= "^";
        }

        for (const auto& item : CodePointIntervals )
        {
            item->print(str);
        }
        str+= "]";
    }

    std::vector<NodePtr> CodePointIntervals;
    bool Negated;
};

class CodePointRange : public Node
{
public:
    CodePointRange(NodePtr& start, NodePtr& end)
    : Start{std::move(start)}
    , End{std::move(end)}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        Start->print(str);
        str+= "-";
        End->print(str);
    }

    NodePtr Start;
    NodePtr End;
};

class CharacterClassAnyWord : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\w";
    }
};

class CharacterClassAnyWordNegated : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\W";
    }
};

class CharacterClassAnyDigit : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\d";
    }
};

class CharacterClassAnyDigitNegated : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\D";
    }
};

class CharacterClassAnyWhitespace : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\s";
    }
};

class CharacterClassAnyWhitespaceNegated : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\S";
    }
};

class AST
{
public:

    AST(NodePtr& node ) : root{std::move(node)}
    {}

    std::string print()
    {
        std::string str;
        root->print(str);
        return str;
    }

private:
    NodePtr root;
};

}
