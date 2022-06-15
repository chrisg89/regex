
#pragma once

#include "NFA.hpp"

#include <string>



namespace regex
{
    using NFA = nfa::NFA;
    using StateId = nfa::StateId;

    constexpr char epsilon = 0; //TODO: map to definition in NFA

    bool isValidRegex(std::string regex);
    std::string PreprocessRegex(std::string regex);
    std::string RegexInfixToPostfix(std::string regex);
    std::string RegexPostfixToInfix(std::string postfix);
    void regexToNFA(NFA& nfa, std::string regex);
}