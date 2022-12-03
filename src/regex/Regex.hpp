
#pragma once

#include "Alphabet.hpp"
#include "CodePoint.hpp"
#include "DFA.hpp"
#include <string>

namespace regex
{

using DFA = automata::DFA;

class Regex
{

public:
    Regex(const std::string& regex);
    bool match(std::string string);
    bool search(std::string string);

private:
    DFA mDFA;
    Alphabet mAlphabet;
    automata::InputType findInAlphabet(CodePoint input); // todo should be free standing function
};

} //namespace regex