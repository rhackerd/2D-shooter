#pragma once

#include <Vector4.hpp>
#include <box2d/id.h>
#include <raylib.h>
#include <string>
#include <raylib-cpp.hpp>
#include "playable.hpp"
#include "core/physics.hpp"
#include <box2d/box2d.h>

#define MAX_BOXES 5

class Game {
    private:
        Playable o_player;

        ::raylib::Camera2D camera;

        b2WorldId world;

        ::raylib::Shader shadows;

        std::vector<Entity> boxes;

        raylib::Vector4 boxdata[MAX_BOXES]; // Compiled boxes

    public:
        Game();
        ~Game();

    public:
        void init();

        void update();

        void render();

        void shutdown();
};