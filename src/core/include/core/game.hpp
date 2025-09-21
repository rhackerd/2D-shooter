#pragma once

#include <box2d/id.h>
#include <string>
#include <raylib-cpp.hpp>
#include "playable.hpp"
#include "core/physics.hpp"
#include <box2d/box2d.h>

class Game {
    private:
        Playable o_player;

        ::raylib::Camera2D camera;

        b2WorldId world;

        Entity box;
    public:
        Game();
        ~Game();

    public:
        void init();

        void update();

        void render();

        void shutdown();
};