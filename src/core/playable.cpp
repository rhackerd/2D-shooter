#include "core/playable.hpp"


#include <Keyboard.hpp>
#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <raylib.h>
#include <iostream>

Playable::Playable() {}
Playable::~Playable() {}

void Playable::update() {
    float inputX = (raylib::Keyboard::IsKeyDown(KEY_D) - raylib::Keyboard::IsKeyDown(KEY_A));
    float inputY = (raylib::Keyboard::IsKeyDown(KEY_S) - raylib::Keyboard::IsKeyDown(KEY_W));
    

    float speedInMeters = speed / physicslenght;
    
    b2Vec2 velocity{0,0};

    if (inputX != 0 || inputY != 0) {
        velocity.x = inputX * speedInMeters;
        velocity.y = inputY * speedInMeters;
    }

    b2Body_SetLinearVelocity(m_body.bodyId, velocity);
}