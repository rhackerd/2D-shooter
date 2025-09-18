#pragma once

#include <raygui-cpp.h>
#include <cstdint>

class Menu {
    private:
        ::rgc::Button o_playBtn;
        ::rgc::Button o_setBtn;
        ::rgc::Button o_exitBtn;

    public:
        Menu();
        ~Menu();

    public:
        void init();

        void update();

        void shutdown();
};