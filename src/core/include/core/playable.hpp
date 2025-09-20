#pragma once

#include "player.hpp"

class Playable : public Player {
    private:

    public:
        Playable();
        ~Playable();

    public:
        void update() override;
};