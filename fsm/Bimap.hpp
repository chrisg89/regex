
#pragma once 
#include <unordered_map>

template <typename Type1, typename Type2, typename Type1Hasher = std::hash<Type1>, typename Type2Hasher = std::hash<Type2>>
class Bimap
{
public:
    Bimap();

    void insert(Type1, Type2);
    void insert(Type2, Type1);

    Type2 get(Type1);
    Type1 get(Type2);

private:
    std::unordered_map<Type1, Type2, Type1Hasher> unordered_map1;
    std::unordered_map<Type2, Type1, Type2Hasher> unordered_map2;
};



#include "Bimap-inl.hpp"