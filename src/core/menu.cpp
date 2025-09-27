#include "core/menu.hpp"
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <raygui-cpp/Bounds.h>
#include <raygui-cpp/Button.h>
#include <raylib.h>
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#include <raygui-cpp.h>

Menu::Menu() = default;
Menu::~Menu() = default;

void Menu::init() {
    isInPlayMenu = false;
    mainMenu();
}

void Menu::mainMenu() {
    buttons.clear();
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
        isInPlayMenu = true;
    });

    buttons[1].OnClick([&]() {
        action = 1;
    });
    buttons[2].OnClick([&]()  {
        action = 2;
    });
}

void Menu::play() {
    // Don't clear if we're just switching from main menu
    if (!server_grid.empty()) {
        // Clean up existing components properly
        server_grid.clear();
    }
    
    buttons.clear();
    servers.clear();
    server_titles.clear();

    std::ifstream fileIn(SERVER_FILE);
    nlohmann::json servers_json;
    fileIn >> servers_json;

    servers = servers_json["servers"].get<std::vector<Server>>();
    
    server_grid.reserve(servers.size());
    server_titles.reserve(servers.size());

    int rowHeight = 100;
    int titleHeight = 25;
    int margin = 10;
    int index = 0;
    for (const Server& server : servers) {
        auto& box = server_grid.emplace_back();
        
        server_titles.emplace_back(server.ip + ":" + std::to_string(server.port));
        box.SetTitle(server_titles.back().c_str());
        
        Rectangle bounds = {static_cast<float>(margin), static_cast<float>(margin + (rowHeight + margin) * index), 
                           static_cast<float>(GetScreenWidth() - 2 * margin), static_cast<float>(rowHeight)};
        box.SetBounds(rgc::Bounds(bounds));

        auto* preview = new PreviewRect(bounds.x + margin, bounds.y + titleHeight + margin, 
                                       50, bounds.height - titleHeight - 2 * margin);
        box.AddChild(preview);

        auto* textComp = new ServerText(server.name,
                                        bounds.x + 70 + margin,
                                        bounds.y + titleHeight + margin + 10);
        box.AddChild(textComp);

        auto* connectBtn = new rgc::Button();
        connectBtn->SetText("Connect");
        connectBtn->SetBounds({bounds.x + bounds.width - 120 - margin,
                              bounds.y + titleHeight + margin,
                              100, static_cast<float>(rowHeight - titleHeight - 2 * margin)});
            connectBtn->OnClick([this, index]() {
                selectedServerIndex = index;
                std::cout << "Connecting to " << servers[index].name << " (" << servers[index].ip << ":" << servers[index].port << ")\n";
                action = 0;
                isInPlayMenu = false;
            });
        box.AddChild(connectBtn);

        index++;
    }
}

void Menu::update() {
    if (IsWindowResized()) {
        refreshLayout();
    }
    
    for (auto& btn : buttons) {
        btn.Update();
        (void)btn.Show(true);
    }

    for (size_t i = 0; i < server_grid.size(); ++i) {
        auto& box = server_grid[i];
        box.Update();
        (void)box.Show(true);
    }
}

void Menu::refreshLayout() {
    if (!servers.empty() && !server_grid.empty()) {
        // Clean up existing components properly
        for (auto& box : server_grid) {
            box.ClearChildren();
        }
        server_grid.clear();
        
        server_grid.reserve(servers.size());
        
        int rowHeight = 100;
        int titleHeight = 25;
        int margin = 10;
        int index = 0;
        
        for (const Server& server : servers) {
            auto& box = server_grid.emplace_back();
            box.SetTitle(server_titles[index].c_str());
            
            Rectangle bounds = {static_cast<float>(margin), 
                               static_cast<float>(margin + (rowHeight + margin) * index), 
                               static_cast<float>(GetScreenWidth() - 2 * margin), 
                               static_cast<float>(rowHeight)};
            box.SetBounds(rgc::Bounds(bounds));

            auto* preview = new PreviewRect(bounds.x + margin, bounds.y + titleHeight + margin, 
                                           50, bounds.height - titleHeight - 2 * margin);
            box.AddChild(preview);

            auto* textComp = new ServerText(server.name,
                                            bounds.x + 70 + margin,
                                            bounds.y + titleHeight + margin + 10);
            box.AddChild(textComp);

            auto* connectBtn = new rgc::Button();
            connectBtn->SetText("Connect");
            connectBtn->SetBounds({bounds.x + bounds.width - 120 - margin,
                                  bounds.y + titleHeight + margin,
                                  100, static_cast<float>(rowHeight - titleHeight - 2 * margin)});
        connectBtn->OnClick([this, index]() {
            selectedServerIndex = index;
            std::cout << "Connecting to " << servers[index].name << " (" << servers[index].ip << ":" << servers[index].port << ")\n";
        });
            box.AddChild(connectBtn);

            index++;
        }
    }
}

void Menu::shutdown() {
    // Clean up server grid first
    for (auto& box : server_grid) {
        box.ClearChildren();
    }
    server_grid.clear();
    
    // Clean up main menu buttons
    for (auto& btn : buttons) {
        btn.Disable();
        btn.OnClick(nullptr);
    }
    buttons.clear();
    
    servers.clear();
    server_titles.clear();
}