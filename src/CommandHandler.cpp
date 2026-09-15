#include "CommandHandler.h"
#include "Marquee.h"
#include <iostream>
#include <string>

// [TODO]: add comments for description

CommandHandler::CommandHandler(Marquee &m) : marquee(m) {}

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

void CommandHandler::print_group()
{
    std::cout << "Group Developers:\n"
              << "Chavez, Max Benedict B.\n";
    std::cout << "\n";
}

// Displays the main menu after running
void CommandHandler::run()
{
    std::string command;

    // Prints Once
    std::cout << "Welcome to CSOPESY!\n";
    std::cout << "\n";
    std::cout << "Don't know what to type? Type help to know the commands!\n";
    std::cout << "\n";

    // Runs every time
    while (marquee.is_app_alive)
    {
        print_group();
        std::cout << "Command > ";
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
            std::cout << "Enter text: " << std::flush;
            std::string next_text;
            if (!std::getline(std::cin, next_text))
            {
                break;
            }
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
                std::cout << "Invalid speed. Must be a positive integer greater than 0.\n";
            }
        }
        else if (command == "clear_screen")
        {
            marquee.clear_screen();
        }
        else if (command == "exit")
        {
            marquee.is_app_alive = false;
            marquee.is_running = false;
            std::cout << "Goodbye.\n";
            break;
        }
        else if (!command.empty())
        {
            std::cout << "Unknown command. Type 'help'.\n";
        }
    }
}
