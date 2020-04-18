#ifndef CREDITS
#define CREDITS 

#include <Godot.hpp>
#include <Control.hpp>
#include <TextureButton.hpp>
#include <SceneTree.hpp>
#include <IP.hpp>

namespace godot{

    class Credits : public Control{
        GODOT_CLASS(Credits, Control)

        public:

        static void _register_methods();

        void _init();
        void _ready();

        void switch_main();

        Credits();
        ~Credits();
    };

}

#endif
