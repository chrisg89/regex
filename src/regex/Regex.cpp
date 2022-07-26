#include "Regex.hpp" 


#include <cassert> //todo try to replace with excpetion everywhere



namespace regex
{


fa::Alphabet kAlphabet
{
    '\t', '\n', '\v', '\f', '\r',
    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', 
    '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', 
    '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', 
    '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', 
    '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 
    'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 
    'z', '{', '|', '}', '~'
};







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

void Regex::compile(std::string regex)
{
    
    auto tokenStream = TokenStream(regex);

    if (!isValidRegex(tokenStream))
        assert(false);

        mDFA = build(tokenStream, kAlphabet);
}

bool Regex::match(std::string string)
{
    return mDFA.run(string);
}



} //namespace regex