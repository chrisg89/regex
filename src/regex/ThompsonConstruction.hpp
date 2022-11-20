#pragma once

#include "NFA.hpp"
#include "Alphabet.hpp"

namespace regex
{

using StateId = fa::StateId;

struct BlackBox
{
    BlackBox(StateId entry, StateId exit);
    StateId entry;
    StateId exit;
};

} //namespace regex