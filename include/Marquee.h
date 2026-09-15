#pragma once
#include <atomic>
#include <mutex>
#include <string>

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
    void clear_line();
};
