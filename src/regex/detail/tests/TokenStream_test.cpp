
#include "catch.hpp"
#include "TokenStream.hpp"

#include <stdexcept>

namespace regex {

namespace {

//TODO extraction of meta characters not tested
//TODO most of the new metacharacters and escapes are not tested

SCENARIO( "TokenStream", "[TokenStream]" ) 
{
    SECTION("Basic token extraction")
    {
        std::string string = "hello";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, 'h'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, 'e'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, 'l'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, 'l'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, 'o'});
                }
            }
        }
    }

    SECTION("Peeking ahead in the token stream")
    {
        std::string string = "123";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '1'});
                    tokenStream.get();
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '2'});
                    tokenStream.get();
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '3'});
                    tokenStream.get();
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                    tokenStream.get();
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                }
            }
        }
    }

    SECTION("Tokenstream from escape sequence '\\\\'")
    {
        std::string string = "\\\\";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '\\'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, '\\'});
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                    REQUIRE(tokenStream.get() == Token{TokenType::eEOF, ' '});
                }
            }
        }
    }


    SECTION("Tokenstream from escape sequence '\\('")
    {
        std::string string = "\\(";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '('});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, '('});
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                    REQUIRE(tokenStream.get() == Token{TokenType::eEOF, ' '});
                }
            }
        }
    }

    SECTION("Tokenstream from escape sequence '\\)'")
    {
        std::string string = "\\)";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, ')'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, ')'});
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                    REQUIRE(tokenStream.get() == Token{TokenType::eEOF, ' '});
                }
            }
        }
    }

    SECTION("Tokenstream from escape sequence '\\|'")
    {
        std::string string = "\\|";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '|'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, '|'});
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                    REQUIRE(tokenStream.get() == Token{TokenType::eEOF, ' '});
                }
            }
        }
    }

    SECTION("Tokenstream from escape sequence '\\*'")
    {
        std::string string = "\\*";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '*'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, '*'});
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                    REQUIRE(tokenStream.get() == Token{TokenType::eEOF, ' '});
                }
            }
        }
    }

    SECTION("Tokenstream from escape sequence '\\+'")
    {
        std::string string = "\\+";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '+'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, '+'});
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                    REQUIRE(tokenStream.get() == Token{TokenType::eEOF, ' '});
                }
            }
        }
    }

    SECTION("Tokenstream from escape sequence '\\?'")
    {
        std::string string = "\\?";
        GIVEN( "a string: " << string) 
        {
            WHEN( "the string is tokenized")
            {
                auto tokenStream = TokenStream{string};

                THEN( "the correct tokens can be extracted" ) 
                {
                    REQUIRE(tokenStream.peek() == Token{TokenType::eSymbol, '?'});
                    REQUIRE(tokenStream.get() == Token{TokenType::eSymbol, '?'});
                    REQUIRE(tokenStream.peek() == Token{TokenType::eEOF, ' '});
                    REQUIRE(tokenStream.get() == Token{TokenType::eEOF, ' '});
                }
            }
        }
    }

    SECTION("Tokenstream from illegal escape sequence throws")
    {
        GIVEN( "a string containing escape character followed by EOF") 
        {
            std::string string = "\\";
            WHEN( "the string is tokenized")
            {
                THEN( "the correct tokens can be extracted") 
                {
                    REQUIRE_THROWS_AS(TokenStream{string}, std::runtime_error);
                }
            }
        }

        GIVEN( "a string containing escape character followed an illegal character") 
        {
            std::string string = "\\1";
            WHEN( "the string is tokenized")
            {
                THEN( "the correct tokens can be extracted") 
                {
                    REQUIRE_THROWS_AS(TokenStream{string}, std::runtime_error);
                }
            }
        }
    }
}

} //namespace
} //namespace regex 