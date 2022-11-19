#include "Parser.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

namespace regex::parser
{

NodePtr buildSubtree(CharacterGroup& Segments)
{
    NodePtr out = std::make_unique<ast::Epsilon>();

    for(auto it = Segments.rbegin(); it != Segments.rend(); ++it) {
        NodePtr segment = std::make_unique<ast::CharacterRange>(it->first, it->second);
        out = std::make_unique<ast::Alternative>(segment, out);
    }
    return out;
}

Parser::Parser(const std::string& regex)
: mCurser{regex.begin()}
, mBegin {regex.begin()}
, mEnd {regex.end()}
{}

AST Parser::parse()
{
    NodePtr root;
    parse<tags::RegexTag>(root); //TODO handle false

    if(mCurser != mEnd)
    {
        HandleUnexpected();  //TODO this if should look at return from parse.
    }

    return AST(root);
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
    return (mCurser++ == mEnd ? false : true);  // TODO use a CP over 0x10FFFF to signal EOF
}

bool Parser::parse(tags::RegexTag, NodePtr& astNode)
{
    if(parse<tags::AnchorStartOfStringTag>())
    {
        error("Anchors not supported ");
    }

    parse<tags::ExpressionTag>(astNode);

    //TODO this should check for EOF as the last entry
    return true;
}

bool Parser::parse(tags::ExpressionTag, NodePtr& astNode)
{
    auto subexpression = NodePtr();
    auto expression = NodePtr();

    if(!parse<tags::SubexpressionTag>(subexpression))
    {
        subexpression = std::make_unique<ast::Epsilon>();
    }

    if(parse<tags::AlternativeTag>())
    {
        if(!parse<tags::ExpressionTag>(expression))
        {
            expression = std::make_unique<ast::Epsilon>();
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
        expression = std::make_unique<ast::Epsilon>();
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

    if(parse<tags::CharacterClassTypeTag>(astNode))
    {
        return true;
    }

    CodePoint cp;
    if(parse<tags::CharacterTag>(astNode, cp))
    {
        return true;
    }

    return false;
}

bool Parser::parse(tags::CharacterClassTypeTag, NodePtr& node)
{
    CharacterGroup characterGroup;

    if(parse<tags::CharacterClassTag>(characterGroup))
    {
        node = buildSubtree(characterGroup);
        return true;
    }

    if(parse<tags::ShorthandCharacterClassTag>(characterGroup))
    {
        node = buildSubtree(characterGroup);
        return true;
    }

    return false;
}

bool Parser::parse(tags::CharacterClassTag, CharacterGroup& group)
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

    if(parse<tags::CharacterClassCloseTag>())
    {
        group.emplace_back(']', ']');
    }

    while(parse<tags::CharacterClassItemTag>(group))
    {}

    if(!parse<tags::CharacterClassCloseTag>())
    {
        error("Character class missing closing bracket");
    }

    if(negated)
    {
        negate(group);
    }
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

bool Parser::parse(tags::CharacterClassItemTag, CharacterGroup& group)
{
    if(parse<tags::ShorthandCharacterClassTag>(group))
    {
        return true;
    }

    if(parse<tags::CharacterRangeTag>(group))
    {
        return true;
    }

    CodePoint cp;
    NodePtr node;
    if(parse<tags::CharacterClassCharacterTag>(node, cp))
    {
        group.emplace_back(cp, cp);
        return true;
    }

    return false;
}

bool Parser::parse(tags::CharacterClassCharacterTag, NodePtr& node, CodePoint& cp)
{
    if(parse<tags::EscapedCharacterTag>(node, cp))
    {
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

    node = std::make_unique<ast::Character>(cp);
    return true;
}

bool Parser::parse(tags::CharacterRangeTag, CharacterGroup& group)
{
    NodePtr start;
    NodePtr end;
    CodePoint startCP; 
    CodePoint endCP; 
    
    if(!parse<tags::CharacterClassCharacterTag>(start, startCP))  //TOOD this should not use node anymore.
    {
        return false;
    }

    if(!parse<tags::CharacterRangeSeparatorTag>())
    {
        return false;
    }

    if(!parse<tags::CharacterClassCharacterTag>(end, endCP))
    {
        return false;
    }

    if(startCP > endCP)
    {
        error("Character range is out of order");
    }

    group.emplace_back(startCP, endCP);
    return true;
}

bool Parser::parse(tags::CharacterRangeSeparatorTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '-');
}

bool Parser::parse(tags::ShorthandCharacterClassTag, CharacterGroup& group)
{
    if(parse<tags::ShorthandCharacterClassWordTag>(group))
    {
        return true;
    }

    if(parse<tags::ShorthandCharacterClassWordNegatedTag>(group))
    {
        return true;
    }

    if(parse<tags::ShorthandCharacterClassDigitTag>(group))
    {
        return true;
    }

    if(parse<tags::ShorthandCharacterClassDigitNegatedTag>(group))
    {
        return true;
    }

    if(parse<tags::ShorthandCharacterClassWhitespaceTag>(group))
    {
        return true;
    }

    if(parse<tags::ShorthandCharacterClassWhitespaceNegatedTag>(group))
    {
        return true;
    }

    return false;
}

bool Parser::parse(tags::ShorthandCharacterClassWordTag, CharacterGroup& group)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }
    if(!get(cp) || cp != 'w')
    {
        return false;
    }
    group = {{0x30, 0x39}, {0x41, 0x5A}, {0x5F, 0x5F}, {0x61, 0x7A}};
    return true;
}

bool Parser::parse(tags::ShorthandCharacterClassWordNegatedTag, CharacterGroup& group)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }
    if(!get(cp) || cp != 'W')
    {
        return false;
    }
    group = {{kCodePointMin, 0x2F}, {0x3A, 0x40}, {0x5B, 0x5E}, {0x60, 0x60}, {0x7B, kCodePointMax}};
    return true;
}

