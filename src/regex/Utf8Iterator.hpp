#pragma once

#include "CodePoint.hpp"
#include <string>

namespace regex
{

// The implementation of this utf8 iterator was taken from: 
// http://www.nubaria.com/en/blog/?p=371
// 
// The current implementation assumes the source
// string is valid utf-8. No error checking was 
// implemented. Future work may address this.

class Utf8Iterator
{
public:

    // iterator traits
    using difference_type = std::string::difference_type;
    using value_type = CodePoint;
    using pointer = const CodePoint*;
    using reference = const CodePoint&;
    using iterator_category = std::bidirectional_iterator_tag;
 
    Utf8Iterator(std::string::const_iterator it);
 
    Utf8Iterator& operator++();
    Utf8Iterator operator++(int);
    Utf8Iterator& operator--();
    Utf8Iterator operator--(int);

    CodePoint operator*() const;

    bool operator==(const Utf8Iterator& rhs) const;
    bool operator!=(const Utf8Iterator& rhs) const;

    bool operator==(std::string::iterator rhs) const;
    bool operator==(std::string::const_iterator rhs) const;
    bool operator!=(std::string::iterator rhs) const;
    bool operator!=(std::string::const_iterator rhs) const;

private:
 
    std::string::const_iterator mStringIterator;
    mutable CodePoint mCurrentCodePoint{};
    mutable bool mDirty{true};
 
    void CalculateCurrentCodePoint() const;
};

}