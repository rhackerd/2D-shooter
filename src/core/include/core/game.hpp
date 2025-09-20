#pragma once

#include <raylib-cpp.hpp>
#include "playable.hpp"


#include "physac.h"



class Game {
    private:
        Playable o_player;

        ::raylib::Camera2D camera;

        

    public:
        Game();
        ~Game();

    public:
        void init();

        void update();

        void render();

        void shutdown();
};