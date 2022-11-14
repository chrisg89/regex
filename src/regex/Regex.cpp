#include "Regex.hpp" 


#include <cassert> //todo try to replace with excpetion everywhere

#include "Utf8Iterator.hpp"

namespace regex
{









bool isValidRegex(TokenStream regex)
{

    // TODO: there is prob a much more elegant way of writing this function. Refactor later?
    bool valid = true;
    int leftBracketCount = 0;
    int rightBracketCount = 0;

    Token current;
    Token next;

    while( true )
    {
        current = regex.get();
        next = regex.peek();

        if(current.first == TokenType::eEOF || !valid)
        {
            if (rightBracketCount != leftBracketCount)
            {
                valid = false;
            }
            break;
        }

        if (current.first == TokenType::eOpenBracket)
        {
            leftBracketCount++;
            if(next.first == TokenType::eOpenBracket){
                valid = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                valid = false;
            }
            else if(next.first == TokenType::eClosure){
                valid = false;
            }
            else if(next.first == TokenType::eClosurePlus){
                valid = false;
            }
            else if(next.first == TokenType::eOptional){
                valid = false;
            }
            else if(next.first == TokenType::eUnion){
                valid = false;
            }
            else if(next.first == TokenType::eEOF){
                valid = false;
            }
            else{
                valid = true;
            }
        }
        else if (current.first == TokenType::eCloseBracket)
        {
            rightBracketCount++;
            if(next.first == TokenType::eOpenBracket){
                valid = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                valid = true;
            }
            else if(next.first == TokenType::eClosure){
                valid = true;
            }
            else if(next.first == TokenType::eClosurePlus){
                valid = true;
            }
            else if(next.first == TokenType::eOptional){
                valid = true;
            }
            else if(next.first == TokenType::eUnion){
                valid = true;
            }
            else if(next.first == TokenType::eEOF){
                valid = true;
            }
            else{
                valid = true;
            }
        }
        else if (current.first == TokenType::eClosure)
        {
            if(next.first == TokenType::eOpenBracket){
                valid = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                valid = true;
            }
            else if(next.first == TokenType::eClosure){
                valid = true;
            }
            else if(next.first == TokenType::eClosurePlus){
                valid = true;
            }
            else if(next.first == TokenType::eOptional){
                valid = true;
            }
            else if(next.first == TokenType::eUnion){
                valid = true;
            }
            else if(next.first == TokenType::eEOF){
                valid = true;
            }
            else{
                valid = true;
            }
        }
        else if (current.first == TokenType::eClosurePlus)
        {
            if(next.first == TokenType::eOpenBracket){
                valid = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                valid = true;
            }
            else if(next.first == TokenType::eClosure){
                valid = true;
            }
            else if(next.first == TokenType::eClosurePlus){
                valid = true;
            }
            else if(next.first == TokenType::eOptional){
                valid = true;
            }
            else if(next.first == TokenType::eUnion){
                valid = true;
            }
            else if(next.first == TokenType::eEOF){
                valid = true;
            }
            else{
                valid = true;
            }
        }
        else if (current.first == TokenType::eOptional)
        {
            if(next.first == TokenType::eOpenBracket){
                valid = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                valid = true;
            }
            else if(next.first == TokenType::eClosure){
                valid = true;
            }
            else if(next.first == TokenType::eClosurePlus){
                valid = true;
            }
            else if(next.first == TokenType::eOptional){
                valid = true;
            }
            else if(next.first == TokenType::eUnion){
                valid = true;
            }
            else if(next.first == TokenType::eEOF){
                valid = true;
            }
            else{
                valid = true;
            }
        }
        else if (current.first == TokenType::eUnion)
        {
            if(next.first == TokenType::eOpenBracket){
                valid = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                valid = false;
            }
            else if(next.first == TokenType::eClosure){
                valid = false;
            }
            else if(next.first == TokenType::eClosurePlus){
                valid = false;
            }
            else if(next.first == TokenType::eOptional){
                valid = false;
            }
            else if(next.first == TokenType::eUnion){
                valid = false;
            }
            else if(next.first == TokenType::eEOF){
                valid = false;
            }
            else{
                valid = true;
            }
        }
        else
        {
            if(next.first == TokenType::eOpenBracket){
                valid = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                valid = true;
            }
            else if(next.first == TokenType::eClosure){
                valid = true;
            }
            else if(next.first == TokenType::eClosurePlus){
                valid = true;
            }
            else if(next.first == TokenType::eOptional){
                valid = true;
            }
            else if(next.first == TokenType::eUnion){
                valid = true;
            }
            else if(next.first == TokenType::eEOF){
                valid = true;
            }
            else{
                valid = true;
            }
        }

        if (rightBracketCount > leftBracketCount)
        {
            valid = false;
        }
    }

    return valid;
}


Regex::Regex()
: mDFA{{}} //TODO: create default constructor?
{}

void Regex::makeAlphabet(TokenStream regex)
{
    Alphabet alphabet;

    Token current;
    Token next;

    while( true )
    {
        current = regex.get();
        next = regex.peek();

        if(current.first == TokenType::eEOF)
        {
            break;
        }
        if(current.first == TokenType::eSymbol)
        {
            alphabet.emplace_back(current.second);
        }
    }

    DisjoinOverlap(alphabet, kCodePointMin, kCodePointMax);
    
    mAlphabet = alphabet;
}

void Regex::compile(std::string regex)
{
    
    auto tokenStream = TokenStream(regex);

    if (!isValidRegex(tokenStream)) // TODO rename to validateRegex and void. Throw exception inside
        assert(false);

    makeAlphabet(tokenStream);
    
    mDFA = build(tokenStream, mAlphabet);
}

fa::InputType Regex::findInAlphabet(CodePoint input)
{
    int index = 0;
    for(auto& x : mAlphabet)
    {
        if(input>= x.first && input <= x.second)
        {
            return index;
        }
        index++;
    }

    assert(false); //TODO throw exception
    return 0;
}

bool Regex::match(const std::string string)
{

    auto current = mDFA.getFirst();

    Utf8Iterator utf8It = string.cbegin();

    while( utf8It != string.end() ) //TODO chnage to for loop
    {
        //Translate CP to int
        auto input = findInAlphabet(*utf8It);
        current = mDFA.step(current, input);
        
        if(mDFA.isDeadState(current))
        {
            break;
        }

        utf8It++;
    }

    return mDFA.isFinalState(current);
}


bool Regex::search(std::string string)
{
    //TODO broken
    /*
    for(auto result : mDFA.search(string))
    {
        std::cout << result << std::endl;
    }
    */

    return true; //todo
}



} //namespace regex