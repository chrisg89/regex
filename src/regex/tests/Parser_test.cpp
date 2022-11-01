#include "catch.hpp"
#include "Parser.hpp"

#include <stdexcept>
#include <iostream>

namespace regex::parser {

namespace {

using Catch::Contains;

//TODO : test anchors. Rename /B to word boundary?

SCENARIO("Parse characters")
{
    SECTION("Simple character")
    {
        const std::string regex = "a";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character from unicode")
    {
        const std::string regex = "a";
        //TODO not yet implemented
    }

    SECTION("Any character .")
    {
        const std::string regex = ".";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\n")
    {
        const std::string regex = "\\n";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\f")
    {
        const std::string regex = "\\f";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\r")
    {
        const std::string regex = "\\r";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\t")
    {
        const std::string regex = "\\t";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\v")
    {
        const std::string regex = "\\v";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped character \\a")
    {
        const std::string regex = "\\a";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character ^")
    {
        const std::string regex = "\\^";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character $")
    {
        const std::string regex = "\\$";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character *")
    {
        const std::string regex = "\\*";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character +")
    {
        const std::string regex = "\\+";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character ?")
    {
        const std::string regex = "\\?";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character .")
    {
        const std::string regex = "\\.";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character |")
    {
        const std::string regex = "\\|";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character (")
    {
        const std::string regex = "\\(";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character )")
    {
        const std::string regex = "\\)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character [")
    {
        const std::string regex = "\\[";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Escaped meta character ]")
    {
        const std::string regex = "\\]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    //chris
}

SCENARIO("Parse shorthand character classes") 
{
    SECTION("Digit")
    {
        const std::string regex = R"(\d)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Negated digit character")
    {
        const std::string regex = R"(\D)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Word character")
    {
        const std::string regex = R"(\w)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Negated word character")
    {
        const std::string regex = R"(\W)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("White space character")
    {
        const std::string regex = R"(\s)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Negated white space character")
    {
        const std::string regex = R"(\S)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }
}

SCENARIO("Parse character classes") 
{

    SECTION("Empty Character class is allowed")
    {
        const std::string regex = "[]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Simple character class")
    {
        const std::string regex = "[hello]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Negated character class")
    {
        const std::string regex = "[^hello]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with shorthand")
    {
        const std::string regex = R"([\w\W\d\D\s\S])";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with escaped characters that have special meaning")
    {
        const std::string regex = R"([\n\f\r\t\v\a\\])";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Character class with escaped meta characters")
    {
        const std::string regex = R"([\^\$\*\+\?\.\(\)\[\]])";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    /*
    TODO: this is wrong. Character class needs its own special character parser
    SECTION("Character class with characters that are considered meta characters outside the character class")
    {
        const std::string regex = "[$*+?.()]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }
    */

    SECTION("Character class with ranges")
    {
        const std::string regex = "[a-zA-Z0-9]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Throw exception when character range is out of order")
    {
        const std::string regex = "[z-a]";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Character range is out of order"));
    }

    SECTION("Throw exception when character range is out of order")
    {
        const std::string regex = "[a-]";
        //TODO test this
    }

    SECTION("Throw exception when the closing bracket is missing")
    {
        const std::string regex = "[123";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Character class missing closing bracket"));
    }
}

SCENARIO("Parse quantifiers") 
{
    SECTION("Lazy Modifier (kleene star *) is not supported")
    {
        const std::string regex = "a*?";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (kleene plus +) is not supported")
    {
        const std::string regex = "a+?";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (optional ?) is not supported")
    {
        const std::string regex = "a??";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lazy modifier is not supported"));
    }

    SECTION("Lazy Modifier (ranged quantifier) is not supported")
    {
        const std::string regex = "a{1,3}?";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lazy modifier is not supported"));
    }

    SECTION("Kleene star (*)")
    {
        const std::string regex = "a*";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Kleene plus (+)")
    {
        const std::string regex = "a+";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Optional (?)")
    {
        const std::string regex = "a?";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier: lower bound only ")
    {
        const std::string regex = "a{100}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier: upper bound is omitted ")
    {
        const std::string regex = "a{100,}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier: lower and upper bound ")
    {
        const std::string regex = "a{100,200}";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Ranged quantifier throws when lower bound is too large")
    {
        const std::string regex = "a{999999999999}";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Lower bound on ranged quantifier too large"));
    }

    SECTION("Ranged quantifier throws when upper bound is too large")
    {
        const std::string regex = "a{1,999999999999}";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Upper bound on ranged quantifier too large"));
    }

    SECTION("Ranged quantifier throws when lower bound is greater than upper bound")
    {
        const std::string regex = "a{2,1}";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("The quantifier range is out of order"));
    }
}

SCENARIO("Parse backreference") 
{
    SECTION("Backreference is not supported")
    {
        const std::string regex = R"(a\4)";
        auto ast = ast::AST();
        REQUIRE_THROWS_WITH(Parser(ast, regex), Contains("Backreferences are not supported"));
    }
}

SCENARIO("Parse concatenation")
{
    SECTION("Concatenation of two characters")
    {
        const std::string regex = "ab";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Concatenation of two escaped meta characters")
    {
        const std::string regex = R"(\*\*)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Concatenation of two escaped characters with special meaning")
    {
        const std::string regex = R"(\n\r)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Concatenation of two characters from unicode")
    {
        const std::string regex = R"(\n\r)";
        //TODO
    }

    SECTION("Concatenation of two character classes")
    {
        const std::string regex = "[abc][123]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Concatenation of two character groups")
    {
        const std::string regex = R"(\n\r)";
        //TODO
    }
}

SCENARIO("Parse Alternation")
{
    SECTION("Alternation of two characters")
    {
        const std::string regex = "a|b";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Alternation of two escaped meta characters")
    {
        const std::string regex = R"(\*|\*)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Alternation of two escaped characters with special meaning")
    {
        const std::string regex = R"(\n|\r)";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Alternation of two characters from unicode")
    {
        const std::string regex = R"(\n|\r)";
        //TODO
    }

    SECTION("Alternation of two character classes")
    {
        const std::string regex = "[abc]|[123]";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("Alternation of two character groups")
    {
        const std::string regex = R"(\n|\r)";
        //TODO
    }

    SECTION("Alternation and concatenation")
    {
        const std::string regex = "123|abc|xy|z";
        auto ast = ast::AST();
        Parser(ast, regex);
        CHECK(ast.print() == regex);
    }

    SECTION("TODO what to name this???")
    {
        const std::string regex = "1|";
        //TODO
    }
}


}
}