bool Parser::parse(tags::ShorthandCharacterClassDigitTag, CharacterGroup& group)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }
    if(!get(cp) || cp != 'd')
    {
        return false;
    }
    group = {{0x30, 0x39}};
    return true;
}

bool Parser::parse(tags::ShorthandCharacterClassDigitNegatedTag, CharacterGroup& group)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }
    if(!get(cp) || cp != 'D')
    {
        return false;
    }
    group = {{kCodePointMin, 0x2F}, {0x3A, kCodePointMax}};
    return true;
}

bool Parser::parse(tags::ShorthandCharacterClassWhitespaceTag, CharacterGroup& group)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }
    if(!get(cp) || cp != 's')
    {
        return false;
    }
    group = {{0x09, 0x0D}, {0x20, 0x20}};
    return true;
}

bool Parser::parse(tags::ShorthandCharacterClassWhitespaceNegatedTag, CharacterGroup& group)
{
    CodePoint cp;
    if(!get(cp) || cp != '\\')
    {
        return false;
    }
    if(!get(cp) || cp != 'S')
    {
        return false;
    }
    group = {{kCodePointMin, 0x08}, {0x0E, 0x1F}, {0x21, kCodePointMax}};
    return true;
}

bool Parser::parse(tags::AnyCharacterTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '.');
}

bool Parser::parse(tags::EscapedCharacterTag, NodePtr& node, CodePoint& cp)
{
    if(!get(cp) || cp != '\\')
    {
        return false;
    }

    if(!get(cp))
    {
        error("Pattern may not end with a trailing backslash");
    }

    switch (cp)
    {
        case '^':
        case '$':
        case '*':
        case '+':
        case '?':
        case '.':
        case '|':
        case '(':
        case ')':
        case '[':
        case ']':
        case '-':
        {
            node = std::make_unique<ast::Character>(cp);
            return true;
        }

        case 'w':
        case 'W':
        case 'd':
        case 'D':
        case 's':
        case 'S':
        {
            return false;
        }

        case 'n':
        {
            cp = '\n';
            node = std::make_unique<ast::Character>('\n');
            return true;
        } 
        case 'f':
        {
            cp = '\f';
            node = std::make_unique<ast::Character>('\f');
            return true;
        }
        case 'r':
        {
            cp = '\r';
            node = std::make_unique<ast::Character>('\r');
            return true;
        }
        case 't':
        {
            cp = '\t';
            node = std::make_unique<ast::Character>('\t');
            return true;
        }
        case 'v':
        {
            cp = '\v';
            node = std::make_unique<ast::Character>('\v');
            return true;
        }
        case 'a':
        {
            cp = '\a';
            node = std::make_unique<ast::Character>('\a');
            return true;
        }
        case '\\':
        {
            cp = '\\';
            node = std::make_unique<ast::Character>('\\');
            return true;
        }
        
        case 'u':
        {
            return parse<tags::UnicodeTag>(node, cp);
        }

        default:
        {
            error("This token has no special meaning and has thus been rendered erroneous");
            return false;
        }
    }
}

