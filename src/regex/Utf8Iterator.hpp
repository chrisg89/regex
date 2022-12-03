#pragma once
#include <string>

namespace regex
{

// The implementation of this utf8 iterator was taken from: 
// http://www.nubaria.com/en/blog/?p=371
// 
// The current implementation assumes the source
// string is valid utf-8. No error checking was 
// implemented. Future work may address this.

class Utf8Iterator : public std::iterator<std::bidirectional_iterator_tag, char32_t, std::string::difference_type, const char32_t*, const char32_t&>
{
public:
 
    Utf8Iterator(std::string::const_iterator it);
 
    Utf8Iterator(const Utf8Iterator& source);
 
    Utf8Iterator& operator=(const Utf8Iterator& rhs);

    Utf8Iterator& operator++();
    Utf8Iterator operator++(int);
    Utf8Iterator& operator--();
    Utf8Iterator operator--(int);

    char32_t operator*() const;

    bool operator==(const Utf8Iterator& rhs) const;
    bool operator!=(const Utf8Iterator& rhs) const;

    bool operator==(std::string::iterator rhs) const;
    bool operator==(std::string::const_iterator rhs) const;
    bool operator!=(std::string::iterator rhs) const;
    bool operator!=(std::string::const_iterator rhs) const;

private:
 
    std::string::const_iterator mStringIterator;
    mutable char32_t mCurrentCodePoint;
    mutable bool mDirty;
 
    void CalculateCurrentCodePoint() const;
};

}