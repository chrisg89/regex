
#pragma once

#include "NFA.hpp"
#include "DFA.hpp"

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



constexpr char epsilon = 0; //TODO: map to definition in NFA

bool isValidRegex(std::string regex);
std::string PreprocessRegex(std::string regex);
std::string RegexInfixToPostfix(std::string regex);
std::string RegexPostfixToInfix(std::string postfix);
void regexToNFA(NFA& nfa, std::string regex);


} //namespace regex