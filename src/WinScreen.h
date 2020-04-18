#ifndef WINSCREEN_H
#define WINSCREEN_H

#include "Network.h"
#include <Godot.hpp>
#include <Control.hpp>
#include <SceneTree.hpp>
#include <Button.hpp>

namespace godot {

    class WinScreen : public Control {
        GODOT_CLASS(WinScreen, Control)

    public:
        static void _register_methods();

        WinScreen();
        ~WinScreen();

        void _init();
        void _ready();

        void switch_screen();
    
    private:
        Network* theNetwork;
    };
}

#endif