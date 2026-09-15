#include "AsciiArt.h"
#include <array>
#include <cctype>
#include <unordered_map>

namespace ascii_art
{
    /**
     * @brief Anonymous namespace to encapsulate internal implementation details.
     *
     * Hides the font dictionary and glyph lookup mechanics from the global scope,
     * ensuring they are only accessible within this specific translation unit.
     */
    namespace
    {
        /**
         * @brief Defines the structure of a single ASCII character.
         *
         * Each character is represented as an array of 5 strings, corresponding
         * to the 5 horizontal rows that make up the blocky ASCII art.
         */
        using Glyph = std::array<std::string, 5>;

        /**
         * @brief Retrieves the internal dictionary of supported ASCII characters.
         *
         * Utilizes a static local variable to ensure the font map is initialized
         * exactly once (Thread-safe in C++11 and later) and reused on subsequent calls.
         *
         * @return const std::unordered_map<char, Glyph>& A reference to the font map.
         */
        const std::unordered_map<char, Glyph> &get_font()
        {
            static const std::unordered_map<char, Glyph> font = {
                {'A', {"  #  ", " # # ", "#####", "#   #", "#   #"}},
                {'B', {"#### ", "#   #", "#### ", "#   #", "#### "}},
                {'C', {" ####", "#    ", "#    ", "#    ", " ####"}},
                {'D', {"#### ", "#   #", "#   #", "#   #", "#### "}},
                {'E', {"#####", "#    ", "#### ", "#    ", "#####"}},
                {'F', {"#####", "#    ", "#### ", "#    ", "#    "}},
                {'G', {" ####", "#    ", "# ###", "#   #", " ####"}},
                {'H', {"#   #", "#   #", "#####", "#   #", "#   #"}},
                {'I', {"#####", "  #  ", "  #  ", "  #  ", "#####"}},
                {'J', {"  ###", "   # ", "   # ", "#  # ", " ##  "}},
                {'K', {"#   #", "#  # ", "###  ", "#  # ", "#   #"}},
                {'L', {"#    ", "#    ", "#    ", "#    ", "#####"}},
                {'M', {"#   #", "## ##", "# # #", "#   #", "#   #"}},
                {'N', {"#   #", "##  #", "# # #", "#  ##", "#   #"}},
                {'O', {" ### ", "#   #", "#   #", "#   #", " ### "}},
                {'P', {"#### ", "#   #", "#### ", "#    ", "#    "}},
                {'Q', {" ### ", "#   #", "#   #", "#  ##", " ## #"}},
                {'R', {"#### ", "#   #", "#### ", "#  # ", "#   #"}},
                {'S', {" ####", "#    ", " ### ", "    #", "#### "}},
                {'T', {"#####", "  #  ", "  #  ", "  #  ", "  #  "}},
                {'U', {"#   #", "#   #", "#   #", "#   #", " ### "}},
                {'V', {"#   #", "#   #", "#   #", " # # ", "  #  "}},
                {'W', {"#   #", "#   #", "# # #", "## ##", "#   #"}},
                {'X', {"#   #", " # # ", "  #  ", " # # ", "#   #"}},
                {'Y', {"#   #", " # # ", "  #  ", "  #  ", "  #  "}},
                {'Z', {"#####", "   # ", "  #  ", " #   ", "#####"}},
                {'0', {" ### ", "#  ##", "# # #", "##  #", " ### "}},
                {'1', {"  #  ", " ##  ", "  #  ", "  #  ", " ### "}},
                {'2', {" ### ", "#   #", "  ## ", " #   ", "#####"}},
                {'3', {"#### ", "    #", " ### ", "    #", "#### "}},
                {'4', {"#   #", "#   #", "#####", "    #", "    #"}},
                {'5', {"#####", "#    ", "#### ", "    #", "#### "}},
                {'6', {" ### ", "#    ", "#### ", "#   #", " ### "}},
                {'7', {"#####", "    #", "   # ", "  #  ", "  #  "}},
                {'8', {" ### ", "#   #", " ### ", "#   #", " ### "}},
                {'9', {" ### ", "#   #", " ####", "    #", " ### "}},
                {' ', {"     ", "     ", "     ", "     ", "     "}},
                {'!', {"  #  ", "  #  ", "  #  ", "     ", "  #  "}},
                {'?', {" ### ", "#   #", "  ## ", "     ", "  #  "}},
                {'.', {"     ", "     ", "     ", "     ", "  #  "}},
            };
            return font;
        }

        /**
         * @brief Looks up a character and returns its corresponding ASCII Glyph.
         *
         * If the requested character is not found in the font dictionary, a
         * default fallback glyph (a question mark '?') is returned.
         *
         * @param c The character to look up.
         * @return const Glyph& A reference to the matched or fallback 5-row Glyph.
         */
        const Glyph &resolve_glyph(char c)
        {
            const auto &font = get_font();
            auto it = font.find(c);
            if (it != font.end())
            {
                return it->second;
            }
            // Fallback glyph: essentially a "?" pattern
            static const Glyph fallback = {" ### ", "#   #", "  ## ", "     ", "  #  "};
            return fallback;
        }
    }

    /**
     * @brief Converts a standard text string into a 5-row ASCII art representation.
     *
     * Iterates through the input string, converts each character to uppercase,
     * retrieves its blocky glyph, and concatenates it line-by-line into a 5-element
     * string vector. Spaces are automatically appended between characters for readability.
     *
     * @param text The input string to convert.
     * @return std::vector<std::string> A vector of size 5 containing the final ASCII rows.
     */
    std::vector<std::string> convert_to_ascii(const std::string &text)
    {
        std::vector<std::string> rows(5, "");
        if (text.empty())
        {
            return rows;
        }
        for (char raw : text)
        {
            // Normalize characters to uppercase before lookup
            char key = static_cast<char>(std::toupper(static_cast<unsigned char>(raw)));
            const Glyph &glyph = resolve_glyph(key);

            // Append each row of the character's glyph to the final output rows
            for (std::size_t r = 0; r < rows.size(); ++r)
            {
                rows[r] += glyph[r] + " ";
            }
        }
        return rows;
    }
}