#include "ThompsonConstruction.hpp"

#include <cassert>
#include <stack>
#include <numeric>

namespace regex
{

BlackBox::BlackBox(StateId entry, StateId exit)
: entry {entry}
, exit {exit}
{}

} //namespace regex