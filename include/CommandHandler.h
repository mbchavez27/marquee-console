#pragma once

// [TODO]: add comments for description
class Marquee;

class CommandHandler
{
public:
    explicit CommandHandler(Marquee &marquee);
    void run();
    void print_help();
    void print_group();

private:
    Marquee &marquee;
};
