#pragma once 

#include <cassert>

//TODO Unit test this class

template <typename Type1, typename Type2, typename Type1Hasher, typename Type2Hasher>
Bimap<Type1, Type2, Type1Hasher, Type2Hasher>::Bimap()
    : unordered_map1{}
    , unordered_map2{}
{}


template <typename Type1, typename Type2, typename Type1Hasher, typename Type2Hasher>
void Bimap<Type1, Type2, Type1Hasher, Type2Hasher>::insert(Type1 type1, Type2 type2)
{
        
        if (unordered_map1.find(type1) != unordered_map1.end())
            assert(false);  //todo replace with exception

        if (unordered_map2.find(type2) != unordered_map2.end())
            assert(false);  //todo replace with exception

        unordered_map1[type1] = type2;
        unordered_map2[type2] = type1;
}

template <typename Type1, typename Type2, typename Type1Hasher, typename Type2Hasher>
void Bimap<Type1, Type2, Type1Hasher, Type2Hasher>::insert(Type2 type2, Type1 type1)
{
        if (unordered_map1.find(type1) != unordered_map1.end())
            assert(false);  //todo replace with exception

        if (unordered_map2.find(type2) != unordered_map2.end())
            assert(false);  //todo replace with exception
            
        unordered_map1[type1] = type2;
        unordered_map2[type2] = type1;
}

template <typename Type1, typename Type2, typename Type1Hasher, typename Type2Hasher>
Type2 Bimap<Type1, Type2, Type1Hasher, Type2Hasher>::get(Type1 type1)
{
    return unordered_map1.at(type1);
}

template <typename Type1, typename Type2, typename Type1Hasher, typename Type2Hasher>
Type1 Bimap<Type1, Type2, Type1Hasher, Type2Hasher>::get(Type2 type2)
{
    return unordered_map2.at(type2);
}

template <typename Type1, typename Type2, typename Type1Hasher, typename Type2Hasher>
bool Bimap<Type1, Type2, Type1Hasher, Type2Hasher>::contains(Type1 type1)
{
    return (unordered_map1.find(type1) != unordered_map1.end());
}

template <typename Type1, typename Type2, typename Type1Hasher, typename Type2Hasher>
bool Bimap<Type1, Type2, Type1Hasher, Type2Hasher>::contains(Type2 type2)
{
    return (unordered_map2.find(type2) != unordered_map2.end());
}