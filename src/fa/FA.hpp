#pragma once 

#include <vector>
#include <set>

namespace fa
{

using StateId = int;
using Alphabet = std::set<char>;
constexpr char kNullState = -1;
constexpr char kEpsilon = 0;

}