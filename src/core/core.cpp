#include "core/core.hpp"

void Core::init() {
    o_window.Init(800,600, "2D shooter", FLAG_WINDOW_RESIZABLE);
    menu.init();
};

void Core::run() {
    while (!o_window.ShouldClose()) {
        o_window.BeginDrawing();

        o_window.ClearBackground(GRAY);

        menu.update();

        o_window.EndDrawing();
    };
};

void Core::shutdown() {
    menu.shutdown();
    o_window.Close();
};