#include "core/game.hpp"
#include "core/physics.hpp"
#include <box2d/box2d.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <iostream>
#include <memory>
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
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = 0.0f;
    world = b2CreateWorld(&worldDef);

    o_player.init(raylib::Color::Black(), "player", world);

    // Create static boxes
    b2BodyDef def = b2DefaultBodyDef();
    def.type = b2_staticBody;
    def.position = {300.0f / 50.0f, 300.0f / 50.0f};
    boxes.emplace_back(createBoxEx(world, def, {200.0f / 50.0f, 200.0f / 50.0f}));

    def.position = {500.0f / 50.0f, 200.0f / 50.0f};
    boxes.emplace_back(createBoxEx(world, def, {100.0f / 50.0f, 100.0f / 50.0f}));

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

    for (const Entity&box : boxes) {
        DrawRectangleB2(box, BLUE);
    }
    
    EndMode2D();
}

void Game::shutdown() {
    o_player.shutdown();

    for (Entity&box : boxes) {
        destroyBox(box, world);
    };
    boxes.clear();

    if (b2World_IsValid(world)) {
        b2DestroyWorld(world);
        world = b2_nullWorldId;
    };
}