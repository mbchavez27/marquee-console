#include "CommandHandler.h"
#include "Marquee.h"
#include <iostream>
#include <string>

/**
 * @brief Constructs a new CommandHandler object.
 *
 * @param m Reference to the Marquee instance that this handler will control.
 */
CommandHandler::CommandHandler(Marquee &m) : marquee(m) {}

/**
 * @brief Outputs the list of available commands to the console.
 *
 * Prints the exact 7 supported commands: help, start_marquee,
 * stop_marquee, set_text, set_speed, clear_screen, exit.
 */
void CommandHandler::print_help()
{
    std::cout << "help - show commands\n"
              << "start_marquee - begin scrolling\n"
              << "stop_marquee - pause scrolling\n"
              << "set_text - change marquee text\n"
              << "set_speed - change speed (ms)\n"
              << "clear_screen - clear the screen\n"
              << "exit - quit\n";
    std::cout << "\n";
}

/**
 * @brief Prints the developer roster.
 *
 * Outputs "Group Developers:" followed by the member names.
 * Called once per prompt iteration in run().
 */
void CommandHandler::print_group()
{
    std::cout << "Group Developers:\n"
              << "Chavez, Max Benedict B.\n"
              << "Leano, Jeremy L.\n";
    std::cout << "\n";
}

/**
 * @brief Outputs the welcome/CSOPESY greeting to the console.
 *
 * Prints "Welcome to CSOPESY!" followed by a hint to type 'help'
 * for available commands.
 */
void CommandHandler::print_greetings()
{
    std::cout << "Welcome to CSOPESY!\n";
    std::cout << "\n";
    std::cout << "Don't know what to type? Type help to know the commands!\n";
    std::cout << "\n";
}

/**
 * @brief Main execution loop for the command-line interface.
 *
 * Prints the Welcome/CSOPESY greeting, then per iteration prints
 * the developer roster via print_group() and prompts with "Command > ".
 * Parses standard input with std::getline and dispatches to the Marquee
 * until exit or EOF.
 */
void CommandHandler::run()
{
    std::string command;

    // Runs continuously until the 'exit' command is issued or EOF is reached
    while (marquee.is_app_alive)
    {
        print_greetings();
        print_group();
        std::cout << "Command > ";

        // Wait for user input; break if the input stream fails (e.g., EOF)
        if (!std::getline(std::cin, command))
        {
            break;
        }

        if (command == "help")
        {
            std::cout << "\n";
            print_help();
        }
        else if (command == "start_marquee")
        {
            std::cout << "\n";
            marquee.start_marquee();
            std::cout << "\n";
        }
        else if (command == "stop_marquee")
        {
            std::cout << "\n";
            marquee.stop_marquee();
            std::cout << "\n";
        }
        else if (command == "set_text")
        {
            std::cout << "\n";
            std::cout << "Current text is " << marquee.marquee_text << "\n\n";
            std::cout << "Enter text: " << std::flush;
            std::string next_text;

            // Wait for the multi-word string payload
            if (!std::getline(std::cin, next_text))
            {
                break;
            }

            std::cout << "New text set to " << next_text << "\n\n";
            marquee.set_text(next_text);
        }
        else if (command == "set_speed")
        {
            std::cout << "\n";
            std::cout << "Current speed is " << marquee.speed_ms << "ms\n\n";
            std::cout << "Enter new speed (in milliseconds): " << std::flush;
            std::string line;
            if (!std::getline(std::cin, line))
            {
                break;
            }

            // Safely parse user input into an integer
            try
            {
                int value = std::stoi(line);
                if (value <= 0)
                {
                    std::cout << "Invalid speed. Must be a positive integer greater than 0.\n";
                }
                else
                {
                    marquee.set_speed(value);
                    std::cout << "Speed set to " << value << "ms\n\n";
                }
            }
            catch (const std::exception &)
            {
                // Catch invalid types (e.g., letters) or out-of-range values
                std::cout << "Invalid speed. Must be a positive integer greater than 0.\n";
            }
        }
        else if (command == "clear_screen")
        {
            marquee.clear_screen();
        }
        else if (command == "exit")
        {
            // Shut down the app loop: stop worker thread cleanly, print goodbye, break
            marquee.stop_worker();
            std::cout << "Goodbye.\n";
            break;
        }
        else if (!command.empty())
        {
            std::cout << "Unknown command. Type 'help'.\n";
        }
    }
}