bool Parser::parse(tags::CharacterTag, NodePtr& node, CodePoint& cp)
{

    // This is a bit messy. Not sure how to account for this in
    // the grammar. A '{' can be either treated as a literal or
    // the start of a ranged quantifier.
    auto dummy1 = NodePtr();
    auto dummy2 = NodePtr();
    if(parse<tags::RangeQuantifierTag>(dummy1,dummy2))
    {
        return false;
    }

    if(parse<tags::EscapedCharacterTag>(node, cp))
    {
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

    node = std::make_unique<ast::Character>(cp);
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
    if(parse<tags::ZeroOrMoreQuantifierTag>(astNode, inner))
    {
        return true;
    }

    if(parse<tags::OneOrMoreQuantifierTag>(astNode, inner))
    {
        return true;
    }

    if(parse<tags::ZeroOrOneQuantifierTag>(astNode, inner))
    {
        return true;
    }

    if(parse<tags::RangeQuantifierTag>(astNode, inner))
    {
        return true;
    }

    return false;
}

bool Parser::parse(tags::LazyModifierTag)
{
    CodePoint cp; 
    return (get(cp) == true && cp == '?');
}

bool Parser::parse(tags::ZeroOrMoreQuantifierTag, NodePtr& astNode, NodePtr& inner)
{
    CodePoint cp; 
    if (get(cp) == true && cp == '*')
    {
        astNode = std::make_unique<ast::Quantifier>(inner, 0, 0, false);
        return true;
    }
    return false;
}

bool Parser::parse(tags::OneOrMoreQuantifierTag, NodePtr& astNode, NodePtr& inner)
{
    CodePoint cp; 
    if (get(cp) == true && cp == '+')
    {
        astNode = std::make_unique<ast::Quantifier>(inner, 1, 0, false);
        return true;
    }
    return false;
}

bool Parser::parse(tags::ZeroOrOneQuantifierTag, NodePtr& astNode, NodePtr& inner)
{
    CodePoint cp; 
    if (get(cp) == true && cp == '?')
    {
        astNode = std::make_unique<ast::Quantifier>(inner, 0, 1, true);
        return true;
    }
    return false;
}

bool Parser::parse(tags::RangeQuantifierTag, NodePtr& astNode, NodePtr& inner)
{
    uint64_t min = 0;
    uint64_t max = 0;
    bool isMaxBounded = true;
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
        if(!parse<tags::RangeQuantifierUpperBoundTag>(max, maxOverflow))
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

    astNode = std::make_unique<ast::Quantifier>(inner, min, max, isMaxBounded);
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

inline int hex2int(char ch)
{
    int val;
    if (ch >= '0' && ch <= '9')
        val = ch - '0';
    if (ch >= 'A' && ch <= 'F')
        val = ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        val = ch - 'a' + 10;
    return val;
}

bool Parser::parse(tags::UnicodeTag, NodePtr& node, CodePoint& cp)
{
    constexpr auto kNumDigits = 6u;
    CodePoint digit;
    
    
    cp = 0;
    for(int i=0; i < kNumDigits; i++)
    {
        if(!get(digit) || !isxdigit(digit))
        {
            error("The Unicode codepoint is incomplete");
        }
        
        cp = cp << 4;
        cp |= hex2int(digit);
    }

    if(cp > 0x0010FFFF)
    {
        error("The Unicode codepoint invalid");
    }

    node = std::make_unique<ast::Character>(cp);
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

} // namespace regex::parser
