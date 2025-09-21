#include "core/game.hpp"
#include "chipmunk/chipmunk.h"
#include <iostream>
#include <raylib.h>
#include <chipmunk/chipmunk.h>

// Static collision callback function
static cpBool playerWallPreSolve(cpArbiter* arb, cpSpace* space, void* userData) {
    cpBody* playerBody;
    cpBody* wallBody;
    cpArbiterGetBodies(arb, &playerBody, &wallBody);
    
    // Stop the player immediately upon collision
    cpBodySetVelocity(playerBody, cpvzero);
    return cpTrue; // Allow collision to be processed normally
}

Game::Game(): camera() {
    camera.target = {0.0f, 0.0f};
    camera.offset = {0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0,0));
}

Game::~Game() {}

void Game::init() {
    o_player.init(raylib::Color::Black(), "player", space);
    
    // Add testing rectangle
    cpBody* staticBody = cpSpaceGetStaticBody(space);
    float rectX = 200; // desired center X in pixels
    float rectY = 200; // desired center Y
    float rectW = 100;
    float rectH = 100;
    
    // Create bounding box centered at the desired position
    cpBB bbox = cpBBNew(rectX - rectW/2, rectY - rectH/2, rectX + rectW/2, rectY + rectH/2);
    cpShape* wall = cpBoxShapeNew2(staticBody, bbox, 0);
    cpShapeSetFriction(wall, 1.0f);
    cpShapeSetCollisionType(wall, 2); // Wall collision type
    cpSpaceAddShape(space, wall);
    
    // Add collision handler that stops movement immediately
    cpCollisionHandler* handler = cpSpaceAddCollisionHandler(space, 1, 2);
    handler->preSolveFunc = playerWallPreSolve;
}

void Game::update() {
    o_player.update();
    cpSpaceStep(space, GetFrameTime());
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
    
    // Draw rectangle at the correct position
    DrawRectangle(200 - 50, 200 - 50, 100, 100, BLUE);
    
    EndMode2D();
}

void Game::shutdown() {
    o_player.shutdown();
    cpSpaceFree(space);
}