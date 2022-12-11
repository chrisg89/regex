#pragma once

#include <memory>
#include <string>

namespace regex
{

class Regex
{
public:
    explicit Regex(const std::string& pattern);
    ~Regex();
    bool match(const std::string& target);

    // no copy
    Regex(const Regex&) = delete;
    Regex& operator=(const Regex&) = delete;

    // move allowed
    Regex(Regex&&) = default;
    Regex& operator=(Regex&&) = default;

private:
    class RegexImpl;
    std::unique_ptr<RegexImpl> impl;
};

} // namespace regex