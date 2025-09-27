#include "include/core/network.h"
#include <chrono>
#include <cstdio>
#include <enet.h>
#include <string>
#include <thread>
#include <atomic>



Network::Network() {
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initializing ENet\n");
    } else {
        printf("ENet initialized\n");
    }
}

Network::~Network() {
    deinit();
}

void Network::init() {
    client = enet_host_create(NULL, 1, 2, 0, 0);
    if (client == NULL) {
        fprintf(stderr, "Failed to create client\n");
    } else {
        printf("Client created\n");
    }
}

void Network::_connect(ENetAddress address) {
    connecting.store(true);
    connected.store(false);
    cancelConnect.store(false);
    connectMessage.store((int)LoadingMessage::Connecting);

    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL) {
        fprintf(stderr, "No available peers for initiating an ENet connection.\n");
        connecting.store(false);
        connectMessage.store((int)LoadingMessage::Failed);
        return;
    }

    ENetEvent event;
    int elapsed = 0;
    const int timeout = 5000;
    const int step = 100;
    
    while (elapsed < timeout && !cancelConnect.load()) {
        int result = enet_host_service(client, &event, step);
        
        if (result > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                printf("Connected to the server successfully!\n");
                connected.store(true);
                connectMessage.store((int)LoadingMessage::Connected);
                connecting.store(false);
                return;
            } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                printf("Server rejected connection\n");
                connectMessage.store((int)LoadingMessage::Failed);
                connecting.store(false);
                return;
            } else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                enet_packet_destroy(event.packet);
            }
        }
        
        elapsed += step;
    }

    enet_peer_reset(peer);
    peer = NULL;
    
    if (cancelConnect.load()) {
        connectMessage.store((int)LoadingMessage::Canceled);
    } else {
        connectMessage.store((int)LoadingMessage::Failed);
    }
    connecting.store(false);
}

void Network::connect(std::string ip) {
    if (connecting.load()) {
        printf("Already connecting, please wait...\n");
        return;
    }
    
    if (connect_thread.joinable()) {
        connect_thread.join();
    }
    
    ENetAddress address;
    int port;
    std::string host;
    size_t colonPos = ip.find(':');
    if (colonPos != std::string::npos) {
        host = ip.substr(0, colonPos);
        port = std::stoi(ip.substr(colonPos + 1));
    } else {
        host = ip;
        port = 25565;
    }
    
    if (enet_address_set_host(&address, host.c_str()) != 0) {
        fprintf(stderr, "Failed to resolve host: %s\n", host.c_str());
        return;
    }
    address.port = port;
    
    connect_thread = std::thread(&Network::_connect, this, address);
}

void Network::connect(std::string ip, int port) {
    if (connecting.load()) {
        printf("Already connecting, please wait...\n");
        return;
    }
    
    if (connect_thread.joinable()) {
        connect_thread.join();
    }
    
    ENetAddress address;
    if (enet_address_set_host(&address, ip.c_str()) != 0) {
        fprintf(stderr, "Failed to resolve host: %s\n", ip.c_str());
        return;
    }
    address.port = port;
    
    connect_thread = std::thread(&Network::_connect, this, address);
}

void Network::disconnect() {
    if (connect_thread.joinable()) {
        cancelConnect.store(true);
        connect_thread.join();
    }
    
    if (!connected.load() || peer == NULL) {
        return;
    }
    
    ENetEvent event;
    enet_peer_disconnect(peer, 0);
    
    bool graceful = false;
    int timeout = 3000;
    while (enet_host_service(client, &event, 100) > 0 && timeout > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                graceful = true;
                break;
        }
        if (graceful) break;
        timeout -= 100;
    }
    
    if (!graceful) {
        enet_peer_reset(peer);
    }
    
    peer = NULL;
    connected.store(false);
}

void Network::run() {
    running.store(true);
    network_thread = std::thread(&Network::_run, this);
}

void Network::_run() {
    using namespace std::chrono;
    auto tick_duration = milliseconds(1000 / TARGET_TPS);
    ENetEvent event;
    
    PlayerUpdate update;
    PlayerUpdateCorrective updateCorrective;

    while (running.load()) {
        auto tick_start = high_resolution_clock::now();
        
        //
        // START
        //

        tick++;
        if (tick % POS_CORRECTION == 0) {
            updateCorrective.posX = posX.load();
            updateCorrective.posY = posY.load();
            updateCorrective.range = 0;
            updateCorrective.rot = 0;
            updateCorrective.velX = velX.load();
            updateCorrective.velY = velY.load();

            ENetPacket * packet = enet_packet_create(&updateCorrective, sizeof(updateCorrective), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 1, packet);
        }else {
            update.range = 0;
            update.rot = 0;
            update.velX = velX.load();
            update.velY = velY.load();

            ENetPacket * packet = enet_packet_create(&update, sizeof(update), ENET_PACKET_FLAG_UNSEQUENCED);
            enet_peer_send(peer, 1, packet);
        };
        
        while (enet_host_service(client, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    printf("Received packet from server (%zu bytes)\n", 
                           event.packet->dataLength);
                    enet_packet_destroy(event.packet);
                    break;
                    
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("Server disconnected us\n");
                    connected.store(false);
                    peer = NULL;
                    break;
            }
        }


        // 
        // END
        //
        
        auto tick_end = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(tick_end - tick_start);
        
        if (elapsed < tick_duration) {
            std::this_thread::sleep_for(tick_duration - elapsed);
        }
    }
}

void Network::deinit() {
    running.store(false);
    if (network_thread.joinable()) {
        network_thread.join();
    }
    
    disconnect();
    
    if (client != NULL) {
        enet_host_destroy(client);
        client = NULL;
    }
    
    enet_deinitialize();
}

void Network::cancelConnection() { cancelConnect.store(true); }
bool Network::isConnecting() const { return connecting.load(); }
bool Network::isConnected() const { return connected.load(); }
int Network::getConnectMessage() const { return connectMessage.load(); }

void Network::setVelocity(int x, int y) {
    velX.store(x);
    velY.store(y);
};
void Network::setPosition(int x, int y) {
    posX.store(x);
    posY.store(y);
};