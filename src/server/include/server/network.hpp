#pragma once

#include "server/Server.hpp"
#include <enet.h>
#include <thread>

namespace Server_Side {

    struct NetworkSettings {
        std::string host;
        int port;
        int maxPlayers;

    };

    class Network {
        private:
            ENetHost * server;
            ENetPeer * peer;

            std::thread network_thread;

            std::atomic<bool> running{false};
        public:
            Network();
            ~Network();

        public:
            void init(NetworkSettings& settings);
            void run();
            void _run();
            void deinit();
    };
};