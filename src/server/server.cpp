#include "server/Server.hpp"
#include "server/network.hpp"
namespace Server_Side
{    
Server::Server()  {

};

Server::~Server() {

};


void Server::init() {
    NetworkSettings settings = {};
    settings.maxPlayers = 32;
    settings.host = "127.0.0.1";
    settings.port = 25565;
    network.init(settings);
};

void Server::update() {

};

void Server::run() {
    network.run();
    while (true) {};
};

void Server::deinit() {
    network.deinit();
};
}