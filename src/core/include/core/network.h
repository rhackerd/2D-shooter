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


#pragma pack(push, 1)
struct PlayerUpdate {
    int32_t velX;
    int32_t velY;
    uint16_t rot;   // 0-360
    uint16_t range; // 0-1000
};
struct PlayerUpdateCorrective {
    int32_t velX;
    int32_t velY;
    int64_t posX;
    int64_t posY;
    uint16_t rot;   // 0-360
    uint16_t range; // 0-1000
};
struct Rect {
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
};
struct MapChunk {
    uint16_t rectCount;
    Rect rectangles[128];
};
#pragma pack(pop)

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

        // Velocity
        std::atomic<int> velX{0};std::atomic<int> velY{0};

        // Position
        std::atomic<int> posX{0};std::atomic<int> posY{0};

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

        void setVelocity(int x, int y);
        void setPosition(int x, int y);
};