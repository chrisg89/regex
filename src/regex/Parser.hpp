
#pragma once

#include "AST.hpp"
#include "Utf8Iterator.hpp"
#include "CodePoint.hpp"

namespace regex::parser
{

using ast::AST;
using ast::NodePtr;

namespace tags
{
    // Regex & expressions
    struct RegexTag{};
    struct ExpressionTag{};
    struct SubexpressionTag{};
    struct SubexpressionItemTag{};

    // Anchor TODO: some of these need to be renamed...
    struct AnchorTag{};
    struct AnchorStartOfStringTag{};
    struct AnchorEndOfStringTag{};
    struct AnchorWordBoundaryTag{};
    struct AnchorNonWordBoundaryTag{};
    struct AnchorStartOfStringOnlyTag{};
    struct AnchorEndOfStringOnlyNotNewlineTag{};
    struct AnchorEndOfStringOnlyTag{};
    struct AnchorPreviousMatchEndTag{};

    // Backreference
    struct BackreferenceTag{};
    struct BackreferenceStartTag{};

    // Alternation
    struct AlternativeTag{};

    // Group
    struct GroupTag{};
    struct GroupOpenTag{};
    struct GroupCloseTag{};
    struct GroupNonCapturingModifierTag{};

    // Match
    struct MatchTag{};
    struct MatchItemTag{};

    // Short hand character classes
    struct ShorthandCharacterClassTag{};       
    struct ShorthandCharacterClassWordTag{};        
    struct ShorthandCharacterClassWordNegatedTag{};  
    struct ShorthandCharacterClassDigitTag{};        
    struct ShorthandCharacterClassDigitNegatedTag{}; 
    struct ShorthandCharacterClassWhitespaceNegatedTag{};    
    struct ShorthandCharacterClassWhitespaceTag{};

    // Character class
    struct CharacterClassTag{};
    struct CharacterClassOpenTag{};
    struct CharacterClassCloseTag{};
    struct CharacterClassNegativeModifierTag{};
    struct CharacterClassItemTag{};
    struct CharacterClassCharacterTag{};

    // Character range
    struct CharacterRangeTag{};
    struct CharacterRangeSeparatorTag{};

    // Any character
    struct AnyCharacterTag{};

    // Escaped characters
    struct EscapedCharacterTag{};

    // Character
    struct CharacterTag{};

    // Quantifier
    struct QuantifierTag{};
    struct QuantifierTypeTag{};
    struct LazyModifierTag{};
    struct ZeroOrMoreQuantifierTag{};
    struct OneOrMoreQuantifierTag{};
    struct ZeroOrOneQuantifierTag{};
    struct RangeQuantifierTag{};
    struct RangeQuantifierLowerBoundTag{};
    struct RangeQuantifierUpperBoundTag{};
    struct RangeOpenTag{};
    struct RangeCloseTag{};
    struct RangeSeparatorTag{};

    // Numeric
    struct UnicodeTag{};
    struct DigitTag{};
    struct IntegerTag{};
}

class Parser
{
public:
    Parser(AST&, const std::string& );

private:

    Utf8Iterator mCurser;
    const Utf8Iterator mBegin;
    const Utf8Iterator mEnd;

    int pos() const;
    bool get(CodePoint & value);
    void error(const std::string& msg) const;

    void HandleUnexpected();

    // Wraps the specialized parsing routines with backtracking capability
    template <typename Tag, typename... Args>
    bool parse(Args&... args);

    // Regex & expressions
    bool parse(tags::RegexTag, NodePtr&);
    bool parse(tags::ExpressionTag, NodePtr&);
    bool parse(tags::SubexpressionTag, NodePtr&);
    bool parse(tags::SubexpressionItemTag, NodePtr&);

    // Anchor TODO: some of these need to be renamed...
    bool parse(tags::AnchorTag, NodePtr&);
    bool parse(tags::AnchorStartOfStringTag);
    bool parse(tags::AnchorEndOfStringTag);
    bool parse(tags::AnchorWordBoundaryTag);
    bool parse(tags::AnchorNonWordBoundaryTag);
    bool parse(tags::AnchorStartOfStringOnlyTag);
    bool parse(tags::AnchorEndOfStringOnlyNotNewlineTag);
    bool parse(tags::AnchorEndOfStringOnlyTag);
    bool parse(tags::AnchorPreviousMatchEndTag);

    // Backreference
    bool parse(tags::BackreferenceTag, NodePtr&);
    bool parse(tags::BackreferenceStartTag);

    // Alternation
    bool parse(tags::AlternativeTag);

    // Group 
    bool parse(tags::GroupTag, NodePtr&);
    bool parse(tags::GroupOpenTag);
    bool parse(tags::GroupCloseTag);
    bool parse(tags::GroupNonCapturingModifierTag);

    // Match
    bool parse(tags::MatchTag, NodePtr&);
    bool parse(tags::MatchItemTag, NodePtr&);

    // Short hand character classes
    bool parse(tags::ShorthandCharacterClassTag, NodePtr&);
    bool parse(tags::ShorthandCharacterClassWordTag);
    bool parse(tags::ShorthandCharacterClassWordNegatedTag);
    bool parse(tags::ShorthandCharacterClassDigitTag);
    bool parse(tags::ShorthandCharacterClassDigitNegatedTag);
    bool parse(tags::ShorthandCharacterClassWhitespaceTag);
    bool parse(tags::ShorthandCharacterClassWhitespaceNegatedTag);

    // Character class
    bool parse(tags::CharacterClassTag, NodePtr&);  
    bool parse(tags::CharacterClassOpenTag);
    bool parse(tags::CharacterClassCloseTag);
    bool parse(tags::CharacterClassNegativeModifierTag);
    bool parse(tags::CharacterClassItemTag, NodePtr&);
    bool parse(tags::CharacterClassCharacterTag, NodePtr&, CodePoint&);

    // Character range
    bool parse(tags::CharacterRangeTag, NodePtr&);
    bool parse(tags::CharacterRangeSeparatorTag);

    // Any character
    bool parse(tags::AnyCharacterTag);

    // Escaped characters
    bool parse(tags::EscapedCharacterTag, NodePtr&, CodePoint&);

    // Character
    bool parse(tags::CharacterTag, NodePtr&, CodePoint&);

    // Quantifier
    bool parse(tags::QuantifierTag, NodePtr&, NodePtr&);
    bool parse(tags::QuantifierTypeTag, NodePtr&, NodePtr&);
    bool parse(tags::LazyModifierTag);
    bool parse(tags::ZeroOrMoreQuantifierTag);
    bool parse(tags::OneOrMoreQuantifierTag);
    bool parse(tags::ZeroOrOneQuantifierTag);
    bool parse(tags::RangeQuantifierTag, uint64_t&, uint64_t&, bool&);
    bool parse(tags::RangeQuantifierLowerBoundTag, uint64_t&, bool&);
    bool parse(tags::RangeQuantifierUpperBoundTag, uint64_t&, bool&);
    bool parse(tags::RangeOpenTag);
    bool parse(tags::RangeCloseTag);
    bool parse(tags::RangeSeparatorTag);

    // Numeric
    bool parse(tags::UnicodeTag, NodePtr&, CodePoint&);
    bool parse(tags::DigitTag, uint8_t&);
    bool parse(tags::IntegerTag, uint64_t&, bool&);

};

} //namespace regex

#include "Parser-inl.hpp"