
#pragma once

#include "TokenStream.hpp"
#include "DFABuilder.hpp"

#include <string>
#include <cassert> 

namespace regex
{

using DFA = fa::DFA;
bool isValidRegex(TokenStream regex);
extern fa::Alphabet kAlphabet;



class Regex
{

public:

    template <class InputIt>
    Regex(const InputIt first, const InputIt last)
    : mDFA{}
    {
        if(first!=last)
        {
            std::string temp{first,last};  ////TODO fix: this is very wasteful!

            auto tokenStream = TokenStream(temp);

            if (!isValidRegex(tokenStream))
                assert(false);

            mDFA = build(tokenStream, kAlphabet);
        }
    }

    template <class InputIt>
    bool match(InputIt first, InputIt last) const
    {
        std::string temp(first,last);  ////TODO fix: this is very wasteful!
        return mDFA.run(temp);
    }

    //TODO with match results
    //TODO should there be DFA builder and DFA Runner classes?
    bool search(std::string string);


private:
    DFA mDFA;

};



} //namespace regex