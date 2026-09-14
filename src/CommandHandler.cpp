#include "CommandHandler.h"
#include "Marquee.h"
#include <iostream>
#include <string>

CommandHandler::CommandHandler(Marquee &m) : marquee(m) {}

void CommandHandler::print_help()
{
    std::cout << "help - show commands\n"
              << "start_marquee - begin scrolling\n"
              << "stop_marquee - pause scrolling\n"
              << "set_text - change marquee text\n"
              << "set_speed - change speed (ms)\n"
              << "exit - quit\n";
}

void CommandHandler::print_group()
{
    std::cout << "Group Developers:\n"
              << "Chavez, Max Benedict B.\n";
    std::cout << "\n";
}

void CommandHandler::run()
{
    std::string command;

    // Prints Once
    std::cout << "Welcome to CSOPESY!\n";
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
            print_help();
        }
        else if (command == "start_marquee")
        {
            std::cout << "[TODO] start_marquee\n";
        }
        else if (command == "stop_marquee")
        {
            std::cout << "[TODO] stop_marquee\n";
        }
        else if (command == "set_text")
        {
            std::cout << "[TODO] set_text\n";
        }
        else if (command == "set_speed")
        {
            std::cout << "[TODO] set_speed\n";
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
