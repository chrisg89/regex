
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
    explicit Regex(const std::string& pattern);
    bool match(const std::string& target);

private:

    Regex(const ast::AST& ast);

    automata::InputType findInAlphabet(CodePoint input);

    Alphabet mAlphabet;
    DFA mDFA;
};

} //namespace regex