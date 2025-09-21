#pragma once
#include <Vector2.hpp>
#include <box2d/box2d.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <raygui-cpp/Bounds.h>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <cstdint>
#include <string>
#include "physics.hpp"
#include "core/physics.hpp"

#define PLAYER_SIZE 20

class Player {
protected:
    raylib::Texture m_texture;
    std::string m_nick;
    uint16_t m_health{100};
    int speed = 200;
    Entity m_body;

public:
    Player();
    virtual ~Player();

public:
    virtual void init(raylib::Color color = raylib::Color::Gray(),
                     std::string nick = "player", b2WorldId world = b2_nullWorldId);
    virtual void update();
    virtual void render();
    virtual void shutdown();

public:
    inline void SetPosition(const raylib::Vector2& pos) {}
    
    inline raylib::Vector2 GetPosition() const {
        b2Vec2 pos = b2Body_GetPosition(m_body.bodyId);  // Fixed: underscore, not asterisk
        return {pos.x * 50, pos.y * 50};                 // Fixed: pos, not _pos
    }
    
    inline void SetNick(const std::string& nick) { m_nick = nick; }
    inline const std::string& GetNick() const { return m_nick; }
    inline void SetHealth(uint16_t health) { m_health = health; }
    inline uint16_t GetHealth() const { return m_health; }
};