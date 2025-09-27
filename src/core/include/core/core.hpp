#pragma once
#include <raygui-cpp/Bounds.h>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include "menu.hpp"
#include "game.hpp"
#include "network.h"

struct LoadingDialog {
    std::string title = "Loading";
    std::string message = "Please wait...";
    Color bgColor = {40, 40, 40, 200};
    Color titleBarColor = {60, 60, 60, 255};
    Color borderColor = {80, 80, 80, 255};
    Color textColor = WHITE;
    Color buttonColor = {70, 70, 70, 255};
    Color buttonHoverColor = {90, 90, 90, 255};
    Color buttonTextColor = WHITE;
    int minWidth = 300;
    int minHeight = 120;
    int padding = 20;
    int titleHeight = 30;
    int buttonHeight = 30;
    int buttonWidth = 80;
    float animationTime = 0.0f;
    bool buttonClicked = false;
    
    void update() {
        animationTime += GetFrameTime();
        
        // Check for button click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            Rectangle buttonRect = getButtonRect();
            
            if (CheckCollisionPointRec(mousePos, buttonRect)) {
                buttonClicked = true;
            }
        }
    }
    
    Rectangle getButtonRect() {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        int textWidth = MeasureText(message.c_str(), 20);
        int titleWidth = MeasureText(title.c_str(), 18);
        int width = fmax(minWidth, fmax(textWidth + padding * 2, titleWidth + padding * 2));
        int height = minHeight + buttonHeight + padding / 2; // Added space for button
        int x = (screenW - width) / 2;
        int y = (screenH - height) / 2;
        
        // Button positioned at bottom right of dialog
        int buttonX = x + width - buttonWidth - padding;
        int buttonY = y + height - buttonHeight - padding / 2;
        
        return {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    }
    
    void draw() {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        int textWidth = MeasureText(message.c_str(), 20);
        int titleWidth = MeasureText(title.c_str(), 18);
        int width = fmax(minWidth, fmax(textWidth + padding * 2, titleWidth + padding * 2));
        int height = minHeight + buttonHeight + padding / 2; // Added space for button
        int x = (screenW - width) / 2;
        int y = (screenH - height) / 2;
        
        // Draw main dialog background
        DrawRectangle(x, y, width, height, bgColor);
        
        // Draw title bar
        DrawRectangle(x, y, width, titleHeight, titleBarColor);
        
        // Draw borders
        DrawRectangleLinesEx({(float)x, (float)y, (float)width, (float)height}, 2, borderColor);
        DrawRectangleLinesEx({(float)x, (float)y, (float)width, (float)titleHeight}, 1, borderColor);
        
        // Draw title
        DrawText(title.c_str(), x + padding, y + 6, 18, textColor);
        
        // Draw message
        DrawText(message.c_str(), x + padding, y + titleHeight + 20, 20, textColor);
        
        // Draw animated dots
        int dotCount = (int)(animationTime * 2) % 4;
        std::string dots(dotCount, '.');
        DrawText(dots.c_str(), x + padding + textWidth + 5, y + titleHeight + 20, 20, textColor);
        
       // Draw cancel button using GuiButton
        Rectangle buttonRect = getButtonRect();
        if (GuiButton(buttonRect, "Abort")) {
            buttonClicked = true;
        }
    }
    
    void setMessage(const std::string& newTitle, const std::string& newMessage) {
        title = newTitle;
        message = newMessage;
        animationTime = 0.0f;
        buttonClicked = false; // Reset button state
    }
};

struct Settings {
    std::string ip;
    bool vsync;
    int width;
    int height;
};

class Core {
private:
    ::raylib::Window o_window;
    Menu menu;
    Game game;
    Network network;
    Settings settings;
    LoadingDialog loadingDialog;
    int m_scene = 0;
    bool running = true;

    int continueToGame = false;

public:
    Core();
    ~Core();

public:
    void init();
    void run();
    void shutdown();
    void update();
    void switchScene(int sceneID);
};