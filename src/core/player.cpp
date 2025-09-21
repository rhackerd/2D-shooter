#include <Image.hpp>
#include <box2d/id.h>
#include <box2d/types.h>
#include <exception>
#include <raylib.h>
#include "core/player.hpp"
#include "core/physics.hpp"

Player::Player() = default;
Player::~Player() = default;

void Player::init(raylib::Color color, std::string nick, b2WorldId world) {
    
    ::Image image = GenImageColor(PLAYER_SIZE, PLAYER_SIZE, color);
    m_texture = LoadTextureFromImage(image);
    UnloadImage(image);
    m_nick = std::move(nick);
    m_health = 100;
    
    b2BodyDef def = b2DefaultBodyDef();

    def.type = b2_dynamicBody;

    m_body = createBoxEx(world, def, {(float)PLAYER_SIZE / 50, (float)PLAYER_SIZE / 50});
}

void Player::update() {

}

void Player::render() {
    m_texture.Draw(GetPosition());
    //DrawRectangleB2(m_body, BLACK);
}

void Player::shutdown() {
    m_texture.Unload();
    m_nick.clear();
}
