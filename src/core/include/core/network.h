#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <enet.h>
#include <string>
#include <thread>
#include <chrono>


#define TARGET_TPS 128 // sets 128 to be target random ticks per second
#define POS_CORRECTION 4 // Corrects position every fourth tick§
enum class Chanels : int {
    Session,
    Position,
    Gameplay,
    Global
};

enum class LoadingMessage : int {
    Connecting,
    Failed,
    Nothing,
    Connected,
    Canceled
};

inline bool operator==(Chanels channel, int value) {
    return static_cast<int>(channel) == value;
}

inline bool operator==(int value, Chanels channel) {
    return static_cast<int>(channel) == value;
}

class Network {
    private:
        ENetHost * client;
        ENetPeer * peer;

        std::thread network_thread;

        std::atomic<bool> running{false};

        std::atomic<uint_fast8_t> tick{0};

        std::thread connect_thread;
        std::atomic<bool> connecting{false};
        std::atomic<bool> connected{false};
        std::atomic<int> connectMessage{(int)LoadingMessage::Nothing};
        std::atomic<bool> cancelConnect{false};


    public:
        Network();
        ~Network();

    public:
        void init();
        void _connect(ENetAddress address);
        void connect(std::string ip);
        void connect(std::string ip, int port);

        void run(); // Run version that will run the thread
        void _run(); // thread function with the loop

        void disconnect();
        void deinit();
        void cancelConnection();

    public:
        bool isConnecting() const;
        bool isConnected() const;
        int getConnectMessage() const;
};