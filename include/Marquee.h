#pragma once
#include <atomic>
#include <mutex>
#include <string>

// [TODO]: add comments for description
class Marquee
{
public:
    std::atomic<bool> is_running{false};
    std::atomic<bool> is_app_alive{true};
    std::atomic<int> speed_ms{200};
    std::string marquee_text{"Hello CSOPESY!"};
    std::mutex text_mutex;

    void start_marquee();
    void stop_marquee();
    void set_text(const std::string &next_text);
    void set_speed(int new_speed_ms);
    void clear_line();
    void clear_screen();
};
