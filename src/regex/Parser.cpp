
#include "Parser.hpp"
#include <stdexcept>
#include <limits>
#include <vector>

namespace regex::parser
{

Parser::Parser(AST& ast, const std::string& regex)
: mCurser{regex.begin()}
, mBegin {regex.begin()}
, mEnd {regex.end()}
{
    parse<tags::RegexTag>(ast.root);

    if(mCurser != mEnd)
    {
        HandleUnexpected();
    }
}

void Parser::HandleUnexpected()
{
    if(parse<tags::GroupCloseTag>())
    {
        error("Unmatched parenthesis");
    }
    
    auto dummy1 = NodePtr();
    auto dummy2 = NodePtr();
    if(parse<tags::QuantifierTypeTag>(dummy1, dummy2))
    {
        error("The preceding token is not quantifiable");
    }

    error("Unknown parse error");
}

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

bool Parser::parse(tags::RegexTag, NodePtr& astNode)
{
    if(parse<tags::AnchorStartOfStringTag>())
    {
        error("Anchors not supported ");
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
            error("missing 2nd Alternative "); //TODO: this is valid. can be epsilon
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
    auto subexpr = NodePtr();
    auto next = NodePtr();

    if(!parse<tags::SubexpressionItemTag>(subexpr))
    {
        return false;
    }

    while(parse<tags::SubexpressionItemTag>(next))
    {
        subexpr = std::make_unique<ast::Concatenation>(subexpr, next);
    }

    std::swap(astNode, subexpr);
    return true;
}

bool Parser::parse(tags::SubexpressionItemTag, NodePtr& astNode)
{
    if(parse<tags::BackreferenceTag>(astNode))
    {
        error("Backreferences are not supported");
    }

    if(parse<tags::AnchorTag>(astNode))
    {
        error("Anchors are not supported ");
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

bool Parser::parse(tags::AnchorStartOfStringTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '^');
}

bool Parser::parse(tags::AnchorEndOfStringTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '$');
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
        error("back reference is too large");
    }

    return true;
}

bool Parser::parse(tags::BackreferenceStartTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '\\');
}

bool Parser::parse(tags::AlternativeTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '|');
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
    if(parse<tags::AnyCharacterTag>())
    {
        astNode = std::make_unique<ast::Any>();
        return true;
    }

    if(parse<tags::CharacterClassTag>(astNode))
    {
        return true;
    }

    if(parse<tags::ShorthandCharacterClassTag>(astNode))
    {
        return true;
    }

    CodePoint cp;
    bool escaped = false;
    if(parse<tags::CharacterTag>(cp, escaped))
    {
        astNode = std::make_unique<ast::Character>(cp, escaped);
        return true;
    }

    return false;
}

bool Parser::parse(tags::ShorthandCharacterClassTag, NodePtr& node)
{
    if(parse<tags::ShorthandCharacterClassWordTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyWord>();
        return true;
    }

    if(parse<tags::ShorthandCharacterClassWordNegatedTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyWordInverted>();
        return true;
    }

    if(parse<tags::ShorthandCharacterClassDigitTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyDecimalDigit>();
        return true;
    }

    if(parse<tags::ShorthandCharacterClassDigitNegatedTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyDecimalDigitInverted>();
        return true;
    }

    if(parse<tags::ShorthandCharacterClassWhitespaceTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyWhitespace>();
        return true;
    }

    if(parse<tags::ShorthandCharacterClassWhitespaceNegatedTag>())
    {
        node = std::make_unique<ast::CharacterClassAnyWhitespaceInverted>();
        return true;
    }
    

    return false;
}   

bool Parser::parse(tags::ShorthandCharacterClassWordTag)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'w');
}

bool Parser::parse(tags::ShorthandCharacterClassWordNegatedTag)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'W');
}

bool Parser::parse(tags::ShorthandCharacterClassDigitTag)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'd');
}

bool Parser::parse(tags::ShorthandCharacterClassDigitNegatedTag)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'D');
}

bool Parser::parse(tags::ShorthandCharacterClassWhitespaceTag)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 's');
}

bool Parser::parse(tags::ShorthandCharacterClassWhitespaceNegatedTag)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }

    return (get(cp) == true && cp == 'S');
}

