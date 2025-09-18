#include "core/menu.hpp"

Menu::Menu() : o_playBtn(), o_setBtn(), o_exitBtn() {
    // Constructor implementation
}

Menu::~Menu() {
    // Destructor implementation
}

void Menu::init() {
    o_playBtn.SetBounds(::rgc::Bounds::WithText("Play", 22, {20, 50}));
    o_setBtn.SetBounds(::rgc::Bounds::WithText("Settings", 22, {20, 120}));
    o_exitBtn.SetBounds(::rgc::Bounds::WithText("Exit", 22, {20, 190}));
}

void Menu::update() {
    o_playBtn.Update();
    o_setBtn.Update();
    o_exitBtn.Update();
}

void Menu::shutdown() {
    o_playBtn.Disable();
    o_setBtn.Disable();
    o_exitBtn.Disable();
}