#pragma once

#include "Console.hpp"
#include "server/network.hpp"

namespace Server_Side {
    class Terminal;

    class Server {
        private:
            Server_Side::Network network;
 
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
