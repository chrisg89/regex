#include "Regex.hpp" 


#include <stack>
#include <map> 
#include <sstream>
#include <cassert> //todo try to replace with excpetion everywhere
#include <algorithm>



namespace regex
{


fa::Alphabet kAlphabet{' ',
                   '!', 
                   '"', 
                   '#', 
                   '$', 
                   '%',
                   '&', 
                   '\'', 
                   '(', 
                   ')', 
                   '*', 
                   '+',
                   ',', 
                   '-', 
                   '.', 
                   '/', 
                   '0', 
                   '1', 
                   '2', 
                   '3', 
                   '4', 
                   '5', 
                   '6', 
                   '7', 
                   '8', 
                   '9', 
                   ':', 
                   ';', 
                   '<',
                   '=', 
                   '>', 
                   '?', 
                   '@', 
                   'A',
                   'B', 
                   'C', 
                   'D', 
                   'E', 
                   'F', 
                   'G', 
                   'H', 
                   'I', 
                   'J', 
                   'K', 
                   'L', 
                   'M', 
                   'N', 
                   'O', 
                   'P', 
                   'Q', 
                   'R', 
                   'S', 
                   'T', 
                   'U', 
                   'V', 
                   'W', 
                   'X', 
                   'Y', 
                   'Z',
                   '[', 
                   '\\', 
                   ']', 
                   '^', 
                   '_', 
                   '`', 
                   'a', 
                   'b', 
                   'c', 
                   'd', 
                   'e', 
                   'f',
                   'g', 
                   'h', 
                   'i', 
                   'j', 
                   'k', 
                   'l',
                   'm', 
                   'n', 
                   'o', 
                   'p', 
                   'q', 
                   'r',
                   's', 
                   't', 
                   'u', 
                   'v', 
                   'w', 
                   'x',
                   'y', 
                   'z', 
                   '{',  
                   '|',  
                   '}',  
                   '~'
                   };





TokenStream PreprocessRegex(TokenStream regex)  //TODO rename to insert???
{

    TokenStream processed;
    bool insert;

    while( true )
    {
        auto current = regex.get();
        auto next = regex.peek();
        insert = false;

        if(next.first == TokenType::eEOF)
        {
            processed.insert(current);
            break;
        }

        if(current.first == TokenType::eClosure)
        {
            if(next.first == TokenType::eClosure){
                insert = false;
            }
            else if(next.first == TokenType::eUnion){
                insert = false;
            }
            else if(next.first == TokenType::eOpenBracket){
                insert = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                insert = false;
            }
            else{
                insert = true;
            }
        }
        else if (current.first == TokenType::eUnion)
        {
            insert = false;
        }
        else if (current.first == TokenType::eOpenBracket)
        {
            insert = false;
        }
        else if (current.first == TokenType::eCloseBracket)
        {
            if(next.first == TokenType::eClosure){
                insert = false;
            }
            else if(next.first == TokenType::eUnion){
                insert = false;
            }
            else if(next.first == TokenType::eOpenBracket){
                insert = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                insert = false;
            }
            else{
                insert = true;
            }
        }
        else
        {
            if(next.first == TokenType::eClosure){
                insert = false;
            }
            else if(next.first == TokenType::eUnion){
                insert = false;
            }
            else if(next.first == TokenType::eOpenBracket){
                insert = true;
            }
            else if(next.first == TokenType::eCloseBracket){
                insert = false;
            }
            else{
                insert = true;
            }
        }

        processed.insert(current);
        if(insert)
        {
            processed.insert(Token{TokenType::eConcat, '&'});
        }

    }

    return processed;
}

TokenStream RegexInfixToPostfix(TokenStream infix)
{
    TokenStream postfix;
    std::stack<Token> stack;

    auto token = Token{};
    while( token = infix.get(), token.first != TokenType::eEOF )
    {

        if(token.first == TokenType::eOpenBracket)
        {
            stack.push(token);
        }

        else if(token.first == TokenType::eCloseBracket)
        {
            while(stack.top().first != TokenType::eOpenBracket)
            {
                postfix.insert(stack.top());
                stack.pop();
            }
            stack.pop();
        }

        else if(token.first == TokenType::eUnion || 
                token.first == TokenType::eConcat || 
                token.first == TokenType::eClosure)
        {
            while(!stack.empty())
            {
                if (stack.top().first == TokenType::eOpenBracket)
                    break;
                if (stack.top().first < token.first)
                    break;

                postfix.insert(stack.top());
                stack.pop();
            }
            stack.push(token);
        }

        else
        {
            postfix.insert(token);
        }

    }
    while(!stack.empty())
    {
        postfix.insert(stack.top());
        stack.pop();
    }

    return postfix;
}

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

fa::Alphabet getAlphabet(TokenStream regex)
{
    fa::Alphabet alphabet;

    auto token = Token{};
    while( token = regex.get(), token.first != TokenType::eEOF )
    {
        if (token.first == TokenType::eSymbol)
        {
            alphabet.insert(token.second);
        }

    }

    return alphabet;
}




// maybe change to return FSM by value? Would this break the 
// internal references? I think it would be a move...
NFA regexToNFA(TokenStream regex)
{
    std::stack<BlackBox> stack;

    auto alphabet = kAlphabet; //getAlphabet(regex);
    NFA nfa{alphabet};

    auto token = Token{};
    while( token = regex.get(), token.first != TokenType::eEOF )
    {

        if(token.first == TokenType::eUnion)
        {
            auto op1 = stack.top();
            stack.pop();
            auto op2 = stack.top();
            stack.pop();
            stack.push(buildUnion(nfa, op2, op1));
        }
        else if(token.first == TokenType::eConcat)
        {   
            auto op1 = stack.top();
            stack.pop();
            auto op2 = stack.top();
            stack.pop();
            stack.push(buildConcatenation(nfa, op2, op1));
        }
        else if(token.first == TokenType::eClosure)
        {
            auto op1 = stack.top();
            stack.pop();
            stack.push(buildClosure(nfa, op1));
        }

        else
        {
            stack.push(buildSymbol(nfa, token.second));
        }

    }
    assert(stack.size()==1);

    // stack now contains the black box containing within it
    // the FSA equivalent of the regex. Only need to connect
    // the entry and exit states of the black box to the start
    // and end states.
    auto bb = stack.top();
    auto start = nfa.addState(true, false);
    auto end = nfa.addState(false, true);
    nfa.addTransition(fa::kEpsilon, start, bb.entry);
    nfa.addTransition(fa::kEpsilon, bb.exit, end);

    return nfa;
}


Regex::Regex()
: mDFA{{}} //TODO: create default constructor?
{}

void Regex::compile(std::string regex)
{
    
    auto tokenStream = TokenStream(regex);

    if (!isValidRegex(tokenStream))
        assert(false);


    auto preprocessed = PreprocessRegex(tokenStream);
    auto postfix = RegexInfixToPostfix(preprocessed);

    auto nfa = regexToNFA(postfix);

    //std::cout << nfa.toPlantUML();

    mDFA = nfa.toDFA();
    mDFA.minimizeDFA();

    //std::cout << nfa.toPlantUML();


}

bool Regex::match(std::string string)
{
    return mDFA.run(string);
}



} //namespace regex