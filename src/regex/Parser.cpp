
#include "Parser.hpp"
#include <stdexcept>
#include <iostream>
#include <limits>
#include <vector>

namespace regex::parser
{

int Parser::pos() const
{
    return std::distance(mBegin, mCurser);
}

void Parser::error(const std::string& msg) const
{
    throw std::runtime_error("Error at position " + std::to_string(pos()) + ". Message: " + msg);
}

bool Parser::get(CodePoint & value)
{
    value = *mCurser;
    return (mCurser++ == mEnd ? false : true);
}

Parser::Parser(AST& ast, const std::string& regex)
: mCurser{regex.begin()}
, mBegin {regex.begin()}
, mEnd {regex.end()}
{
    parse<tags::RegexTag>(ast.root);

    if(mCurser != mEnd)
    {
        if(parse<tags::GroupCloseTag>())
        {
            error("Unmatched parenthesis");
        }
        
        if(parse<tags::ZeroOrMoreQuantifierTag>())
        {
            error("The preceding token is not quantifiable");
        }
        
        if(parse<tags::OneOrMoreQuantifierTag>())
        {
            error("The preceding token is not quantifiable");
        }
        
        if(parse<tags::ZeroOrOneQuantifierTag>())
        {
            error("The preceding token is not quantifiable");
        }
        
        uint64_t min = 0;
        uint64_t max = 0;
        bool isMaxBounded = false;
        if(parse<tags::RangeQuantifierTag>(min, max, isMaxBounded))
        {
            error("The preceding token is not quantifiable");
        }



        error("Unknown parse error");

    }
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
        //TODO: this is valid. can be epsilon
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
    auto lhs = NodePtr();
    auto rhs = NodePtr();

    if(!parse<tags::SubexpressionItemTag>(lhs))
    {
        return false;
    }

    while(parse<tags::SubexpressionItemTag>(rhs))
    {
        lhs = std::make_unique<ast::Concatenation>(lhs, rhs);
    }

    std::swap(astNode, lhs);
    return true;

}

bool Parser::parse(tags::SubexpressionItemTag, NodePtr& astNode)
{
    if(parse<tags::BackreferenceTag>(astNode))
    {
        error("Backreferences are not supported"); //TODO what to do with this?
    }

    if(parse<tags::AnchorTag>(astNode))
    {
        error("Anchors are not supported "); //TODO what to do with this?
    }

    if(parse<tags::MatchTag>(astNode))
    {
        return true;
    }

    if(parse<tags::GroupTag>(astNode))
    {
        return true;
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

bool Parser::parse(tags::CharacterClassAnyWordTag)
{
    CodePoint cp;
    if(!get(cp))
    {
        return false;
    }

    if(cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'w');
}

bool Parser::parse(tags::CharacterClassAnyWordInvertedTag)
{
    CodePoint cp;
    if(!get(cp))
    {
        return false;
    }

    if(cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'W');
}

bool Parser::parse(tags::CharacterClassAnyDecimalDigitTag)
{
    CodePoint cp;
    if(!get(cp))
    {
        return false;
    }

    if(cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'd');
}

bool Parser::parse(tags::CharacterClassAnyDecimalDigitInvertedTag)
{
    CodePoint cp;
    if(!get(cp))
    {
        return false;
    }

    if(cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'D');
}

bool Parser::parse(tags::CharacterClassWhitespaceTag)
{
    CodePoint cp;
    if(!get(cp))
    {
        return false;
    }

    if(cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 's');
}

bool Parser::parse(tags::CharacterClassWhitespaceInvertedTag)
{
    CodePoint cp;
    if(!get(cp))
    {
        return false;
    }

    if(cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'S');
}

bool Parser::parse(tags::BackreferenceStartTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '\\');
}

bool Parser::parse(tags::MatchCharacterClassTag, NodePtr& node)
{
    if(parse<tags::CharacterGroupTag>(node))
    {
        return true;
    }

    if(parse<tags::CharacterClassTag>(node))
    {
        return true;
    }

    return false;
}

bool Parser::parse(tags::CharacterGroupTag, NodePtr& node)
{
    bool negated = false;

    if(!parse<tags::CharacterGroupOpenTag>())
    {
        return false;
    }

    if(parse<tags::CharacterGroupNegativeModifierTag>())
    {
        negated = true;
    }

    auto groupItemPtr = NodePtr();
    std::vector<NodePtr> groupItems;

    while(parse<tags::CharacterGroupItemTag>(groupItemPtr))
    {
        groupItems.emplace_back(groupItemPtr.release());
    }

    if(!parse<tags::CharacterGroupCloseTag>())
    {
        error("Character class missing closing bracket");
    }

    node = std::make_unique<ast::CharacterClass>(std::move(groupItems), negated);

    return true;
}

bool Parser::parse(tags::CharacterGroupItemTag, NodePtr& node)
{
    if(parse<tags::CharacterClassTag>(node))
    {
        return true;
    }

    if(parse<tags::CharacterRangeTag>(node))
    {
        return true;
    }

    CodePoint cp;
    if(parse<tags::MatchCharacterEscapeTag>(cp))
    {
        node = std::make_unique<ast::CharacterFromEscape>(cp);
        return true;
    }

    if(parse<tags::MatchCharacterTag>(cp))
    {
        node = std::make_unique<ast::CharacterLiteral>(cp);
        return true;
    }

    return false;
}

bool Parser::parse(tags::CharacterRangeTag, NodePtr& node)
{
    CodePoint start;
    CodePoint end;

    if(!parse<tags::MatchCharacterTag>(start))
    {
        return false;
    }

    if(!parse<tags::CharacterRangeHyphenTag>())
    {
        return false;
    }

    if(!parse<tags::MatchCharacterTag>(end))
    {
        return false;
    }

    if(start > end)
    {
        error("Character range is out of order");
    }

    node = std::make_unique<ast::CodePointRange>(CodePointInterval(start,end));

    return false;
}

bool Parser::parse(tags::CharacterRangeHyphenTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '-');
}

bool Parser::parse(tags::CharacterClassTag, NodePtr& node)
{
    if(parse<tags::CharacterClassAnyWordTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyWord>();
        return true;
    }

    if(parse<tags::CharacterClassAnyWordInvertedTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyWordInverted>();
        return true;
    }

    if(parse<tags::CharacterClassAnyDecimalDigitTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyDecimalDigit>();
        return true;
    }

    if(parse<tags::CharacterClassAnyDecimalDigitInvertedTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyDecimalDigitInverted>();
        return true;
    }

    if(parse<tags::CharacterClassWhitespaceTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyWhitespace>();
        return true;
    }

    if(parse<tags::CharacterClassWhitespaceInvertedTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyWhitespaceInverted>();
        return true;
    }
    

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
       cp == '|' || 
       cp == '(' || 
       cp == ')' || 
       cp == '[' || 
       cp == ']'
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
    return false; // to supress compiler warning
}

bool Parser::parse(tags::MatchCharacterTag, CodePoint& cp)
{

    // This is a bit messy. Not sure how to account for this in
    // the grammar. A '{' can be either treated as a literal or
    // the start of a ranged quantifier.
    uint64_t min = 0;
    uint64_t max = 0;
    bool isMaxBounded = false;
    if(parse<tags::RangeQuantifierTag>(min, max, isMaxBounded))
    {
        return false;
    }

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
       cp == ']' || 
       cp == '|'
    )
    {
        return false;
    }

