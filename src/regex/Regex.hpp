
#pragma once

#include "Alphabet.hpp"
#include "CodePoint.hpp"
#include "DFA.hpp"
#include <string>

namespace regex
{

using DFA = fa::DFA;

class Regex
{

public:
    Regex();
    void compile(std::string regex);
    bool match(std::string string);
    bool search(std::string string);

private:
    DFA mDFA;
    Alphabet mAlphabet;
    fa::InputType findInAlphabet(CodePoint input); // should be free standing function
};

} //namespace regex