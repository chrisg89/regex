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



void DisjoinOverlap_debug(ranges& ranges, char32_t min, char32_t max)
{
    enum class EndpointType { eStart , eEnd};
    using Point = std::pair<char32_t, EndpointType>;
    using Points = std::set<Point>;

    auto points = Points{};

    for (auto& range : ranges)
    {
        points.emplace(range.first, EndpointType::eStart);
        points.emplace(range.second, EndpointType::eEnd);
    }
    ranges.clear();

    auto current = min;
    for (auto& point : points)
    {

        std::cout << "--------------------------" << std::endl;
        std::cout << "current = " << current << std::endl;
        if(point.second  == EndpointType::eStart)
        {
            std::cout << "point = " << point.first << " " << "start" << std::endl;
        }
        else
        {
            std::cout << "point = " << point.first << " " << "end" << std::endl;
        }

        if(point.second == EndpointType::eStart)
        {
            if(current < point.first )
            {
                ranges.emplace_back(current, point.first-1);
                std::cout << "adds= " << current << "-" << point.first-1 << std::endl;
                current = point.first;
            }
            else
            {
                std::cout << "Ignore " << std::endl;
            }

        }
        else
        {
            ranges.emplace_back(current, point.first);
            std::cout << "insert point y= " << current << "-" << point.first << std::endl;
            current = point.first+1;
            //std::cout << "current = " << current << std::endl;
        }
    }

    if(current < max)
    {
        ranges.emplace_back(current, max);
    }
}

void DisjoinOverlap(ranges& ranges, char32_t min, char32_t max)
{
    enum class EndpointType { eStart , eEnd};
    using Point = std::pair<char32_t, EndpointType>;
    using Points = std::set<Point>;

    auto points = Points{};

    for (auto& range : ranges)
    {
        points.emplace(range.first, EndpointType::eStart);
        points.emplace(range.second, EndpointType::eEnd);
    }
    ranges.clear();

    auto current = min;
    for (auto& point : points)
    {
        if(point.second == EndpointType::eStart)
        {
            if(current < point.first )
            {
                ranges.emplace_back(current, point.first-1);
                current = point.first;
            }
        }
        else
        {
            ranges.emplace_back(current, point.first);
            current = point.first+1;
        }
    }

    if(current < max)
    {
        ranges.emplace_back(current, max);
    }
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

ranges makeCompressedAlphabet(TokenStream regex)
{
    ranges compressedAlphabet;

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
            compressedAlphabet.emplace_back(current.second, current.second); //TODO use real ranges here later
        }
    }

    DisjoinOverlap(compressedAlphabet, 0, 255); //TODO use min and max constexpr here later
    return compressedAlphabet;
}

void Regex::compile(std::string regex)
{
    
    auto tokenStream = TokenStream(regex);

    auto compressedAlphabet = makeCompressedAlphabet(regex);
    
    std::cout << "---------compressed alphabet---------" << std::endl;
    for (auto x : compressedAlphabet)
    {
        std::cout << x.first << " - " << x.second << std::endl;
    }

    if (!isValidRegex(tokenStream))
        assert(false);

        mDFA = build(tokenStream, kAlphabet);
}

bool Regex::match(std::string string)
{
    return mDFA.run(string);
}


bool Regex::search(std::string string)
{
    for(auto result : mDFA.search(string))
    {
        std::cout << result << std::endl;
    }

    return true; //todo
}



} //namespace regex