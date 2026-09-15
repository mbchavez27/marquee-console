#include "Marquee.h"
#include "AsciiArt.h"
#include <iostream>
#include <mutex>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @brief Clears the current line in the console.
 *
 * Uses ANSI escape sequences to clear the line and return the cursor to the
 * beginning (`\033[2K\r`). On Windows systems, it attempts to enable virtual
 * terminal processing to support ANSI codes. If that fails (older consoles),
 * it falls back to overwriting the line with spaces.
 */
void Marquee::clear_line()
{
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode))
    {
        // Try to enable ANSI escape sequence support on Windows
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        std::cout << "\033[2K\r" << std::flush;
    }
    else
    {
        // Fallback for older Windows command prompts
        std::cout << "\r" << std::string(80, ' ') << "\r" << std::flush;
    }
#else
    // Native ANSI support for Unix-like systems
    std::cout << "\033[2K\r" << std::flush;
#endif
}

/**
 * @brief Clears the entire console screen and resets the cursor.
 *
 * Uses the ANSI escape sequence `\033[2J` to clear the screen and `\033[H`
 * to move the cursor to the top-left (home) position. Includes a fallback
 * mechanism for older Windows consoles using the Win32 Console API.
 */
void Marquee::clear_screen()
{
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode))
    {
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        std::cout << "\033[2J\033[H" << std::flush;
    }
    else
    {
        // Fallback: manually fill the console buffer with spaces
        COORD top = {0, 0};
        CONSOLE_SCREEN_BUFFER_INFO info;
        DWORD written = 0;
        if (GetConsoleScreenBufferInfo(h, &info))
        {
            DWORD cells = info.dwSize.X * info.dwSize.Y;
            FillConsoleOutputCharacterA(h, ' ', cells, top, &written);
            SetConsoleCursorPosition(h, top);
        }
    }
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}

/**
 * @brief Prints the current text as a one-shot ASCII banner.
 *
 * Exchanges is_running to true. Prints "Marquee is already running."
 * if already on, otherwise snapshots marquee_text under lock, converts
 * it via ascii_art::convert_to_ascii outside the lock, and prints the
 * 5 rows row-by-row.
 */
void Marquee::start_marquee()
{
    // Atomically set to true and check the previous value
    if (is_running.exchange(true))
    {
        std::cout << "Marquee is already running.\n";
        return;
    }

    std::string snapshot;
    {
        // Lock mutex only long enough to safely copy the text
        std::lock_guard<std::mutex> lock(text_mutex);
        snapshot = marquee_text;
    }

    const std::vector<std::string> art = ascii_art::convert_to_ascii(snapshot);
    for (const auto &row : art)
    {
        std::cout << row << "\n";
    }
    std::cout << std::flush;
}

/**
 * @brief Stops banner output and clears the current line.
 *
 * Exchanges is_running to false. Prints "Marquee is already stopped."
 * if already off, otherwise calls clear_line().
 */
void Marquee::stop_marquee()
{
    // Atomically set to false and check the previous value
    if (!is_running.exchange(false))
    {
        std::cout << "Marquee is already stopped.\n";
        return;
    }
    clear_line();
}

/**
 * @brief Safely updates the text to be displayed.
 *
 * Uses a std::lock_guard to prevent race conditions if the rendering thread
 * attempts to read the text at the same time it is being modified.
 *
 * @param next_text The new string to display.
 */
void Marquee::set_text(const std::string &next_text)
{
    std::lock_guard<std::mutex> lock(text_mutex);
    marquee_text = next_text;
}

/**
 * @brief Updates the refresh speed of the marquee.
 *
 * @param new_speed_ms The new interval in milliseconds.
 */
void Marquee::set_speed(int new_speed_ms)
{
    // Atomically update the speed value
    speed_ms.store(new_speed_ms);
}