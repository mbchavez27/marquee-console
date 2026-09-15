#include "CommandHandler.h"
#include "Marquee.h"

/**
 * @brief The main entry point for the application.
 *
 * This function initializes the core components of the system. It creates a
 * single Marquee instance to manage the scrolling text state and passes it
 * to a CommandHandler, which takes over the main thread to process user
 * inputs interactively.
 *
 * @return int Exit status code (0 for successful termination).
 */
int main()
{
    // Instantiate the shared marquee state
    Marquee marquee;

    // Create the command handler and link it to our marquee instance
    CommandHandler handler(marquee);

    // Enter the main interactive console loop
    handler.run();

    return 0;
}