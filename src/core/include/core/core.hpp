#pragma once



#include <raylib-cpp.hpp>

#include "menu.hpp"
#include "game.hpp"

class Core {
    private:
        ::raylib::Window o_window;
        Menu menu;
        Game game;

        int m_scene = 0;

        bool running = true;

    public:
        Core();
        ~Core();

    public:
        void init();
        void run();
        void shutdown();

        void switchScene(int sceneID);
};