#include "Alphabet.hpp"
#include <catch2/catch.hpp>

namespace regex
{

namespace
{

SCENARIO("Alphabet utilities")
{
    SECTION("disjoin overlapping intervals in the alphabet #1")
    {
        auto myRanges = Alphabet{ { 2, 2 },   { 3, 5 },   { 0, 1 },  { 33, 44 },
                                  { 33, 37 }, { 31, 40 }, { 33, 39 } };
        disjoinOverlap(myRanges, 0, 255);
        CHECK((myRanges == Alphabet({ { 0, 1 },
                                      { 2, 2 },
                                      { 3, 5 },
                                      { 6, 30 },
                                      { 31, 32 },
                                      { 33, 37 },
                                      { 38, 39 },
                                      { 40, 40 },
                                      { 41, 44 },
                                      { 45, 255 } })));
    }

    SECTION("disjoin overlapping intervals in the alphabet #2")
    {
        auto myRanges = Alphabet{ { 65, 65 }, { 124, 124 }, { 124, 124 },
                                  { 66, 66 }, { 255, 255 }, { 255, 255 } };
        disjoinOverlap(myRanges, 0, 255);
        CHECK((myRanges == Alphabet({ { 0, 64 },
                                      { 65, 65 },
                                      { 66, 66 },
                                      { 67, 123 },
                                      { 124, 124 },
                                      { 125, 254 },
                                      { 255, 255 } })));
    }
}

} // namespace
} // namespace regex