bool Parser::parse(tags::CharacterClassTag, NodePtr& node)
{
    bool negated = false;

    if(!parse<tags::CharacterClassOpenTag>())
    {
        return false;
    }

    if(parse<tags::CharacterClassNegativeModifierTag>())
    {
        negated = true;
    }

    auto groupItemPtr = NodePtr();
    std::vector<NodePtr> groupItems;

    if(parse<tags::CharacterClassCloseTag>())
    {
        groupItemPtr = std::make_unique<ast::Character>(']', false);
        groupItems.emplace_back(groupItemPtr.release());
    }

    while(parse<tags::CharacterClassItemTag>(groupItemPtr))
    {
        groupItems.emplace_back(groupItemPtr.release());
    }

    if(!parse<tags::CharacterClassCloseTag>())
    {
        error("Character class missing closing bracket");
    }

    node = std::make_unique<ast::CharacterClass>(std::move(groupItems), negated);

    return true;
}

bool Parser::parse(tags::CharacterClassOpenTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '[');
}

bool Parser::parse(tags::CharacterClassCloseTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == ']');
}

bool Parser::parse(tags::CharacterClassNegativeModifierTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '^');
}

bool Parser::parse(tags::CharacterClassItemTag, NodePtr& node)
{
    if(parse<tags::ShorthandCharacterClassTag>(node))
    {
        return true;
    }

    if(parse<tags::CharacterRangeTag>(node))
    {
        return true;
    }

    CodePoint cp;
    bool escaped = false;
    if(parse<tags::CharacterClassCharacterTag>(cp, escaped))
    {
        node = std::make_unique<ast::Character>(cp, escaped);
        return true;
    }

    return false;
}

bool Parser::parse(tags::CharacterClassEscapedCharacterTag, CodePoint& cp)
{
    if(!get(cp) || cp != '\\')
    {
        return false;
    }

    if(!get(cp))
    {
        error("Pattern may not end with a trailing backslash");
    }

    // escaped meta-characters
    if(cp == '-' ||    
       cp == ']' ||
       cp == '[' ||
       cp == '^'
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

bool Parser::parse(tags::CharacterClassCharacterTag, CodePoint& cp, bool& escaped)
{
    if(parse<tags::CharacterClassEscapedCharacterTag>(cp))
    {
        escaped = true;
        return true;
    }

    if(!get(cp))
    {
        return false;
    }

    // ignore meta-characters
    // While '-' and '[' and '^' are meta-characters, they may be 
    // interpreted literally depending on their position in the
    // character class. Hence, they're excluded here.
    if(cp == ']')
    {
        return false;
    }

    return true;
}

bool Parser::parse(tags::CharacterRangeTag, NodePtr& node)
{
    CodePoint start;
    bool startIsEscaped = false;

    CodePoint end;
    bool endIsEscaped = false;

    if(!parse<tags::CharacterClassCharacterTag>(start, startIsEscaped))
    {
        return false;
    }

    if(!parse<tags::CharacterRangeSeparatorTag>())
    {
        return false;
    }

    if(!parse<tags::CharacterClassCharacterTag>(end, endIsEscaped))
    {
        return false;
    }

    if(start > end)
    {
        error("Character range is out of order");
    }

    NodePtr nodeStart = 
        std::make_unique<ast::Character>(start, startIsEscaped);

    NodePtr nodeEnd = 
        std::make_unique<ast::Character>(end, endIsEscaped);

    node = std::make_unique<ast::CodePointRange>(
        std::move(nodeStart),
        std::move(nodeEnd));

    return true;
}

bool Parser::parse(tags::CharacterRangeSeparatorTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '-');
}

bool Parser::parse(tags::AnyCharacterTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '.');
}

bool Parser::parse(tags::EscapedCharacterTag, CodePoint& cp)
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

bool Parser::parse(tags::CharacterTag, CodePoint& cp, bool& escaped)
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

    if(parse<tags::EscapedCharacterTag>(cp))
    {
        escaped = true;
        return true;
    }

    if(!get(cp))
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

    if(parse<tags::RangeSeparatorTag>())
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
    return parse<tags::IntegerTag>(min, overflow);
}

bool Parser::parse(tags::RangeQuantifierUpperBoundTag, uint64_t& max, bool& overflow)
{
    return parse<tags::IntegerTag>(max, overflow);
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

bool Parser::parse(tags::RangeSeparatorTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == ',');
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

}



