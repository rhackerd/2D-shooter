#include "core/game.hpp"
#include "core/physics.hpp"
#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <iostream>
#include <raylib.h>


Game::Game(): camera() {
    camera.target = {0.0f, 0.0f};
    camera.offset = {0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

Game::~Game() {}

void Game::init() {
    b2SetLengthUnitsPerMeter(50);
    b2WorldDef worldef = b2DefaultWorldDef();
    worldef.gravity.x = 0.0f;

    world = b2CreateWorld(&worldef);

    o_player.init(raylib::Color::Black(), "player", world);


    b2BodyDef def = b2DefaultBodyDef();
    def.type = b2_staticBody;
    def.position = {300 / 50,300 / 50};

    box = createBoxEx(world, def, {200/50, 200/50});

}

void Game::update() {
    o_player.update();
    b2World_Step(world, GetFrameTime(), 4);
}

void Game::render() {
    Vector2 playerPos = o_player.GetPosition();
    float deltaTime = GetFrameTime();
    float smoothSpeed = 20;
    camera.target.x = Lerp(camera.target.x, playerPos.x, 1.0f - exp(-smoothSpeed * deltaTime));
    camera.target.y = Lerp(camera.target.y, playerPos.y, 1.0f - exp(-smoothSpeed * deltaTime));
    camera.offset = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    BeginMode2D(camera);
    o_player.render();

    DrawRectangleB2(box, BLUE);
    
    EndMode2D();
}

void Game::shutdown() {
    o_player.shutdown();

    destroyBox(box, world);
}