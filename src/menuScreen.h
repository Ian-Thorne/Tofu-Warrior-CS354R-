#ifndef MENUSCREEN
#define MENUSCREEN 

#include <Godot.hpp>
#include <Control.hpp>
#include <TextureButton.hpp>
#include <SceneTree.hpp>
#include <IP.hpp>

namespace godot{

    class menuScreen : public Control{
        GODOT_CLASS(menuScreen, Control)

        public:

        static void _register_methods();

        void _init();
        void _ready();

        void switch_single();
        void switch_multi();
        void switch_credits();
        void switch_howto();

        String INT_IP;
        String EXT_IP;


        menuScreen();
        ~menuScreen();
    };

}

#endif
