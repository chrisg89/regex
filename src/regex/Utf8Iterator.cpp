#include "Utf8Iterator.hpp"

namespace regex
{

const unsigned char kFirstBitMask  = 128;
const unsigned char kSecondBitMask = 64;
const unsigned char kThirdBitMask  = 32;
const unsigned char kFourthBitMask = 16;

Utf8Iterator::Utf8Iterator(std::string::const_iterator it) 
: mStringIterator(it)
{}

Utf8Iterator& Utf8Iterator::operator++()
{
    char firstByte = *mStringIterator;
 
    std::string::difference_type offset = 1;
 
    if((firstByte & kFirstBitMask) != 0) // This means the first byte has a value greater than 127, and so is beyond the ASCII range.
    {
        if((firstByte & kThirdBitMask) != 0) // This means that the first byte has a value greater than 224, and so it must be at least a three-octet code point.
        {
            if((firstByte & kFourthBitMask) != 0) // This means that the first byte has a value greater than 240, and so it must be a four-octet code point.
            {
                offset = 4;
            }    
            else
            {
                offset = 3;
            }  
        }
        else
        {
            offset = 2;
        }
    }
 
    mStringIterator += offset;
    mDirty = true;
 
    return *this;
}
 
Utf8Iterator Utf8Iterator::operator++(int)
{
    Utf8Iterator temp = *this;
    ++(*this);
    return temp;
}
 
Utf8Iterator& Utf8Iterator::operator--()
{
    --mStringIterator;
 
    if((*mStringIterator & kFirstBitMask) == 0) // This means that the previous byte is not an ASCII character.
    {
        --mStringIterator;
        if((*mStringIterator & kSecondBitMask) == 0)
        {
            --mStringIterator;
            if((*mStringIterator & kSecondBitMask) == 0)
            {
                --mStringIterator;
            }
        }
    }
    mDirty = true;
     
    return *this;
}
 
Utf8Iterator Utf8Iterator::operator--(int)
{
    Utf8Iterator temp = *this;
    --(*this);
    return temp;
}

CodePoint Utf8Iterator::operator*() const
{
    CalculateCurrentCodePoint();
 
    return mCurrentCodePoint;
}

void Utf8Iterator::CalculateCurrentCodePoint() const
{

    if(mDirty)
    {
        CodePoint codePoint = 0;
 
        char firstByte = *mStringIterator;
    
        if((firstByte & kFirstBitMask) != 0) // This means the first byte has a value greater than 127, and so is beyond the ASCII range.
        {
            if((firstByte & kThirdBitMask) != 0) // This means that the first byte has a value greater than 191, and so it must be at least a three-octet code point.
            {
                if((firstByte & kFourthBitMask) != 0) // This means that the first byte has a value greater than 224, and so it must be a four-octet code point.
                {
                    codePoint = static_cast<CodePoint>((firstByte & 0x07) << 18);
                    char secondByte = *(mStringIterator + 1);
                    codePoint +=  static_cast<CodePoint>((secondByte & 0x3f) << 12);
                    char thirdByte = *(mStringIterator + 2);
                    codePoint +=  static_cast<CodePoint>((thirdByte & 0x3f) << 6);
                    char fourthByte = *(mStringIterator + 3);
                    codePoint += static_cast<CodePoint>((fourthByte & 0x3f));
                }
                else
                {
                    codePoint = static_cast<CodePoint>((firstByte & 0x0f) << 12);
                    char secondByte = *(mStringIterator + 1);
                    codePoint += static_cast<CodePoint>((secondByte & 0x3f) << 6);
                    char thirdByte = *(mStringIterator + 2);
                    codePoint +=  static_cast<CodePoint>((thirdByte & 0x3f));
                }
            }
            else
            {
                codePoint = static_cast<CodePoint>((firstByte & 0x1f) << 6);
                char secondByte = *(mStringIterator + 1);
                codePoint +=  static_cast<CodePoint>((secondByte & 0x3f));
            }
        }
        else
        {
            codePoint = static_cast<CodePoint>(firstByte);
        }

        mCurrentCodePoint = codePoint;
        mDirty = false;
    }
}

bool Utf8Iterator::operator==(const Utf8Iterator& rhs) const
{
    return mStringIterator ==  rhs.mStringIterator;
}
 
bool Utf8Iterator::operator!=(const Utf8Iterator& rhs) const
{
    return mStringIterator !=  rhs.mStringIterator;
}

bool Utf8Iterator::operator==(std::string::iterator rhs) const
{
    return mStringIterator ==  rhs;
}
 
bool Utf8Iterator::operator==(std::string::const_iterator rhs) const
{
    return mStringIterator ==  rhs;
}
 
bool Utf8Iterator::operator!=(std::string::iterator rhs) const
{
    return mStringIterator !=  rhs;
}
 
bool Utf8Iterator::operator!=(std::string::const_iterator rhs) const
{
    return mStringIterator !=  rhs;
}


}