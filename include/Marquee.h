#pragma once
#include <atomic>
#include <mutex>
#include <string>

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
     * @brief Prints the current text as a one-shot ASCII banner.
     *
     * Sets is_running to true via exchange. Prints
     * "Marquee is already running." if already on, otherwise snapshots
     * marquee_text and prints its 5-row ASCII art.
     */
    void start_marquee();

    /**
     * @brief Stops banner output and clears the current line.
     *
     * Sets is_running to false via exchange. Prints
     * "Marquee is already stopped." if already off, otherwise calls
     * clear_line().
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
     * @brief Clears the current line on the console output.
     *
     * Typically used to clean up trailing characters before rendering the next frame.
     */
    void clear_line();

    /**
     * @brief Clears the entire console screen.
     *
     * Used for full screen resets or layout changes.
     */
    void clear_screen();
};