#include "server/network.hpp"

Server_Side::Player& Server_Side::Network::newPlayer() {
    Player& player = players.emplace_back();
    player.health = 100;
    player.x = 0;
    player.y = 0;
    return player;
};