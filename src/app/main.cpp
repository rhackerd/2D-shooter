#include "core/core.hpp"
#include "server/Server.hpp"

#define SERVER true

int main() {
    if (SERVER) {
        Server_Side::Server server;
        server.run();
    }else {
        Core core;
        core.init();
        core.run();
        core.shutdown();
    }
    return 0;
};