#include "Marquee.h"
#include "AsciiArt.h"
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

namespace
{
    // Helper to get terminal console column width
    int get_terminal_width()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(h, &csbi))
        {
            int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            if (width > 0)
            {
                return width;
            }
        }
#endif
        return 80;
    }

#ifdef _WIN32
    // Helper to position cursor at 1-based (row, col)
    void move_cursor(int row, int col)
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos = {static_cast<SHORT>(col - 1), static_cast<SHORT>(row - 1)};
        SetConsoleCursorPosition(h, pos);
    }
#endif
}

Marquee::Marquee()
{
    worker_thread = std::thread(&Marquee::worker_loop, this);
}

Marquee::~Marquee()
{
    stop_worker();
}

void Marquee::stop_worker()
{
    is_app_alive = false;
    is_running = false;
    cv.notify_all();
    if (worker_thread.joinable())
    {
        worker_thread.join();
    }
}

void Marquee::clear_marquee_area()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(h, &csbi))
    {
        COORD orig_pos = csbi.dwCursorPosition;
        int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        if (width <= 0)
        {
            width = 80;
        }
        for (int r = 1; r <= MARQUEE_ROWS; ++r)
        {
            move_cursor(r, 1);
            std::cout << std::string(width, ' ');
        }
        SetConsoleCursorPosition(h, orig_pos);
        std::cout << std::flush;
        return;
    }
#endif
    // ANSI fallback: save cursor, clear lines 1-MARQUEE_ROWS, restore cursor
    std::cout << "\033[s";
    for (int r = 1; r <= MARQUEE_ROWS; ++r)
    {
        std::cout << "\033[" << r << ";1H\033[2K";
    }
    std::cout << "\033[u" << std::flush;
}

void Marquee::render_current_frame()
{
    std::string snapshot;
    {
        std::lock_guard<std::mutex> lock(text_mutex);
        snapshot = marquee_text;
    }

    std::vector<std::string> art = ascii_art::convert_to_ascii(snapshot);
    if (art.empty() || art[0].empty())
    {
        return;
    }

    std::size_t total_width = art[0].size();
    int term_width = get_terminal_width();
    std::size_t view_width = static_cast<std::size_t>(term_width > 0 ? term_width : 80);

    // Slice viewport horizontally using modular arithmetic
    std::vector<std::string> sliced(MARQUEE_ROWS);
    for (std::size_t r = 0; r < MARQUEE_ROWS; ++r)
    {
        sliced[r].reserve(view_width);
        for (std::size_t col = 0; col < view_width; ++col)
        {
            sliced[r].push_back(art[r][(scroll_offset + col) % total_width]);
        }
    }

#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(h, &csbi))
    {
        COORD orig_pos = csbi.dwCursorPosition;
        for (int r = 1; r <= MARQUEE_ROWS; ++r)
        {
            move_cursor(r, 1);
            std::cout << sliced[r - 1];
        }
        SetConsoleCursorPosition(h, orig_pos);
        std::cout << std::flush;
        return;
    }
#endif
    std::cout << "\033[s";
    for (int r = 1; r <= MARQUEE_ROWS; ++r)
    {
        std::cout << "\033[" << r << ";1H" << sliced[r - 1];
    }
    std::cout << "\033[u" << std::flush;
}

void Marquee::worker_loop()
{
    while (is_app_alive)
    {
        {
            std::unique_lock<std::mutex> lock(cv_mutex);
            cv.wait(lock, [this]()
                    { return !is_app_alive || is_running.load(); });
        }

        if (!is_app_alive)
        {
            break;
        }

        if (is_running)
        {
            render_current_frame();
            scroll_offset++;

            int delay = speed_ms.load();
            if (delay <= 0)
            {
                delay = 200;
            }

            std::unique_lock<std::mutex> lock(cv_mutex);
            cv.wait_for(lock, std::chrono::milliseconds(delay), [this]()
                        { return !is_app_alive || !is_running.load(); });
        }
    }
}

/**
 * @brief Starts or resumes continuous background marquee scrolling.
 */
void Marquee::start_marquee()
{
    if (is_running.exchange(true))
    {
        std::cout << "Marquee is already running.\n";
        return;
    }
    cv.notify_all();
}

/**
 * @brief Stops banner output and clears the marquee area.
 */
void Marquee::stop_marquee()
{
    if (!is_running.exchange(false))
    {
        std::cout << "Marquee is already stopped.\n";
        return;
    }
    cv.notify_all();
    clear_marquee_area();
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
    speed_ms.store(new_speed_ms);
    cv.notify_all();
}