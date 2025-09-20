#pragma once

#include <Vector2.hpp>
#include <raygui-cpp/Bounds.h>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <cstdint>
#include <string>

#include <physac.h>

class Player {
protected:
    raylib::Texture m_texture;
    std::string m_nick;
    uint16_t m_health{100};

    int speed = 200;

    PhysicsBody p_body;

public:
    Player();
    virtual ~Player();   // virtual destructor for inheritance

public:
    virtual void init(raylib::Color color = raylib::Color::Gray(), 
                      std::string nick = "player");

    virtual void update();
    virtual void render();

    virtual void shutdown();

public:
    inline void SetPosition(const raylib::Vector2& pos) { p_body->position = pos; }
    inline raylib::Vector2 GetPosition() const { return p_body->position; }

    inline void SetNick(const std::string& nick) { m_nick = nick; }
    inline const std::string& GetNick() const { return m_nick; }

    inline void SetHealth(uint16_t health) { m_health = health; }
    inline uint16_t GetHealth() const { return m_health; }
};
