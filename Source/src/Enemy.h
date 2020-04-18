#ifndef ENEMY_H
#define ENEMY_H

#include "constants.h"
#include "Player.h"
#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <Area.hpp>
#include <AudioStreamPlayer.hpp>

namespace godot {

class Enemy : public KinematicBody {
    GODOT_CLASS(Enemy, KinematicBody);

private:
    int hp;
    int damageDealt;
    int goldValue;

public:
    static void _register_methods();

    Enemy();
    ~Enemy();

    void _init();
    void _ready();
    void _process(float delta);
    void _on_damage_taken(int damageTaken, NodePath damageSource);
    void set_hp(int newHp);
    int get_hp();
    int get_damage_dealt();
    void connect_to_player(NodePath playerPath);
};

}

#endif