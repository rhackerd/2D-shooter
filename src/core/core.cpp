#include "core/core.hpp"
#include "core/network.h"
#include <enet.h>
#include <exception>
#include <iterator>
#include <memory>
#include <raygui-cpp/Bounds.h>
#include <raylib.h>
#include <iostream>

Core::Core(): o_window() {

}

Core::~Core() {
}
void Core::init() {
    settings.height = 600;
    settings.width = 800;
    settings.vsync = false;
    settings.ip = "";

    o_window.Init(settings.width,settings.height, "2D shooter", FLAG_WINDOW_RESIZABLE); 
    menu.init();
    o_window.SetExitKey(KEY_NULL);
    network.init();
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
                    if (menu.isPlayMenuActive()) {
                        menu.play();
                    }else {
                        Server* selectedServer = menu.getSelectedServer();
                        loadingDialog.setMessage("Connecting", "Connecting To " + selectedServer->name);
                        network.connect(selectedServer->ip, selectedServer->port);
                        continueToGame = false;
                        this->switchScene(1);
                    }
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

            if(!continueToGame) {
                loadingDialog.draw();
                if(loadingDialog.buttonClicked) {
                    network.cancelConnection();
                    switchScene(0); 
                    continue;
                    continueToGame = false;
                }else if(network.isConnected()) {
                    continueToGame = true;
                }

                switch (network.getConnectMessage()) {
                    case (int)LoadingMessage::Failed:
                        loadingDialog.setMessage("Connecting", "Connection Failed...");
                        break;
                }
            }else {
                if(!network.isConnected()) {switchScene(0); continue;};
                game.update();
                game.render();
            }
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
    network.deinit();
};

void Core::update() {
    o_window.SetSize({(float)settings.width, (float)settings.height});
    if (settings.vsync) {
        o_window.SetState(FLAG_VSYNC_HINT);
    }else {
        o_window.ClearState(FLAG_VSYNC_HINT);
    }
};