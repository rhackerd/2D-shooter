#include "server/network.hpp"
#include <box2d/box2d.h>
#include <box2d/types.h>
#include <chrono>
#include <cstdio>
#include <enet.h>
#include <ratio>
#include <thread>
#include <iostream>

namespace Server_Side {

Network::Network() : server(nullptr), peer(nullptr) {
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initializing ENet\n");
    } else {
        printf("ENet initialized successfully\n");
    }
}

Network::~Network() {
    deinit();
    enet_deinitialize();
}

void Network::init(NetworkSettings& settings) {
    printf("Initializing server with host: %s, port: %d, maxPlayers: %d\n", 
           settings.host.c_str(), settings.port, settings.maxPlayers);
    
    ENetAddress address;
    
    if (settings.host == "localhost" || settings.host == "127.0.0.1" || settings.host.empty()) {
        address.host = ENET_HOST_ANY;
        printf("Using ENET_HOST_ANY (listening on all interfaces)\n");
    } else {
        if (enet_address_set_host(&address, settings.host.c_str()) != 0) {
            fprintf(stderr, "Failed to resolve host: %s\n", settings.host.c_str());
            return;
        }
        printf("Host resolved successfully\n");
    }
    
    address.port = settings.port;
    
    server = enet_host_create(&address, settings.maxPlayers, 2, 0, 0);
    
    if (server == NULL) {
        fprintf(stderr, "Failed to create server on port %d\n", settings.port);
        return;
    } else {
        printf("Server created successfully on port %d\n", settings.port);
    }

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = 0.0f;
    world = b2CreateWorld(&worldDef);
}

void Network::deinit() {
    running.store(false);
    
    if (network_thread.joinable()) {
        network_thread.join();
    }
    
    if (server != NULL) {
        enet_host_destroy(server);
        server = NULL;
    }
}

void Network::run() {
    if (server == NULL) {
        printf("ERROR: Cannot start network thread - server is NULL!\n");
        return;
    }
    
    running.store(true);
    network_thread = std::thread(&Network::_run, this);
}

void Network::_run() {
    auto tick_duration = std::chrono::microseconds(1000000 / TARGET_TPS);
    ENetEvent event;
    int tick_count = 0;
    
    // Initialize the timer outside the loop
    auto last_output = std::chrono::high_resolution_clock::now();
    
    while (running.load()) {
        tick_count++;
        tick.store(tick_count % 256);
        
        auto tick_start = std::chrono::high_resolution_clock::now();
        
        int service_result = enet_host_service(server, &event, 0);
        
        if (service_result > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("Player connected from %x:%u\n", 
                           event.peer->address.host, 
                           event.peer->address.port);
                    event.peer->data = &newPlayer();
                    break;
                    
                case ENET_EVENT_TYPE_RECEIVE:
                    _handleRecv(event.packet, event.peer, event.channelID);
                    enet_packet_destroy(event.packet);
                    break;
                    
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("Player disconnected from %x:%u\n",
                           event.peer->address.host,
                           event.peer->address.port);
                    event.peer->data = nullptr;
                    break;
            }
        } else if (service_result < 0) {
            printf("ERROR: enet_host_service returned %d\n", service_result);
        }
        
        // TPS calculation - do this BEFORE updating last_output
        if (tick_count % 128 == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - last_output);
            
            if (tick_count > 128) { // Skip first measurement
                printf("Network tick: %d (%.1f TPS)\n", tick_count, 128000000.0 / duration.count());
            }
            last_output = now;
        }
        
        auto tick_end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(tick_end - tick_start);
        
        if (elapsed < tick_duration) {
            std::this_thread::sleep_for(tick_duration - elapsed);
        }

        tick.store(tick_count % 128);
    }
}


void Network::_handleRecv(ENetPacket* packet, ENetPeer* peer, enet_uint8 chanel) {
    switch (chanel) {
        case (int)Chanels::Session:
            break;

        case (int)Chanels::Position:
            break;

        case (int)Chanels::Gameplay:
            break;

        case (int)Chanels::Global:
            break;
    };
};

uint_fast8_t Network::getTick() const { return tick.load(); }
bool Network::isRunning() const { return running.load(); }

}