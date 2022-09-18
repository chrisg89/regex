
#pragma once

#include "NFA.hpp"
#include "DFA.hpp"
#include "TokenStream.hpp"
#include "Alphabet.hpp"

namespace regex
{

using NFA = fa::NFA;
using DFA = fa::DFA;

DFA build(TokenStream regex, Alphabet& alphabet);

// exposed for unit tests
TokenStream InsertExplicitConcat(TokenStream regex);

//exposed for unit tests
TokenStream InfixToPostfix(TokenStream regex);



} //namespace regex