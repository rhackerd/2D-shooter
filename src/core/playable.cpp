#include "core/playable.hpp"
#include <Keyboard.hpp>
#include <raylib.h>

Playable::Playable() {}
Playable::~Playable() {}

void Playable::update() {
    p_body->velocity.y = ((raylib::Keyboard::IsKeyDown(KEY_S) - raylib::Keyboard::IsKeyDown(KEY_W)));
    p_body->velocity.x = ((raylib::Keyboard::IsKeyDown(KEY_D) - raylib::Keyboard::IsKeyDown(KEY_A)));
};