
#pragma once

#include "AST.hpp"
#include "Utf8Iterator.hpp"
#include "CodePoint.hpp"

namespace regex::parser
{

namespace tags
{
    struct RegexTag{};
    struct ExpressionTag{};
    struct SubexpressionTag{};
    struct SubexpressionItemTag{};
    struct MatchTag{};
    struct GroupTag{};
    struct AnchorTag{};
    struct BackreferenceTag{};

    struct GroupNonCapturingModifierTag{};
    struct QuantifierTag{};
    struct MatchItemTag{};
    struct MatchAnyCharacterTag{};
    struct MatchCharacterClassTag{};
    struct MatchCharacterTag{};
    struct MatchCharacterEscapeTag{};
    struct CharacterGroupTag{};
    struct CharacterClassTag{};
    struct CharacterClassFromUnicodeCategoryTag{};
    struct CharacterGroupNegativeModifierTag{};
    struct CharacterGroupItemTag{};
    struct CharacterRangeTag{};
    struct CharTag{};
    struct CharacterClassAnyWordTag{};
    struct CharacterClassAnyWordInvertedTag{};
    struct CharacterClassAnyDecimalDigitTag{};
    struct CharacterClassAnyDecimalDigitInvertedTag{};
    struct UnicodeCategoryNameTag{};
    struct QuantifierTypeTag{};
    struct LazyModifierTag{};
    struct ZeroOrMoreQuantifierTag{};
    struct OneOrMoreQuantifierTag{};
    struct ZeroOrOneQuantifierTag{};
    struct RangeQuantifierTag{};
    struct RangeQuantifierLowerBoundTag{};
    struct RangeQuantifierUpperBoundTag{};
    struct StartOfStringAnchorTag{};
    struct AnchorWordBoundaryTag{};
    struct AnchorNonWordBoundaryTag{};
    struct AnchorStartOfStringOnlyTag{};
    struct AnchorEndOfStringOnlyNotNewlineTag{};
    struct AnchorEndOfStringOnlyTag{};
    struct AnchorPreviousMatchEndTag{};
    struct AnchorEndOfStringTag{};
    struct IntegerTag{};
    struct LettersTag{};


    struct AlternativeTag{};
}

using ast::AST;
using ast::NodePtr;

class Parser
{
public:
    Parser(AST&, Utf8Iterator, Utf8Iterator);

private:

    template <typename Tag, typename... Args>
    bool parse(Args&... args);

    bool parse(tags::RegexTag, NodePtr&);
    bool parse(tags::ExpressionTag, NodePtr&);
    bool parse(tags::SubexpressionTag, NodePtr&);
    bool parse(tags::SubexpressionItemTag, NodePtr&);
    bool parse(tags::MatchTag, NodePtr&);
    bool parse(tags::GroupTag, NodePtr&);
    bool parse(tags::AnchorTag, NodePtr&);
    bool parse(tags::BackreferenceTag, NodePtr&);
    bool parse(tags::MatchItemTag, NodePtr&);
    bool parse(tags::QuantifierTag, NodePtr&, NodePtr&);

    bool parse(tags::MatchCharacterClassTag, NodePtr&);




    bool parse(tags::MatchCharacterTag, CodePoint&);
    bool parse(tags::MatchCharacterEscapeTag, CodePoint&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
    //bool parse(tags::RegexTag, NodePtr&);
 

    bool parse(tags::StartOfStringAnchorTag);
    bool parse(tags::AlternativeTag);
    bool parse(tags::AnchorWordBoundaryTag);
    bool parse(tags::AnchorNonWordBoundaryTag);
    bool parse(tags::AnchorStartOfStringOnlyTag);
    bool parse(tags::AnchorEndOfStringOnlyNotNewlineTag);
    bool parse(tags::AnchorEndOfStringOnlyTag);
    bool parse(tags::AnchorPreviousMatchEndTag);
    bool parse(tags::AnchorEndOfStringTag);
    bool parse(tags::MatchAnyCharacterTag);
    
    Utf8Iterator mCurser; //TODO why can this not be const?
    const Utf8Iterator mBegin;
    const Utf8Iterator mEnd;

    bool get(CodePoint & value);
    int pos() const;
    void error(const std::string& msg) const;

    friend class Backtracker;
};


class Backtracker
{
public:
    explicit Backtracker(Utf8Iterator& curser)
    : mCurser {curser}
    , mBegin {curser}
    {}

    void backtrack()
    {
        mCurser = mBegin;
    }

private:
    Utf8Iterator& mCurser;
    Utf8Iterator mBegin;
};

} //namespace regex

#include "Parser-inl.hpp"