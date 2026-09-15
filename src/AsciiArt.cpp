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
                {'A', {"    _   ", "   /_\\  ", "  / _ \\ ", " /_/ \\_\\", "        "}},
                {'B', {"  ___ ", " | _ )", " | _ \\", " |___/", "      "}},
                {'C', {"   ___ ", "  / __|", " | (__ ", "  \\___|", "       "}},
                {'D', {"  ___  ", " |   \\ ", " | |) |", " |___/ ", "       "}},
                {'E', {"  ___ ", " | __|", " | _| ", " |___|", "      "}},
                {'F', {"  ___ ", " | __|", " | _| ", " |_|  ", "      "}},
                {'G', {"   ___ ", "  / __|", " | (_ |", "  \\___|", "       "}},
                {'H', {"  _  _ ", " | || |", " | __ |", " |_||_|", "       "}},
                {'I', {"  ___ ", " |_ _|", "  | | ", " |___|", "      "}},
                {'J', {"     _ ", "  _ | |", " | || |", "  \\__/ ", "       "}},
                {'K', {"  _  __", " | |/ /", " | ' < ", " |_|\\_\\", "       "}},
                {'L', {"  _    ", " | |   ", " | |__ ", " |____|", "       "}},
                {'M', {"  __  __ ", " |  \\/  |", " | |\\/| |", " |_|  |_|", "         "}},
                {'N', {"  _  _ ", " | \\| |", " | .` |", " |_|\\_|", "       "}},
                {'O', {"   ___  ", "  / _ \\ ", " | (_) |", "  \\___/ ", "        "}},
                {'P', {"  ___ ", " | _ \\", " |  _/", " |_|  ", "      "}},
                {'Q', {"   ___  ", "  / _ \\ ", " | (_) |", "  \\__\\_\\", "        "}},
                {'R', {"  ___ ", " | _ \\", " |   /", " |_|_\\", "      "}},
                {'S', {"  ___ ", " / __|", " \\__ \\", " |___/", "      "}},
                {'T', {"  _____ ", " |_   _|", "   | |  ", "   |_|  ", "        "}},
                {'U', {"  _   _ ", " | | | |", " | |_| |", "  \\___/ ", "        "}},
                {'V', {" __   __", " \\ \\ / /", "  \\ V / ", "   \\_/  ", "        "}},
                {'W', {" __      __", " \\ \\    / /", "  \\ \\/\\/ / ", "   \\_/\\_/  ", "           "}},
                {'X', {" __  __", " \\ \\/ /", "  >  < ", " /_/\\_\\", "       "}},
                {'Y', {" __   __", " \\ \\ / /", "  \\ V / ", "   |_|  ", "        "}},
                {'Z', {"  ____", " |_  /", "  / / ", " /___|", "      "}},
                {'0', {"   __  ", "  /  \\ ", " | () |", "  \\__/ ", "       "}},
                {'1', {"  _ ", " / |", " | |", " |_|", "    "}},
                {'2', {"  ___ ", " |_  )", "  / / ", " /___|", "      "}},
                {'3', {"  ____", " |__ /", "  |_ \\", " |___/", "      "}},
                {'4', {"  _ _  ", " | | | ", " |_  _|", "   |_| ", "       "}},
                {'5', {"  ___ ", " | __|", " |__ \\", " |___/", "      "}},
                {'6', {"   __ ", "  / / ", " / _ \\", " \\___/", "      "}},
                {'7', {"  ____ ", " |__  |", "   / / ", "  /_/  ", "       "}},
                {'8', {"  ___ ", " ( _ )", " / _ \\", " \\___/", "      "}},
                {'9', {"  ___ ", " / _ \\", " \\_, /", "  /_/ ", "      "}},
                {' ', {"  ", "  ", "  ", "  ", "  "}},
                {'!', {"  _ ", " | |", " |_|", " (_)", "    "}},
                {'?', {"  ___ ", " |__ \\", "   /_/", "  (_) ", "      "}},
                {'.', {"    ", "    ", "  _ ", " (_)", "    "}},
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
            static const Glyph fallback = {"  ___ ", " |__ \\", "   /_/", "  (_) ", "      "};
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