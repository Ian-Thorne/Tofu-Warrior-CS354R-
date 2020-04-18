#ifndef _SHOP_H_
#define _SHOP_H_

#include "PlayerStatus.h"
#include <Godot.hpp>
#include <Control.hpp>
#include <TextureButton.hpp>
#include <Label.hpp>

namespace godot {

class Shop : public Control {
    GODOT_CLASS(Shop, Control)

private:

public:
    static void _register_methods();

    Shop();
    ~Shop();

    void _init();
    void _ready();
};

}

#endif //_SHOP_H_