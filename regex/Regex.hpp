
#pragma once

#include "FSM.hpp"

#include <string>



namespace regex
{
    using FSM = fsm::FSM;
    using State = fsm::State;

    constexpr char epsilon = 0;

    bool isValidRegex(std::string regex);
    std::string PreprocessRegex(std::string regex);
    std::string RegexInfixToPostfix(std::string regex);
    std::string RegexPostfixToInfix(std::string postfix);
    void regexToNFA(FSM& fsm, std::string regex);
}