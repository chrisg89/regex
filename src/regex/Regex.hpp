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

private:
    class RegexImpl;
    std::unique_ptr<RegexImpl> impl;
};

} //namespace regex