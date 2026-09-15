#pragma once
#include <string>
#include <vector>

namespace ascii_art
{
    /**
     * @brief Converts a given text string into its multi-line ASCII art representation.
     *
     * This function takes a standard string and translates its characters into
     * blocky ASCII art. The output is structured as a vector of strings, where
     * each element in the vector corresponds to a single horizontal line of the
     * final ASCII art block.
     *
     * @param text The input string to be converted (e.g., "HELLO").
     * @return std::vector<std::string> A vector containing the rendered lines of ASCII art.
     */
    std::vector<std::string> convert_to_ascii(const std::string &text);
}