#ifndef LOSESCREEN_H
#define LOSESCREEN_H

#include "Network.h"
#include <Godot.hpp>
#include <Control.hpp>
#include <SceneTree.hpp>
#include <Button.hpp>

namespace godot {

    class LoseScreen : public Control {
        GODOT_CLASS(LoseScreen, Control)

    public:
        static void _register_methods();

        LoseScreen();
        ~LoseScreen();

        void _init();
        void _ready();

        void switch_screen();
    
    private:
        Network* theNetwork;
    };
}

#endif