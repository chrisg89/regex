
#pragma once

#include "Alphabet.hpp"
#include "CodePoint.hpp"
#include "DFA.hpp"
#include "AST.hpp"
#include <string>

namespace regex
{

using automata::DFA;

class Regex
{

public:
    Regex(const std::string& regex);
    bool match(const std::string& string);
    bool search(const std::string& string);

private:

    Regex(const ast::AST& ast);

    automata::InputType findInAlphabet(CodePoint input);

    Alphabet mAlphabet;
    DFA mDFA;
};

} //namespace regex