
#include "Parser.hpp"
#include <stdexcept>
#include <iostream>

namespace regex::parser
{

int Parser::pos() const
{
    return std::distance(mBegin, mCurser);
}

void Parser::error(const std::string& msg) const
{
    throw("error at position " + pos() + msg);
}

bool Parser::get(CodePoint & value)
{
    value = *mCurser;
    return (mCurser++ == mEnd ? false : true);
}

Parser::Parser(AST& ast, Utf8Iterator begin, Utf8Iterator end)
: mCurser{begin}
, mBegin {begin}
, mEnd {end}
{
    parse<tags::RegexTag>(ast.root);
}

bool Parser::parse(tags::RegexTag, NodePtr& astNode)
{
    if(parse<tags::StartOfStringAnchorTag>())
    {
        error("Anchors not supported "); //TODO what to do with this?
    }

    parse<tags::ExpressionTag>(astNode);
    return true;
}

bool Parser::parse(tags::ExpressionTag, NodePtr& astNode)
{
    auto subexpression = NodePtr();
    auto expression = NodePtr();

    if(!parse<tags::SubexpressionTag>(subexpression))
    {
        return false;
    }

    if(parse<tags::AlternativeTag>())
    {
        if(!parse<tags::ExpressionTag>(expression))
        {
            error("missing 2nd Alternative "); //TODO this is actually valid...
        }
        astNode = std::make_unique<ast::Alternative>(subexpression, expression);
    }
    else
    {
        std::swap(astNode, subexpression);
    }

    return true;
}

bool Parser::parse(tags::SubexpressionTag, NodePtr& astNode)
{
    auto subexpressionItem = NodePtr();
    auto subexpression = NodePtr();

    if(!parse<tags::SubexpressionItemTag>(subexpressionItem))
    {
        return false;
    }

    if(parse<tags::SubexpressionTag>(subexpression))
    {
        astNode = std::make_unique<ast::Concatenation>(subexpressionItem, subexpression);
    }
    else
    {
        std::swap(astNode, subexpressionItem);
    }

    return true;

}

bool Parser::parse(tags::SubexpressionItemTag, NodePtr& astNode)
{
    if(parse<tags::MatchTag>(astNode))
    {
        return true;
    }

    if(parse<tags::GroupTag>(astNode))
    {
        return true;
    }

    if(parse<tags::AnchorTag>(astNode))
    {
        error("Anchors are not supported "); //TODO what to do with this?
    }

    if(parse<tags::BackreferenceTag>(astNode))
    {
        error("Backreferences are not supported "); //TODO what to do with this?
    }

    return false;
}

bool Parser::parse(tags::MatchTag, NodePtr& astNode)
{
    auto matchItem = NodePtr();
    auto quantifier = NodePtr();

    if(!parse<tags::MatchItemTag>(matchItem))
    {
        return false;
    }

    if(parse<tags::QuantifierTag>(quantifier, matchItem))
    {
        std::swap(astNode, quantifier);   
    }
    else
    {
        std::swap(astNode, matchItem);
    }

    return true;
}


bool Parser::parse(tags::MatchItemTag, NodePtr& astNode)
{
    if(parse<tags::MatchAnyCharacterTag>())
    {
        astNode = std::make_unique<ast::Any>();
        return true;
    }

    if(parse<tags::MatchCharacterClassTag>(astNode))
    {
        return true;
    }

    CodePoint cp;
    if(parse<tags::MatchCharacterEscapeTag>(cp))
    {
        astNode = std::make_unique<ast::CharacterFromEscape>(cp);
        return true;
    }

    if(parse<tags::MatchCharacterTag>(cp))
    {
        astNode = std::make_unique<ast::CharacterLiteral>(cp);
        return true;
    }

    return false;
}

bool Parser::parse(tags::MatchAnyCharacterTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '.');
}

bool Parser::parse(tags::MatchCharacterClassTag, NodePtr&)
{
    return false;
}

