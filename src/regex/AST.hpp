#include <string>
#include <memory>

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

    int min;
    int max;
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
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "[...]";
    }

    CodePoint Character;
};

class ShorthandCharacterClass : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\" + Character;
    }

    CodePoint Character;
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
