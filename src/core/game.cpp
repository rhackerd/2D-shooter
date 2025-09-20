#include "core/game.hpp"

#include <iostream>
#include <raylib.h>

#define PHYSAC_IMPLEMENTATION
#include "physac.h"


Game::Game(): camera() {
    camera.target = {0.0f, 0.0f};
    camera.offset = {0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
};
Game::~Game() {};

void Game::init() {
    o_player.init(raylib::Color::Black());

    InitPhysics();
    SetPhysicsGravity(0, 0);
};

void Game::update() {
    o_player.update();
}

void Game::render() {
    Vector2 playerPos = o_player.GetPosition();
    float deltaTime = GetFrameTime();
    float smoothSpeed = 120; // Adjust this value (higher = faster following)
    
    camera.target.x = Lerp(camera.target.x, playerPos.x, 1.0f - exp(-smoothSpeed * deltaTime));
    camera.target.y = Lerp(camera.target.y, playerPos.y, 1.0f - exp(-smoothSpeed * deltaTime));
    
    camera.offset = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    BeginMode2D(camera);
    o_player.render();
    EndMode2D();
}

void Game::shutdown() {
    o_player.shutdown();
}