bool Parser::parse(tags::MatchCharacterEscapeTag, CodePoint& cp)
{
    if(!get(cp))
    {
        return false;
    }

    if(cp != '\\')
    {
        return false;
    }

    if(!get(cp))
    {
        error("Pattern may not end with a trailing backslash");
    }

    // escaped meta-characters
    if(cp == '^' ||    
       cp == '$' || 
       cp == '*' || 
       cp == '+' || 
       cp == '?' || 
       cp == '.' || 
       cp == '(' || 
       cp == ')' || 
       cp == '[' || 
       cp == ']' ||
       cp == '$'
    )
    {
        return true;
    }

    // escaped characters with special meaning
    if(cp == 'n' ||    // newline
       cp == 'f' ||    // form feed
       cp == 'r' ||    // carriage return
       cp == 't' ||    // horizontal tab
       cp == 'v' ||    // vertical tab
       cp == 'a' ||    // bell
       cp == 'b' ||    // backspace
       cp == '\\'      // black slash
    )
    {
        return true;
    }

    // Ignore short-hand character classes
    if(cp == 'w' ||    
       cp == 'W' || 
       cp == 'd' || 
       cp == 'D' || 
       cp == 's' || 
       cp == 'S'
    )
    {
        return false;
    }

    error("This token has no special meaning and has thus been rendered erroneous");
    return false; // supress compiler warning
}

bool Parser::parse(tags::MatchCharacterTag, CodePoint& cp)
{
    if(!get(cp) || cp == '\\')
    {
        return false;
    }

    // ignore meta-characters
    if(cp == '^' ||    
       cp == '$' || 
       cp == '*' || 
       cp == '+' || 
       cp == '?' || 
       cp == '.' || 
       cp == '(' || 
       cp == ')' || 
       cp == '[' || 
       cp == ']'
    )
    {
        return false;
    }

    return true;
}


bool Parser::parse(tags::QuantifierTag, NodePtr& astNode, NodePtr& inner)
{
    //TODO
    return false;
}


bool Parser::parse(tags::GroupTag, NodePtr& astNode)
{
    return false;
}



bool Parser::parse(tags::BackreferenceTag, NodePtr& astNode)
{
    //TODO
    return false;
}

bool Parser::parse(tags::AnchorTag, NodePtr& astNode)
{

    if(parse<tags::AnchorWordBoundaryTag>())
    {
        return true;
    }

    if(parse<tags::AnchorNonWordBoundaryTag>())
    {
        return true;
    }

    if(parse<tags::AnchorStartOfStringOnlyTag>())
    {
        return true;
    }

    if(parse<tags::AnchorEndOfStringOnlyNotNewlineTag>())
    {
        return true;
    }

    if(parse<tags::AnchorEndOfStringOnlyTag>())
    {
        return true;
    }

    if(parse<tags::AnchorPreviousMatchEndTag>())
    {
        return true;
    }

    if(parse<tags::AnchorEndOfStringTag>())
    {
        return true;
    }

    return false;

}

bool Parser::parse(tags::StartOfStringAnchorTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '^');
}

bool Parser::parse(tags::AlternativeTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '|');
}

bool Parser::parse(tags::AnchorWordBoundaryTag)
{
    CodePoint cp1, cp2;
    return ((get(cp1) == true && cp1 == '\\') &&
            (get(cp2) == true && cp2 == 'b'));
}

bool Parser::parse(tags::AnchorNonWordBoundaryTag)
{
    CodePoint cp1, cp2;
    return ((get(cp1) == true && cp1 == '\\') &&
            (get(cp2) == true && cp2 == 'B'));
}

bool Parser::parse(tags::AnchorStartOfStringOnlyTag)
{
    CodePoint cp1, cp2;
    return ((get(cp1) == true && cp1 == '\\') &&
            (get(cp2) == true && cp2 == 'A'));
}

bool Parser::parse(tags::AnchorEndOfStringOnlyNotNewlineTag)
{
    CodePoint cp1, cp2;
    return ((get(cp1) == true && cp1 == '\\') &&
            (get(cp2) == true && cp2 == 'z'));
}

bool Parser::parse(tags::AnchorEndOfStringOnlyTag)
{
    CodePoint cp1, cp2;
    return ((get(cp1) == true && cp1 == '\\') &&
            (get(cp2) == true && cp2 == 'Z'));
}

bool Parser::parse(tags::AnchorPreviousMatchEndTag)
{
    CodePoint cp1, cp2;
    return ((get(cp1) == true && cp1 == '\\') &&
            (get(cp2) == true && cp2 == 'G'));
}

bool Parser::parse(tags::AnchorEndOfStringTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '$');
}

}



