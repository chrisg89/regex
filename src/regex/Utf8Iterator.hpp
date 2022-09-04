#include <string>

namespace regex
{

class Utf8Iterator : public std::iterator<std::bidirectional_iterator_tag, char32_t, std::string::difference_type, const char32_t*, const char32_t&>
{
public:
 
    Utf8Iterator(std::string::const_iterator it);
 
    Utf8Iterator(const Utf8Iterator& source);
 
    Utf8Iterator& operator=(const Utf8Iterator& rhs);
 
    ~Utf8Iterator();

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