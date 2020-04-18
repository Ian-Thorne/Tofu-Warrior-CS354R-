#ifndef _HEALTH_POTION_H_
#define _HEALTH_POTION_H_

#include "constants.h"
#include "PlayerStatus.h"
#include <Godot.hpp>
#include <Control.hpp>
#include <ColorRect.hpp>
#include <Label.hpp>
#include <TextureButton.hpp>
#include <Input.hpp>

namespace godot {

class HealthPotion : public Control {
    GODOT_CLASS(HealthPotion, Control)

private:
    ColorRect* imgBox;
    ColorRect* descBox;

public:
    static void _register_methods();

    HealthPotion();
    ~HealthPotion();

    void _init();
    void _ready();
    void _process();

    void restore_hp();
};

}

#endif //_HEALTH_POTION_H_
