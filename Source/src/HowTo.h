#ifndef HOWTO
#define HOWTO 

#include <Godot.hpp>
#include <Control.hpp>
#include <TextureButton.hpp>
#include <SceneTree.hpp>
#include <IP.hpp>

namespace godot{

    class HowTo : public Control{
        GODOT_CLASS(HowTo, Control)

        public:

        static void _register_methods();

        void _init();
        void _ready();

        void switch_main();

        HowTo();
        ~HowTo();
    };

}

#endif
