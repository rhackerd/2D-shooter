#pragma once

#include <box2d/id.h>
#include <cstdint>
#include <enet.h>
#include <thread>
#include <vector>
#include <box2d/box2d.h>

#define TARGET_TPS 128 // sets 128 to be target random ticks per second
#define POS_CORRECTION 4 // Corrects position every fourth tick§

namespace Server_Side {

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
    struct ShootInfo {
        int32_t x;
        int32_t y;
        int16_t range;
        int16_t rot;
    };
    #pragma pack(pop)

    struct NetworkSettings {
        std::string host;
        int port;
        int maxPlayers;

    };

    enum class Chanels : int {
        Session,
        Position,
        Gameplay,
        Global
    };

    struct Player {
        int health;
        int x;
        int y;
    };

    class Network {
        private:
            ENetHost * server;
            ENetPeer * peer;

            std::thread network_thread;

            std::atomic<bool> running{false};
            std::atomic<uint_fast8_t> tick{0};

            std::vector<Player> players = {};

            b2WorldId world;
        public:
            Network();
            ~Network();

        public:
            void init(NetworkSettings& settings);
            void run();
            void _run();
            void deinit();


            // Gamplay things

        private:
            void _handleRecv(ENetPacket* packet, ENetPeer* peer, enet_uint8 chanel);

            Player& newPlayer();

        public:
            uint_fast8_t getTick() const;
            bool isRunning() const;
    };
};