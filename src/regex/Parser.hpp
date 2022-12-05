#pragma once

#include "AST.hpp"
#include "CodePoint.hpp"
#include "Utf8Iterator.hpp"

namespace regex::parser
{

using ast::AST;
using ast::NodePtr;
using CharacterGroup = std::vector<CodePointInterval>;

namespace tags
{
    // Regex & expressions
    struct RegexTag{};
    struct ExpressionTag{};
    struct SubexpressionTag{};
    struct SubexpressionItemTag{};

    // Anchors
    struct AnchorTag{};
    struct AnchorStartOfStringTag{};
    struct AnchorEndOfStringTag{};

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

    // Character class
    struct CharacterClassTypeTag{};
    struct CharacterClassTag{};
    struct CharacterClassOpenTag{};
    struct CharacterClassCloseTag{};
    struct CharacterClassNegativeModifierTag{};
    struct CharacterClassItemTag{};
    struct CharacterClassCharacterTag{};

    // Character range
    struct CharacterRangeTag{};
    struct CharacterRangeSeparatorTag{};

    // Short hand character classes    
    struct ShorthandCharacterClassTag{};
    struct ShorthandCharacterClassWordTag{};        
    struct ShorthandCharacterClassWordNegatedTag{};  
    struct ShorthandCharacterClassDigitTag{};        
    struct ShorthandCharacterClassDigitNegatedTag{}; 
    struct ShorthandCharacterClassWhitespaceNegatedTag{};    
    struct ShorthandCharacterClassWhitespaceTag{};

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
    struct Unicode4DigitCodePointTag{};
    struct Unicode8DigitCodePointTag{};
    struct UnicodeTag{};
    struct DigitTag{};
    struct IntegerTag{};

    // EOF
    struct EOFTag{};
}

class Parser
{
public:
    explicit Parser(const std::string&);
    AST parse();

private:

    Utf8Iterator mCurser;
    const Utf8Iterator mBegin;
    const Utf8Iterator mEnd;

    int pos() const;
    CodePoint get();
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

    // Anchors
    bool parse(tags::AnchorTag, NodePtr&);
    bool parse(tags::AnchorStartOfStringTag);
    bool parse(tags::AnchorEndOfStringTag);

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

    // Character class
    bool parse(tags::CharacterClassTypeTag, NodePtr&);  
    bool parse(tags::CharacterClassTag, CharacterGroup&);  
    bool parse(tags::CharacterClassOpenTag);
    bool parse(tags::CharacterClassCloseTag);
    bool parse(tags::CharacterClassNegativeModifierTag);
    bool parse(tags::CharacterClassItemTag, CharacterGroup&);
    bool parse(tags::CharacterClassCharacterTag, CodePoint&);

    // Character range
    bool parse(tags::CharacterRangeTag, CharacterGroup&);
    bool parse(tags::CharacterRangeSeparatorTag);

    // Short hand character classes
    bool parse(tags::ShorthandCharacterClassTag, CharacterGroup&);
    bool parse(tags::ShorthandCharacterClassWordTag, CharacterGroup&);
    bool parse(tags::ShorthandCharacterClassWordNegatedTag, CharacterGroup&);
    bool parse(tags::ShorthandCharacterClassDigitTag, CharacterGroup&);
    bool parse(tags::ShorthandCharacterClassDigitNegatedTag, CharacterGroup&);
    bool parse(tags::ShorthandCharacterClassWhitespaceTag, CharacterGroup&);
    bool parse(tags::ShorthandCharacterClassWhitespaceNegatedTag, CharacterGroup&);

    // Any character
    bool parse(tags::AnyCharacterTag, NodePtr&);

    // Escaped characters
    bool parse(tags::EscapedCharacterTag, CodePoint&);

    // Character
    bool parse(tags::CharacterTag, CodePoint&);

    // Quantifier
    bool parse(tags::QuantifierTag, NodePtr&, NodePtr&);
    bool parse(tags::QuantifierTypeTag, NodePtr&, NodePtr&);
    bool parse(tags::LazyModifierTag);
    bool parse(tags::ZeroOrMoreQuantifierTag, NodePtr&, NodePtr&);
    bool parse(tags::OneOrMoreQuantifierTag, NodePtr&, NodePtr&);
    bool parse(tags::ZeroOrOneQuantifierTag, NodePtr&, NodePtr&);
    bool parse(tags::RangeQuantifierTag, NodePtr&, NodePtr&);
    bool parse(tags::RangeQuantifierLowerBoundTag, uint64_t&, bool&);
    bool parse(tags::RangeQuantifierUpperBoundTag, uint64_t&, bool&);
    bool parse(tags::RangeOpenTag);
    bool parse(tags::RangeCloseTag);
    bool parse(tags::RangeSeparatorTag);

    // Numeric
    bool parse(tags::Unicode4DigitCodePointTag, CodePoint&);
    bool parse(tags::Unicode8DigitCodePointTag, CodePoint&);
    bool parse(tags::DigitTag, uint8_t&);
    bool parse(tags::IntegerTag, uint64_t&, bool&);

    //EOF
    bool parse(tags::EOFTag);
};

} // namespace regex::parser

#include "Parser-inl.hpp"