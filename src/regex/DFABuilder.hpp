
#pragma once

#include "NFA.hpp"
#include "DFA.hpp"
#include "TokenStream.hpp"

namespace regex
{

using NFA = fa::NFA;
using DFA = fa::DFA;
using Alphabet = fa::Alphabet;

DFA build(TokenStream regex, Alphabet alphabet);

// exposed for unit tests
TokenStream PreprocessRegex(TokenStream regex);

//exposed for unit tests
TokenStream RegexInfixToPostfix(TokenStream regex);



} //namespace regex