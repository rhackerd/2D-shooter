#include "core/player.hpp"
#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_types.h"
#include "chipmunk/cpVect.h"
#include <Image.hpp>
#include <Keyboard.hpp>
#include <exception>
#include <raylib.h>

Player::Player() = default;
Player::~Player() = default;

void Player::init(raylib::Color color, std::string nick, cpSpace* space) {
    if(space == nullptr) {
        printf("Player couldn't be initiated");
        return;
    }
    
    ::Image image = GenImageColor(PLAYER_SIZE, PLAYER_SIZE, color);
    m_texture = LoadTextureFromImage(image);
    UnloadImage(image);
    m_nick = std::move(nick);
    m_health = 100;
    
    cpFloat mass = 1.0f;
    cpFloat width = 20, height = 20;
    cpFloat moment = INFINITY; // prevents rotation
    
    body = cpBodyNew(mass, moment); // dynamic body
    cpBodySetPosition(body, cpv(100,100));
    shape = cpBoxShapeNew(body, width, height, 0);
    cpShapeSetFriction(shape, 1.0f);
    cpShapeSetCollisionType(shape, 1); // Player collision type
    
    cpSpaceAddBody(space, body);
    cpSpaceAddShape(space, shape);
}

void Player::update() {
    // Base player has no movement logic — leave it to derived classes
}

void Player::render() {
    m_texture.Draw(cpBodyGetPosition(body).x, cpBodyGetPosition(body).y);
}

void Player::shutdown() {
    m_texture.Unload();
    m_nick.clear();
}
