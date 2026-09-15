#include "CommandHandler.h"
#include "Marquee.h"

// [TODO]: add comments for description
int main()
{
    Marquee marquee;
    CommandHandler handler(marquee);
    handler.run();
    return 0;
}
