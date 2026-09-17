#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

/**
 * @brief Represents a thread-safe, controllable scrolling text display (marquee).
 *
 * The Marquee class manages the state, text, and animation speed of a scrolling
 * display. It utilizes atomic variables and mutexes to allow safe concurrent
 * modifications from other threads (e.g., a CommandHandler thread) while the
 * marquee is actively rendering.
 */
class Marquee
{
public:
    /// Flag indicating whether the marquee animation is currently active.
    std::atomic<bool> is_running{false};

    /// Flag indicating if the main application is still running.
    /// Used to safely terminate background rendering threads.
    std::atomic<bool> is_app_alive{true};

    /// The delay between animation frames in milliseconds.
    std::atomic<int> speed_ms{200};

    /// The text currently being displayed by the marquee.
    std::string marquee_text{"Hello CSOPESY!"};

    /// Mutex protecting read/write access to marquee_text.
    std::mutex text_mutex;

    /**
     * @brief Constructs the Marquee and spawns the background worker thread.
     */
    Marquee();

    /**
     * @brief Destroys the Marquee, stopping the worker thread before teardown.
     */
    ~Marquee();

    /**
     * @brief Starts or resumes continuous background marquee scrolling.
     *
     * Sets is_running to true. Prints "Marquee is already running." if already on.
     */
    void start_marquee();

    /**
     * @brief Stops/pauses marquee scrolling and clears the marquee display rows.
     *
     * Sets is_running to false. Prints "Marquee is already stopped." if already off.
     */
    void stop_marquee();

    /**
     * @brief Safely updates the text displayed by the marquee.
     *
     * This method locks the text_mutex to ensure the text is updated safely
     * without interfering with the rendering thread.
     *
     * @param next_text The new string to be displayed.
     */
    void set_text(const std::string &next_text);

    /**
     * @brief Updates the scrolling speed of the marquee.
     *
     * @param new_speed_ms The new delay between frames in milliseconds.
     *                     Lower values result in a faster scroll.
     */
    void set_speed(int new_speed_ms);

    /**
     * @brief Renders the current frame at the top of the console.
     */
    void render_current_frame();

    /**
     * @brief Clears the 5 rows occupied by the marquee banner at the top of the screen.
     */
    void clear_marquee_area();

    /**
     * @brief Shuts down the background worker thread cleanly and joins it.
     */
    void stop_worker();

private:
    std::thread worker_thread;
    std::mutex cv_mutex;
    std::condition_variable cv;
    std::size_t scroll_offset{0};

    /**
     * @brief Background render loop that drives the scrolling animation.
     */
    void worker_loop();
};