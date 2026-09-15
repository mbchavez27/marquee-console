#include "Marquee.h"
#include <iostream>
#include <mutex>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

void Marquee::clear_line()
{
#ifdef _WIN32
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
    std::cout << "\033[2K\r" << std::flush;
#endif
}

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
    clear_line();
}