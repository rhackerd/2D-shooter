#pragma once

#include <cpp-terminal/terminal.h>

namespace Server_Side {
class Terminal {
    private:
        Term::Terminal term;
        std::string line;
        bool running = false;

    public:
        Terminal();
        ~Terminal();

    public:
        void init();

        void run();
            void udpate();

        void deinit();
};
}