#pragma once

#include <memory>
#include <string>

namespace regex
{

/**
 * @brief Regex class used for matching a target against a pattern.
 */
class Regex
{
public:
    /**
     * @brief Create a new Regex object.
     * @param pattern
     *        The patter to match against.
     *        This string shall contain utf-8 encoded character code points.
     */
    explicit Regex(const std::string& pattern);

    /**
     * @brief Deleted copy construction.
     */
    Regex(const Regex&) = delete;

    /**
     * @brief Deleted copy assignment.
     */
    Regex& operator=(const Regex&) = delete;

    /**
     * @brief Move constructor.
     */
    Regex(Regex&&) = default;

    /**
     * @brief Move assignment.
     */
    Regex& operator=(Regex&&) = default;

    /**
     * @brief Explicit destructor required by PIMPL.
     */
    ~Regex();

    /**
     * @brief Matches a target against the regex.
     * @param target
     *        The string to match.
     *        This string shall contain utf-8 encoded character code points.
     * @return True if the COMPLETE target matches the regex, otherwise false.
     */
    bool match(const std::string& target);

private:
    /**
     * PIMPL.
     */
    class RegexImpl;
    std::unique_ptr<RegexImpl> impl;
};

} // namespace regex