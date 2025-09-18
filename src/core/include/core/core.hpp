#pragma once



#include <raylib-cpp.hpp>

#include "menu.hpp"

class Core {
    private:
        ::raylib::Window o_window;
        Menu menu;

    public:
        Core();
        ~Core();

    public:
        void init();
        void run();
        void shutdown();
};