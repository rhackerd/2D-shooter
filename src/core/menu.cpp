#include "core/menu.hpp"
#include <raygui-cpp/Bounds.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui-cpp.h>

Menu::Menu() = default;
Menu::~Menu() = default;

void Menu::init() {
    buttons.reserve(3);

    buttons.emplace_back(); // Play
    buttons.emplace_back(); // Settings
    buttons.emplace_back(); // Exit

    buttons[0].SetBounds({20, 20, 150, 40});      // Play
    buttons[1].SetBounds({20, 70, 150, 40});      // Settings
    buttons[2].SetBounds({20, 120, 150, 40});     // Exit

    buttons[0].SetText("Play");
    buttons[1].SetText("Settings");
    buttons[2].SetText("Exit");

    buttons[0].OnClick([&]() {
        action = 0;
    });

    buttons[1].OnClick([&]() {
        action = 1;
    });
    buttons[2].OnClick([&]()  {
        action = 2;
    });
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
        btn.OnClick(nullptr);
    }
    buttons.clear();
}