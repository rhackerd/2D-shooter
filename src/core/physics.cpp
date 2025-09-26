#include "core/physics.hpp"
#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <raylib.h>

Entity createBoxEx(b2WorldId world, b2BodyDef &body, b2Vec2 size) {

    Entity entity;
    entity.bodyId = b2CreateBody(world,&body);
    entity.extent = {size.x / 2, size.y / 2};

    b2Polygon polygon = b2MakeBox(entity.extent.x, entity.extent.y);
    b2ShapeDef shapedef = b2DefaultShapeDef();
    b2CreatePolygonShape(entity.bodyId, &shapedef, &polygon);  
    return entity;
};
Entity createBox(b2WorldId world, b2Vec2 position, b2Vec2 size) {
    b2BodyDef bodydef = b2DefaultBodyDef();
    bodydef.position = position;
    return createBoxEx(world, bodydef, size);
};

void destroyBox(Entity &entity, b2WorldId world) {
    b2DestroyBody(entity.bodyId);
};

float pixelsToMeters(float pixels) {
    return pixels * physicslenght;
}

float metersToPixels(float meters) {
    return meters / physicslenght;
};

// Add this to your physics.hpp or create a rendering helper file

// Draw a rectangle using Box2D center position and half-extents (like Box2D uses)
void DrawRectangleB2Ex(b2Vec2 pos, b2Vec2 extent, Color color) {
    // Convert from Box2D meters to pixels
    float pixelX = pos.x * physicslenght;
    float pixelY = pos.y * physicslenght;
    float pixelHalfWidth = extent.x * physicslenght;
    float pixelHalfHeight = extent.y * physicslenght;
    
    // Convert center position to top-left for DrawRectangle
    float topLeftX = pixelX - pixelHalfWidth;
    float topLeftY = pixelY - pixelHalfHeight;
    
    DrawRectangle((int)topLeftX + 10, (int)topLeftY +10, (int)(pixelHalfWidth * 2), (int)(pixelHalfHeight * 2), color);
}

Rectangle getRectangle(const Entity& entity) {
    b2Vec2 pos = b2Body_GetPosition(entity.bodyId);
    b2Vec2 ext = entity.extent;

    float pixelX = pos.x * physicslenght;
    float pixelY = pos.y * physicslenght;
    float pixelHalfWidth = ext.x * physicslenght;
    float pixelHalfHeight = ext.y * physicslenght;

    return (Rectangle){
        pixelX - pixelHalfWidth,   // top-left x
        pixelY - pixelHalfHeight,  // top-left y
        pixelHalfWidth * 2,        // width
        pixelHalfHeight * 2        // height
    };
}


void DrawRectangleB2(const Entity& entity, Color color) {
    b2Vec2 pos = b2Body_GetPosition(entity.bodyId);
    DrawRectangleB2Ex(pos, entity.extent, color);
}