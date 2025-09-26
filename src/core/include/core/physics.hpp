#pragma once

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <raylib.h>

#define physicslenght 50.0f

float pixelsMeter(float pixels);
float metersPixels(float meters);


typedef struct Entity {
    b2BodyId bodyId;
    b2Vec2 extent;
} Entity;

Entity createBoxEx(b2WorldId world, b2BodyDef &body, b2Vec2 size);

Entity createBox(b2WorldId world, b2Vec2 position, b2Vec2 size);

void destroyBox(Entity &entity, b2WorldId world);

void DrawRectangleB2(const Entity& entity, Color color);
void DrawRectangleB2Ex(b2Vec2 pos, b2Vec2 extent, Color color);

Rectangle getRectangle(const Entity& entity);