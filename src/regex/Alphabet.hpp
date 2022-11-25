
#pragma once

#include "CodePoint.hpp"

#include <vector>

namespace regex
{

using Alphabet = std::vector<CodePointInterval>;

bool isSubset(CodePointInterval inner, CodePointInterval outer);
void disjoinOverlap(Alphabet& alphabet, CodePoint min, CodePoint max);
void negate(Alphabet& alphabet);

}