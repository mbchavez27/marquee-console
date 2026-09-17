#pragma once

// Forward declaration of the Marquee class
class Marquee;

/**
 * @brief Handles user commands and interactions for a Marquee instance.
 *
 * The CommandHandler acts as the control interface, responsible for parsing,
 * processing, and executing instructions to manage the associated Marquee.
 */
class CommandHandler
{
public:
    /**
     * @brief Constructs a CommandHandler for the given Marquee.
     *
     * @param marquee A reference to the Marquee instance this handler will control.
     */
    explicit CommandHandler(Marquee &marquee);

    /**
     * @brief Starts the main command processing loop.
     *
     * Prints the Welcome/CSOPESY greeting once, then per iteration prints
     * the developer roster via print_group() and prompts with "Command > ".
     * Reads input with std::getline and dispatches to the Marquee until
     * exit or EOF.
     */
    void run();

    /**
     * @brief Displays the help menu.
     *
     * Prints the exact 6 supported commands: help, start_marquee,
     * stop_marquee, set_text, set_speed, exit.
     */
    void print_help();

    /**
     * @brief Prints the developer roster.
     *
     * Outputs "Group Developers:" followed by the member names.
     * Called once per prompt iteration in run().
     */
    void print_group();

    /**
     * @brief Prints the welcome/CSOPESY greeting message.
     *
     * Outputs a welcome banner and a hint to type 'help'.
     * Called once per prompt iteration in run().
     */
    void print_greetings();

private:
    Marquee &marquee; ///< Reference to the managed Marquee instance.
};