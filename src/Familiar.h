#ifndef Familiar_H
#define Familiar_H

#include "constants.h"
#include "Player.h"
#include "Waypoint.h"
#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <Area.hpp>
#include <AudioStreamPlayer.hpp>
#include <Array.hpp>
#include <SceneTree.hpp>
#include <RayCast.hpp>
#include <string>
#include "Coin.h"

namespace godot {

class Familiar : public KinematicBody {
    GODOT_CLASS(Familiar, KinematicBody);

private:
    float fallSpeed;
    int waitTimer;
    bool playerJoined;
    NodePath targetNode;        //the node this Familiar is moving towards (can be
                                //a Waypoint or a Player)
    NodePath prevWaypoint;      //the last Waypoint this Familiar navigated to
    NodePath patrolTarget;      //the Waypoint this Familiar should move towards if patroling

public:
    static void _register_methods();

    Familiar();
    ~Familiar();

    FamiliarState state; 

    void _init();
    void _ready();
    void _process(float delta);
    void _on_area_entered(Area* other);

    void state_check();
    Array get_valid_coins();

    void connect_to_player(NodePath playerPath);
    void return_to_navmesh();
    void pursue_on_grid();
    void pathfind(NodePath goal);
    void waypoint_reached(NodePath waypointPath);
    void collect_a_coin();
    NodePath get_next_nearest_node();

    Vector3 apply_gravity(Vector3 direction);
    
    void set_state(FamiliarState newState);
    FamiliarState get_state();

};

}

#endif