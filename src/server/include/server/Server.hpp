#pragma once

#include "Console.hpp"

namespace Server_Side {
    class Terminal;

    class Server {
        private:
            

        public:
            Server();
            ~Server();

        public:
            void init();

            void run();
                void update();

            void deinit();
    };
};
