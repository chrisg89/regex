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

}
}