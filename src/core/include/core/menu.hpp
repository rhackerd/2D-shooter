#pragma once
#include <cstdint>

#include <raygui-cpp/Button.h>

class Menu {
    private:
        std::vector<rgc::Button> buttons;

    public:
        Menu();
        ~Menu();

    public:
        void init();

        void update();

        void shutdown();
};