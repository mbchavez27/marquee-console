#include "CommandHandler.h"
#include "Marquee.h"

int main() {
    Marquee marquee;
    CommandHandler handler(marquee);
    handler.run();
    return 0;
}
