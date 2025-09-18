#include "core/menu.hpp"
#include <raygui-cpp/Bounds.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui-cpp.h>

Menu::Menu() = default;
Menu::~Menu() = default;

void Menu::init() {
    // Reserve space for 3 buttons
    buttons.reserve(3);

    // Create buttons directly into the vector
    buttons.emplace_back(); // Play
    buttons.emplace_back(); // Settings
    buttons.emplace_back(); // Exit

    // Set their positions (top-left corner)
    buttons[0].SetBounds({20, 20, 150, 40});      // Play
    buttons[1].SetBounds({20, 70, 150, 40});      // Settings
    buttons[2].SetBounds({20, 120, 150, 40});     // Exit

    buttons[0].SetText("Play");
    buttons[1].SetText("Settings");
    buttons[2].SetText("Exit");
}

void Menu::update() {
    for (auto& btn : buttons) {
        btn.Update();
        btn.Show(true);
    }
}

void Menu::shutdown() {
    for (auto& btn : buttons) {
        btn.Disable();
    }
}
