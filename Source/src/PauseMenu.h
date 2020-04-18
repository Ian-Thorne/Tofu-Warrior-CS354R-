#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "constants.h"
#include "Player.h"
#include "Network.h"
#include "PlayerStatus.h"
#include <Godot.hpp>
#include <Object.hpp>
#include <Control.hpp>
#include <Timer.hpp>
#include <Label.hpp>
#include <TextureProgress.hpp>
#include <string>
#include <Input.hpp>

namespace godot {

class PauseMenu : public Control {
    GODOT_CLASS(PauseMenu, Control);

private:
    Input* input;

public:
    static void _register_methods();

    PauseMenu();
    ~PauseMenu();

    void _init();
    void _ready();
    void _process(float delta);  //might not need _process()
    
};

}

#endif