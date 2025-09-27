#include "server/Console.hpp"
#include <cpp-terminal/terminal.h>
#include <ostream>

namespace Server_Side {
Terminal::Terminal() : term(true) {

};

Terminal::~Terminal() {

};


void Terminal::init() {

};

void Terminal::udpate() {

};

void Terminal::run() {
    printf("\n");
    running = true;
    while (running) {
        udpate();
    }
};  



void Terminal::deinit() {

};
}