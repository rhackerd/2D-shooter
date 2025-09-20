#include "core/player.hpp"
#include <Image.hpp>
#include <Keyboard.hpp>
#include <raylib.h>

Player::Player() = default;
Player::~Player() = default;

void Player::init(raylib::Color color, std::string nick) {
    ::Image image = GenImageColor(20, 20, color);
    m_texture = LoadTextureFromImage(image);
    UnloadImage(image);

    m_nick = std::move(nick);
    m_health = 100;

    p_body = CreatePhysicsBodyRectangle({0,0}, 20, 20, 1.0f);
}

void Player::update() {
    // Base player has no movement logic — leave it to derived classes
}

void Player::render() {
    m_texture.Draw(p_body->position.x, p_body->position.y);
}

void Player::shutdown() {
    m_texture.Unload();
    m_nick.clear();
}
