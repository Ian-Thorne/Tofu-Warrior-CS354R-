#ifndef Familiar_PICKUP_H
#define Familiar_PICKUP_H

#include "constants.h"
#include "Player.h"
#include <Godot.hpp>
#include <Object.hpp>
#include <Spatial.hpp>
#include <Area.hpp>

namespace godot {

class FamiliarPickup : public Spatial {
    GODOT_CLASS(FamiliarPickup, Spatial);

public:
    static void _register_methods();

    FamiliarPickup();
    ~FamiliarPickup();

    void _init();
    void _ready();
    
    void _process();

    void destroy();
};

}

#endif