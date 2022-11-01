#include <string>
#include <memory>
#include <vector>
#include "CodePoint.hpp"

namespace regex::ast
{

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
        left->print(str);
        str+= "|";
        right->print(str);
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
        left->print(str);
        right->print(str);
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
     
    NodePtr inner; // use NodePtr
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

class CharacterLiteral : public Node
{
public:
    CharacterLiteral(CodePoint character)
    : Character{character}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= Character;
    }

    CodePoint Character;
};

class CharacterFromUnicodeEscape : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "TODO";
    }

    CodePoint Character;
};

class CharacterFromEscape : public Node
{
public:
    CharacterFromEscape(CodePoint character)
    : Character{character}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\";
        str+= Character;
    }

    CodePoint Character;
};

class CharacterClass : public Node
{
public:
    CharacterClass(std::vector<NodePtr>&& codePointIntervals, bool isNegated)
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
    CodePointRange(CodePointInterval interval)
    : Interval{interval}
    {}

private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= Interval.first + "-" + Interval.second;
    }

    CodePointInterval Interval;
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

class CharacterClassAnyWordInverted : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\W";
    }
};

class CharacterClassAnyDecimalDigit : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\d";
    }
};

class CharacterClassAnyDecimalDigitInverted : public Node
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

class CharacterClassAnyWhitespaceInverted : public Node
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

    std::string print()
    {
        std::string str;
        root->print(str);
        return str;
    }

    NodePtr root;  // should this be private? maybe make parser friend
};

}
