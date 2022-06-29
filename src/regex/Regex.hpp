
#pragma once

#include "NFA.hpp"
#include "DFA.hpp"
#include "TokenStream.hpp"
#include "ThompsonConstruction.hpp"

#include <string>

namespace regex
{

using NFA = fa::NFA;
using DFA = fa::DFA;
using StateId = fa::StateId;

class Regex
{

public:
    Regex();

    void compile(std::string regex);

    bool match(std::string string);

private:
    DFA mDFA;

};

//TODO encapsulate this stuff
bool isValidRegex(TokenStream regex);
TokenStream PreprocessRegex(TokenStream regex);
TokenStream RegexInfixToPostfix(TokenStream regex);
NFA regexToNFA(TokenStream regex);


} //namespace regex