#include "core/core.hpp"
#include <exception>
#include <memory>
#include <raylib.h>

Core::Core(): o_window() {

}

Core::~Core() {
}
void Core::init() {
    o_window.Init(800,600, "2D shooter", FLAG_WINDOW_RESIZABLE);
    menu.init();
    o_window.SetExitKey(KEY_NULL);
};

void Core::run() {
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    while (running && !o_window.ShouldClose()) {

        o_window.BeginDrawing();

        o_window.ClearBackground(GRAY);

        if (m_scene == 0) {
            menu.update();
            switch (menu.getAction()) {
                case 0:
                    this->switchScene(1);
                    break;
                case 1:
                    break;

                case 2: 
                    running = false;
                    break;
                default:
                    break;
            };

        }else if(m_scene == 1) {
            game.update();
            game.render();
        };

        DrawFPS(1, 1);
        o_window.EndDrawing();
    };
};

void Core::switchScene(int sceneID) {
    switch (m_scene) {
        case 0: 
            menu.shutdown();
            break;
        case 1:
            game.shutdown();
            break;
        default:
            break;
    };

    switch (sceneID) {
        case 0:
            menu.init();
            break;
        case 1: 
            game.init();
            break;
        default:
            break;
    };
    m_scene = sceneID;
};

void Core::shutdown() {
    if (m_scene == 0) menu.shutdown();
    else if (m_scene == 1) game.shutdown();

    o_window.Close();
};