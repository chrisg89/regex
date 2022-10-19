#include <string>
#include <memory>

#include "CodePoint.hpp"

namespace regex::ast
{

class Node
{
public:
    virtual void eval() = 0;
    virtual void print(std::string&) = 0;

private:


};

class Alternative : public Node
{
private:

    void eval() final
    {}

    void print(std::string& str) final
    {
        left->print(str);
        str+= "|";
        right->print(str);
    }
     
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class Concatenation : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        left->print(str);
        right->print(str);
    }
     
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
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
     
    std::unique_ptr<Node> inner;
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
     
    std::unique_ptr<Node> inner;
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
     
    std::unique_ptr<Node> inner;
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
     
    std::unique_ptr<Node> inner;
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
     
    std::unique_ptr<Node> inner;
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
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= character;
    }

    CodePoint character;
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

    CodePoint character;
};

class CharacterFromEscape : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "TODO";
    }

    CodePoint character;
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

    CodePoint character;
};

class ShorthandCharacterClass : public Node
{
private:
    void eval() final
    {}

    void print(std::string& str) final
    {
        str+= "\\" + character;
    }

    CodePoint character;
};

}
