#include "core/playable.hpp"
#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_types.h"
#include <Keyboard.hpp>
#include <raylib.h>

Playable::Playable() {}
Playable::~Playable() {}

void Playable::update() {
    cpVect velocity;
    velocity.y = (raylib::Keyboard::IsKeyDown(KEY_S) - raylib::Keyboard::IsKeyDown(KEY_W)) * speed;
    velocity.x = (raylib::Keyboard::IsKeyDown(KEY_D) - raylib::Keyboard::IsKeyDown(KEY_A)) * speed;
    cpBodySetVelocity(body, velocity);
    
};