#include "catch.hpp"
#include "Parser.hpp"

#include <stdexcept>
#include <iostream>

namespace regex::parser {

namespace {



SCENARIO( "Parse comma", "[Parse]" ) 
{
    const std::string hello = R"(he\nllo)";

    auto begin = hello.begin();
    auto end = hello.end();

    auto ast = ast::AST();
    auto parser = Parser(ast, begin, end);

    std::cout << ast.print() << std::endl;
}

SCENARIO( "Parse KleenStar", "[Parse]" ) 
{
    const std::string hello = R"(a*)";

    auto begin = hello.begin();
    auto end = hello.end();

    auto ast = ast::AST();
    auto parser = Parser(ast, begin, end);

    std::cout << ast.print() << std::endl;
}


SCENARIO( "Parse Ranged Qalifier 1", "[Parse]" ) 
{
    const std::string hello = R"(a{12})";

    auto begin = hello.begin();
    auto end = hello.end();

    auto ast = ast::AST();
    auto parser = Parser(ast, begin, end);

    std::cout << ast.print() << std::endl;
}

SCENARIO( "Parse Ranged Qalifier 2", "[Parse]" ) 
{
    const std::string hello = R"(a{12,})";

    auto begin = hello.begin();
    auto end = hello.end();

    auto ast = ast::AST();
    auto parser = Parser(ast, begin, end);

    std::cout << ast.print() << std::endl;
}

SCENARIO( "Parse Ranged Qalifier 3", "[Parse]" ) 
{
    const std::string hello = R"(a{12,13})";

    auto begin = hello.begin();
    auto end = hello.end();

    auto ast = ast::AST();
    auto parser = Parser(ast, begin, end);

    std::cout << ast.print() << std::endl;
}

SCENARIO( "Parse Ranged Qalifier 4", "[Parse]" ) 
{
    const std::string hello = R"(a{12,11})";

    auto begin = hello.begin();
    auto end = hello.end();

    auto ast = ast::AST();
    auto parser = Parser(ast, begin, end);

    std::cout << ast.print() << std::endl;
}



}
}