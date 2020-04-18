#ifndef SWORD_PICKUP_H
#define SWORD_PICKUP_H

#include "constants.h"
#include "Player.h"
#include <Godot.hpp>
#include <Object.hpp>
#include <Spatial.hpp>
#include <Area.hpp>

namespace godot {

class SwordPickup : public Spatial {
    GODOT_CLASS(SwordPickup, Spatial);

public:
    static void _register_methods();

    SwordPickup();
    ~SwordPickup();

    void _init();
    void _ready();
    
    void _process();

    void destroy();
};

}

#endif