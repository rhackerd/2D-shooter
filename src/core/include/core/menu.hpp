#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <raygui-cpp/WindowBox.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <raygui-cpp/Button.h>
#include <raygui-cpp/Grid.h>
#define SERVER_FILE "assets/config/servers.json"

struct PreviewRect : public rgc::Component<bool> {
    Rectangle rect;
    PreviewRect(float x, float y, float w, float h) : rect{x, y, w, h} {}
    virtual ~PreviewRect() = default;
    virtual bool Show(bool canClick) override {
        DrawRectangleRec(rect, DARKGRAY);
        DrawRectangleLinesEx(rect, 2, BLACK);
        
        Rectangle rightSide = {rect.x + rect.width, rect.y - 3, 3, rect.height};
        Rectangle topSide = {rect.x + 3, rect.y - 3, rect.width, 3};
        DrawRectangleRec(rightSide, GRAY);
        DrawRectangleRec(topSide, LIGHTGRAY);
        
        return false;
    }
};

struct ServerText : public rgc::Component<bool> {
    std::string text;
    float x, y;
    ServerText(const std::string& t, float xPos, float yPos) : text(t), x(xPos), y(yPos) {}
    virtual ~ServerText() = default;
    virtual bool Show(bool canClick) override {
        DrawText(text.c_str(), (int)x, (int)y, 20, BLACK);
        return false;
    }
};

struct Server {
    std::string name;
    std::string ip;
    int port;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Server, name, ip, port)
};

class Menu {
private:
    std::vector<rgc::Button> buttons;
    int action = -1;
    int selectedServerIndex = -1;
    std::vector<Server> servers;
    std::vector<rgc::WindowBox> server_grid;
    std::vector<std::string> server_titles;
    std::string text;
    bool typing;
    bool isInPlayMenu = false;

public:
    Menu();
    ~Menu();

public:
    void init();
    void mainMenu();
    void settings();
    void play();
    void update();
    void shutdown();
    void refreshLayout();

public:
    rgc::Button& getButton(float index) { return buttons[index]; }
    int getAction() { int result = action; action = -1; return result; }
    Server* getSelectedServer() { 
        if (selectedServerIndex >= 0 && selectedServerIndex < servers.size()) {
            return &servers[selectedServerIndex]; 
        }
        return nullptr; 
    }
    int getSelectedServerIndex() { int result = selectedServerIndex; selectedServerIndex = -1; return result; }
    bool isPlayMenuActive() const { return isInPlayMenu; }
};