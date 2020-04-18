#ifndef COIN_H
#define COIN_H

#include "Player.h"
#include <Godot.hpp>
#include <Object.hpp>
#include <Spatial.hpp>
#include <Area.hpp>
#include <AudioStreamPlayer.hpp>

namespace godot {

class Coin : public Spatial {
    GODOT_CLASS(Coin, Spatial);

public:
    static void _register_methods();

    Coin();
    ~Coin();

    void _init();
    void _ready();
    void destroy();
    
    void _process();
    Vector3 get_position();
};

}

#endif