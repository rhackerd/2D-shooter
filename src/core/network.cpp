#include "include/core/network.h"
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <enet.h>
#include <functional>
#include <string>
#include <thread>
#include <type_traits>
#include <atomic>
#include <future>

Network::Network() {
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initiliazing Enet\n");
    } else {
        printf("ENet inited\n");
    }
}

Network::~Network() {
    disconnect();
    if (connect_thread.joinable()) {
        connect_thread.join();
    }
    enet_deinitialize();
}

void Network::init() {
    client = enet_host_create(NULL, 16, 4, 0, 0);
    if (client == NULL) {
        fprintf(stderr, "Failed to create client");
        return;
    } else {
        printf("Client created\n");
    }
}

void Network::_connect(ENetAddress address) {
    connecting.store(true);
    connected.store(false);
    cancelConnect.store(false);

    connectMessage.store((int)LoadingMessage::Connecting);

    peer = enet_host_connect(client, &address, 4, 0);
    if (peer == NULL) {
        fprintf(stderr, "No available peers for initiating an ENet connection.");
        connecting.store(false);
        connectMessage.store((int)LoadingMessage::Failed);
        return;
    }

    ENetEvent event;
    int elapsed = 0;
    const int timeout = 5000;
    const int step = 100;

    while (elapsed < timeout && !cancelConnect.load()) {
        if (enet_host_service(client, &event, step) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT) {
            printf("Connected to the server\n");
            connected.store(true);
            connectMessage.store((int)LoadingMessage::Connected);
            connecting.store(false);
            return;
        }
        elapsed += step;
    }

    // Cancel or timeout
    enet_peer_reset(peer);
    if (cancelConnect.load()) {
        fprintf(stderr, "Connection attempt canceled\n");
        connectMessage.store((int)LoadingMessage::Canceled);
    } else {
        fprintf(stderr, "Failed to connect to the server (timeout)\n");
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
    
    if (enet_address_set_host_ip_new(&address, host.c_str()) != 0) {
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
    if (enet_address_set_host_ip_new(&address, ip.c_str()) != 0) {
        fprintf(stderr, "Failed to resolve host: %s\n", ip.c_str());
        return;
    }
    address.port = port;
    
    connect_thread = std::thread(&Network::_connect, this, address);
}

void Network::disconnect() {
    if (connect_thread.joinable()) {
        connect_thread.join();
    }
    
    if (!connected.load()) return;
    
    ENetEvent event;
    enet_peer_disconnect(peer, 0);
    while (enet_host_service(client, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts("disconnection succeeded");
                break;
        }
    }
    enet_peer_reset(peer);
    connected.store(false);
}

bool Network::isConnecting() const {
    return connecting.load();
}

bool Network::isConnected() const {
    return connected.load();
}

int Network::getConnectMessage() const {
    return connectMessage.load();
}

void Network::cancelConnection() {
    cancelConnect.store(true);
};

void Network::_run() {
    using namespace std::chrono;
    auto tick_duration = milliseconds(1000 / TARGET_TPS);
    ENetEvent event;
    while (running) {
        auto tick_start = high_resolution_clock::now();
        
        tick++;
        if (tick % POS_CORRECTION == 0) {
        } else {
        }
        
        while (enet_host_service(client, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    enet_packet_destroy(event.packet);
                    break;
            }
        }
    }
}

void Network::run() {
    running = true;
    network_thread = std::thread(&Network::_run, this);
}

void Network::deinit() {
    if (connect_thread.joinable()) {
        connect_thread.join();
    }
    enet_deinitialize();
}