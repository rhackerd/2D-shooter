#include "include/core/network.h"
#include <cstddef>
#include <cstdlib>
#include <enet.h>

Network::Network() {
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initiliazing Enet\n");   
    };
};

Network::~Network() {
    enet_deinitialize();
};

void Network::init() {
    client = enet_host_create(NULL, 1, 2, 0, 0);
};