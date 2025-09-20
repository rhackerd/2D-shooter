#pragma once
#include <cstdint>

#include <functional>
#include <memory>
#include <raygui-cpp/Button.h>

class Menu {
    private:
        std::vector<rgc::Button> buttons;
        int action = -1;

    public:
        Menu();
        ~Menu();

    public:
        void init();

        void update();

        void shutdown();

    public:
        rgc::Button& getButton(float index) {return buttons[index];}
        int getAction() {int result = action;action = -1;return result;};
};