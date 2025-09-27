#include "server/network.hpp"
#include <enet.h>

namespace Server_Side {
Network::Network() {
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initiliazing Enet\n");
    } else {
        printf("ENet inited\n");
    }
}

Network::~Network() {
    deinit();
    enet_deinitialize();
}

void Network::init(NetworkSettings& settings) {
    ENetAddress address;
    if (enet_address_set_host_ip_new(&address, settings.host.c_str()) != 0) {
        fprintf(stderr, "Failed to resolve host: %s\n", settings.host.c_str());
        return;
    }
    address.port = settings.port;

    server = enet_host_create(&address, settings.maxPlayers, 4, 0, 0);
    if (server == NULL) {
        fprintf(stderr, "Failed to create client");
        return;
    } else {
        printf("Client created\n");
    }
};

void Network::deinit() {
    running.store(false);
    if (network_thread.joinable()) {
        network_thread.join();
    };
    ENetEvent event;
    enet_peer_disconnect(peer, 0);

    while(enet_host_service(server, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts("disconnection succeeded");
                break;
        };
    };

    enet_peer_reset(peer);

    enet_host_destroy(server);
};

void Network::run() {
    running.store(true);
    network_thread = std::thread(&Network::_run, this);
};
};