    return true;
}

bool Parser::parse(tags::QuantifierTag, NodePtr& astNode, NodePtr& inner)
{
    if(!parse<tags::QuantifierTypeTag>(astNode, inner))
    {
        return false;
    }

    if(parse<tags::LazyModifierTag>())
    {
        error("Lazy modifier is not supported ");
    }

    return true;
}

bool Parser::parse(tags::QuantifierTypeTag, NodePtr& astNode, NodePtr& inner)
{
    if(parse<tags::ZeroOrMoreQuantifierTag>())
    {
        astNode = std::make_unique<ast::KleeneStar>(inner);
        return true;
    }

    if(parse<tags::OneOrMoreQuantifierTag>())
    {
        astNode = std::make_unique<ast::KleenePlus>(inner);
        return true;
    }

    if(parse<tags::ZeroOrOneQuantifierTag>())
    {
        astNode = std::make_unique<ast::Optional>(inner);
        return true;
    }

    uint64_t min = 0;
    uint64_t max = 0;
    bool isMaxBounded = false;

    if(parse<tags::RangeQuantifierTag>(min, max, isMaxBounded))
    {
        astNode = std::make_unique<ast::RangeQuantifier>(inner, min, max, isMaxBounded);
        return true;
    }

    return false;
}

bool Parser::parse(tags::RangeQuantifierTag, uint64_t& min, uint64_t& max, bool& isMaxBounded)
{

    bool minOverflow = false;
    bool maxOverflow = false;

    if(!parse<tags::RangeOpenTag>())
    {
        return false;
    }

    if(!parse<tags::RangeQuantifierLowerBoundTag>(min, minOverflow))
    {
        return false;
    }

    max = min;

    if(parse<tags::RangeCommaDelimiterTag>())
    {
        if(parse<tags::RangeQuantifierUpperBoundTag>(max, maxOverflow))
        {
            isMaxBounded = true;
        }
        else
        {
            isMaxBounded = false;
        }
    }

    if(!parse<tags::RangeCloseTag>())
    {
        return false;
    }

    if(minOverflow)
    {
        error("Lower bound on ranged quantifier too large");
    }

    if(maxOverflow)
    {
        error("Upper bound on ranged quantifier too large");
    }

    if(min > max && isMaxBounded)
    {
        error("The quantifier range is out of order");
    }

    return true;
}

