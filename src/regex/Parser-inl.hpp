#pragma once

namespace regex::parser
{

template<typename Tag, typename... Args>
bool Parser::parse(Args&... args)
{
    const Utf8Iterator begin = mCurser;
    bool success = parse(Tag{}, args...);

    if (!success)
    {
        // backtrack
        mCurser = begin;
    }

    return success;
}

} // namespace regex::parser