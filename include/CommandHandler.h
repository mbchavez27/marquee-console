#pragma once

class Marquee;

class CommandHandler {
public:
    explicit CommandHandler(Marquee& marquee);
    void run();
    void print_help();

private:
    Marquee& marquee;
};
