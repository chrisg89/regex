
#pragma once

#include <utility>

namespace regex
{

using CodePoint = unsigned char;
using CodePointInterval = std::pair<CodePoint, CodePoint>;

constexpr CodePoint kCodePointMin = 0x00U;
constexpr CodePoint kCodePointMax = 0xFFU;

//TODO Add calculation logic here for Codepoints?

}