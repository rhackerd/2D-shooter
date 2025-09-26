#pragma once

#include <cstdio>
#include <enet.h>

class Network {
    private:
        ENetHost * client;

    public:
        Network();
        ~Network();

    public:
        void init();
        void connect(ENetAddress& address);

        void run(); // Run version that will run the thread
        void _run(); // thread function with the loop

        void disconnect();
        void deinit();
};