bool Parser::parse(tags::RangeQuantifierLowerBoundTag, uint64_t& min, bool& overflow)
{
    if(parse<tags::IntegerTag>(min, overflow))
    {
        return true;
    }
    return false;
}

bool Parser::parse(tags::RangeQuantifierUpperBoundTag, uint64_t& max, bool& overflow)
{
    if(parse<tags::IntegerTag>(max, overflow))
    {
        return true;
    }
    return false;
}

bool Parser::parse(tags::IntegerTag, uint64_t& integer, bool& overflow)
{
    integer = 0;
    overflow = false;

    uint8_t digit; 

    if(!parse<tags::DigitTag>(digit))
    {
        return false;
    }
    integer = digit;

    while(parse<tags::DigitTag>(digit))
    {
        integer *= 10;
        integer += digit;

        if(integer > std::numeric_limits<uint32_t>::max())
        {
            overflow = true;
        }
    }

    return true;
}

bool Parser::parse(tags::DigitTag, uint8_t& digit)
{
    CodePoint cp; 
    if(get(cp) == true && isdigit(cp))
    {
        digit = cp - '0';
        return true;
    }
    return false;
}

bool Parser::parse(tags::CharacterGroupNegativeModifierTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '^');
}

bool Parser::parse(tags::LazyModifierTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '?');
}

bool Parser::parse(tags::ZeroOrMoreQuantifierTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '*');
}

bool Parser::parse(tags::OneOrMoreQuantifierTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '+');
}

bool Parser::parse(tags::ZeroOrOneQuantifierTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '?');
}

bool Parser::parse(tags::GroupTag, NodePtr& astNode)
{
    auto expression = NodePtr();
    auto quantifier = NodePtr();



    if(!parse<tags::GroupOpenTag>())
    {
        return false;
    }

    if(parse<tags::GroupNonCapturingModifierTag>())
    {
        error("Non-capturing groups are the default. Capturing groups not supported");
    }

    if(!parse<tags::ExpressionTag>(expression))
    {
        // TODO: support empty group?
        error("Empty group is not supported");
    }

    if(!parse<tags::GroupCloseTag>())
    {
        error("Incomplete group structure");
    }

    if(parse<tags::QuantifierTag>(quantifier, expression))
    {
        std::swap(astNode, quantifier);   
    }
    else
    {
        std::swap(astNode, expression);
    }

    return true;
}

bool Parser::parse(tags::BackreferenceTag, NodePtr& astNode)
{
    uint64_t integer = 0;
    bool overflow = false;

    if(!parse<tags::BackreferenceStartTag>())
    {
        return false;
    }

    if(!parse<tags::IntegerTag>(integer, overflow))
    {
        return false;
    }
    
    if(overflow)
    {
        error("back reference is too large"); // TODO specify limit
    }

    return true;
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

bool Parser::parse(tags::RangeOpenTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '{');
}

bool Parser::parse(tags::RangeCloseTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '}');
}

bool Parser::parse(tags::RangeCommaDelimiterTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == ',');
}

bool Parser::parse(tags::CharacterGroupOpenTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '[');
}

bool Parser::parse(tags::CharacterGroupCloseTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == ']');
}


bool Parser::parse(tags::GroupOpenTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '(');
}

bool Parser::parse(tags::GroupCloseTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == ')');
}

bool Parser::parse(tags::GroupNonCapturingModifierTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '?');
}


}



