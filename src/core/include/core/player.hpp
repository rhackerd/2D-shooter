#pragma once

#include "chipmunk/cpVect.h"
#include <Vector2.hpp>
#include <raygui-cpp/Bounds.h>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <cstdint>
#include <string>

#include <chipmunk/chipmunk.h>

#define PLAYER_SIZE 20

class Player {
protected:
    raylib::Texture m_texture;
    std::string m_nick;
    uint16_t m_health{100};

    int speed = 200;

    cpBody* body;
    cpShape* shape;


public:
    Player();
    virtual ~Player();   // virtual destructor for inheritance

public:
    virtual void init(raylib::Color color = raylib::Color::Gray(), 
                      std::string nick = "player", cpSpace* space = nullptr);

    virtual void update();
    virtual void render();

    virtual void shutdown();

public:
    inline void SetPosition(const raylib::Vector2& pos) { return cpBodySetPosition(body, cpv(pos.x, pos.y)); }
    inline raylib::Vector2 GetPosition() const { return Vector2(cpBodyGetPosition(body).x, cpBodyGetPosition(body).y); }

    inline void SetNick(const std::string& nick) { m_nick = nick; }
    inline const std::string& GetNick() const { return m_nick; }

    inline void SetHealth(uint16_t health) { m_health = health; }
    inline uint16_t GetHealth() const { return m_health; }
};
