#ifndef GLIDER_PICKUP_H
#define GLIDER_PICKUP_H

#include "constants.h"
#include "Player.h"
#include <Godot.hpp>
#include <Object.hpp>
#include <Spatial.hpp>
#include <Area.hpp>

namespace godot {

class GliderPickup : public Spatial {
    GODOT_CLASS(GliderPickup, Spatial);

public:
    static void _register_methods();

    GliderPickup();
    ~GliderPickup();

    void _init();
    void _ready();
    
    void _process();
    void destroy();
};

}

#endif