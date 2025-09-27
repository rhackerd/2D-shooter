#include "core/core.hpp"
#include "server/Server.hpp"
#include <iostream>
#include <string>

int main() {
    std::cout << "Select mode:\n";
    std::cout << "1. Server\n";
    std::cout << "2. Client\n";
    std::cout << "> ";

    int choice = 0;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Starting Server...\n";
        Server_Side::Server server;
        server.init();
        server.run();
        server.deinit();
    } else if (choice == 2) {
        std::cout << "Starting Client...\n";
        Core core;
        core.init();
        core.run();
        core.shutdown();
    } else {
        std::cout << "Invalid choice.\n";
        return 1;
    }

    return 0;
}
