#include "Marquee.h"
#include <iostream>
#include <mutex>

void Marquee::clear_line()
{
#ifdef _WIN32
    // Windows: enable VT if possible, else fallback
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode))
    {
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        std::cout << "\033[2K\r" << std::flush;
    }
    else
    {
        std::cout << "\r" << std::string(80, ' ') << "\r" << std::flush;
    }
#else
    // Linux/macOS: ANSI guaranteed per README.md:24
    std::cout << "\033[2K\r" << std::flush;
#endif
}

void Marquee::start_marquee()
{
    if (is_running.exchange(true))
    {
        std::cout << "Marquee is already running.\n";
        return;
    }
    std::string snapshot;
    {
        std::lock_guard<std::mutex> lock(text_mutex);
        snapshot = marquee_text;
    }
    std::cout << snapshot << "\n";
}

void Marquee::stop_marquee()
{
    if (!is_running.exchange(false))
    {
        std::cout << "Marquee is already stopped.\n";
        return;
    }
}