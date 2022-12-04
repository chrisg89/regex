
#include "Alphabet.hpp"

#include <algorithm>
#include <set>

namespace regex
{

bool isSubset(const CodePointInterval inner, const CodePointInterval outer)
{
    if(outer.first <= inner.first && outer.second >= inner.second)
    {
        return true;
    }

    return false;
}

void disjoinOverlap(Alphabet& alphabet, CodePoint min, CodePoint max)
{
    enum class EndpointType { eStart , eEnd};
    using Point = std::pair<CodePoint, EndpointType>;
    using Points = std::set<Point>;

    auto points = Points{};

    for (const auto& codePointInterval : alphabet)
    {
        points.emplace(codePointInterval.first, EndpointType::eStart);
        points.emplace(codePointInterval.second, EndpointType::eEnd);
    }
    alphabet.clear();

    // this needs to be large enough to prevent overflow errors
    unsigned long long int current = min;

    for (const auto& point : points)
    {
        if(point.second == EndpointType::eStart)
        {
            if(current < point.first )
            {
                alphabet.emplace_back(current, point.first-1);
                current = point.first;
            }
        }
        else
        {
            alphabet.emplace_back(current, point.first);
            current = point.first+1;
        }
    }

    if(current < max)
    {
        alphabet.emplace_back(current, max);
    }
}

void negate(Alphabet& alphabet)
{
    std::sort(alphabet.begin(), alphabet.end(), [](const CodePointInterval& a, const CodePointInterval& b) { 
        return a.first < b.second;
    });

    Alphabet swapped;
    std::swap(swapped, alphabet);

    // this needs to be large enough to prevent overflow errors
    unsigned long long int current = kCodePointMin;

    for (const auto& interval : swapped )
    {
        if(interval.first != current)
        {
            alphabet.emplace_back(current, interval.first-1);
        }
        current = interval.second + 1;
    }

    if(current< kCodePointMax)
    { 
        alphabet.emplace_back(current, kCodePointMax);
    }
}

}