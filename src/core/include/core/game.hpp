#pragma once

#include <string>
#include <raylib-cpp.hpp>
#include "playable.hpp"

#include <chipmunk/chipmunk.h>

class Game {
    private:
        Playable o_player;

        ::raylib::Camera2D camera;

        cpSpace* space;


    public:
        Game();
        ~Game();

    public:
        void init();

        void update();

        void render();

        void shutdown();
};