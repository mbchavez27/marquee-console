#include "CommandHandler.h"
#include "Marquee.h"
#include <cctype>
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
 * Prints the exact 6 supported commands: help, start_marquee,
 * stop_marquee, set_text, set_speed, exit.
 */
void CommandHandler::print_help()
{
    std::cout << "help - show commands\n"
              << "start_marquee - begin scrolling\n"
              << "stop_marquee - pause scrolling\n"
              << "set_text - change marquee text\n"
              << "set_speed - change speed (ms)\n"
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
              << "Leano, Jeremy L.\n"
              << "Go, Timothy Aaron S.\n"
              << "De La Calzada, Wanda\n";
    std::cout << "\n";
}

/**
 * @brief Outputs the welcome/CSOPESY greeting to the console.
 *
 * Prints "Welcome to CSOPESY!" followed by a hint to type 'help'
 * for available commands. Called once before the command loop starts.
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
 * until exit or EOF. Supports inline arguments for set_text and set_speed.
 */
void CommandHandler::run()
{
    std::string line;

    print_greetings();

    // Runs continuously until the 'exit' command is issued or EOF is reached
    while (marquee.is_app_alive)
    {
        print_group();
        std::cout << "Command > " << std::flush;

        // Wait for user input; break if the input stream fails (e.g., EOF)
        if (!std::getline(std::cin, line))
        {
            break;
        }

        // Trim leading whitespace
        size_t first_non_space = line.find_first_not_of(" \t");
        if (first_non_space == std::string::npos)
        {
            continue;
        }

        // Separate command name and argument payload
        size_t space_pos = line.find_first_of(" \t", first_non_space);
        std::string cmd;
        std::string args;

        if (space_pos == std::string::npos)
        {
            cmd = line.substr(first_non_space);
        }
        else
        {
            cmd = line.substr(first_non_space, space_pos - first_non_space);
            size_t arg_start = line.find_first_not_of(" \t", space_pos);
            if (arg_start != std::string::npos)
            {
                args = line.substr(arg_start);
            }
        }

        if (cmd == "help")
        {
            std::cout << "\n";
            print_help();
        }
        else if (cmd == "start_marquee")
        {
            std::cout << "\n";
            marquee.start_marquee();
            std::cout << "\n";
        }
        else if (cmd == "stop_marquee")
        {
            std::cout << "\n";
            marquee.stop_marquee();
            std::cout << "\n";
        }
        else if (cmd == "set_text")
        {
            std::cout << "\n";
            if (args.empty())
            {
                std::cout << "Invalid text. Usage: set_text <text>\n\n";
            }
            else
            {
                // Strip outer surrounding quotes if provided
                if (args.size() >= 2 && ((args.front() == '"' && args.back() == '"') || (args.front() == '\'' && args.back() == '\'')))
                {
                    args = args.substr(1, args.size() - 2);
                }
                std::cout << "New text set to " << args << "\n\n";
                marquee.set_text(args);
            }
        }
        else if (cmd == "set_speed")
        {
            std::cout << "\n";
            if (args.empty())
            {
                std::cout << "Invalid speed. Must be a positive integer greater than 0.\n\n";
            }
            else
            {
                try
                {
                    size_t pos = 0;
                    int value = std::stoi(args, &pos);

                    // Ensure no trailing non-whitespace characters
                    bool valid = true;
                    for (size_t i = pos; i < args.size(); ++i)
                    {
                        if (!std::isspace(static_cast<unsigned char>(args[i])))
                        {
                            valid = false;
                            break;
                        }
                    }

                    if (!valid || value <= 0)
                    {
                        std::cout << "Invalid speed. Must be a positive integer greater than 0.\n\n";
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
                    std::cout << "Invalid speed. Must be a positive integer greater than 0.\n\n";
                }
            }
        }
        else if (cmd == "exit")
        {
            // Shut down the app loop: stop worker thread cleanly, print goodbye, break
            marquee.stop_worker();
            std::cout << "Goodbye.\n";
            break;
        }
        else
        {
            std::cout << "Unknown command. Type 'help'.\n";
        }
    }
}