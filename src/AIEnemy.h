#ifndef AIENEMY_H
#define AIENEMY_H

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
#include <MeshInstance.hpp>
#include <SpatialMaterial.hpp>
#include <Color.hpp>
#include <CollisionShape.hpp>


namespace godot {

class AIEnemy : public KinematicBody {
    GODOT_CLASS(AIEnemy, KinematicBody);

private:
    EnemyState state;
    EnemyState lastState;
    int hp;
    int maxHp;
    int damageDealt;
    int goldValue;
    float speed;
    bool playerJoined;
    bool patrolClockwise;
    bool onWaypoint;
    NodePath targetNode;        //the node this AIEnemy is moving towards (can be
                                //a Waypoint or a Player)
    NodePath prevWaypoint;      //the last Waypoint this AIEnemy navigated to
    NodePath patrolTarget;      //the Waypoint this AIEnemy should move towards if patroling
    int damageCD;
    MeshInstance* meshInstance;
    Color baseColor;
    Color newColor;
    SpatialMaterial* myMaterial;

public:
    static void _register_methods();

    AIEnemy();
    ~AIEnemy();

    void _init();
    void _ready();
    void _process(float delta);
    void _on_damage_taken(int damageTaken, NodePath damageSource);
    void update_cooldowns();
    void set_direction(NodePath target);
    void set_hp(int newHp);
    int get_hp();
    void set_damage_dealt(int newDamageDealt);
    int get_damage_dealt();
    void set_gold_value(int newGoldValue);
    EnemyState get_state();
    void set_on_waypoint(bool on);
    //sets this AIEnemy's last reached node for pathfinding purposes
    void set_prev_waypoint(NodePath newPrevWaypoint);
    //triggered by hitting a Waypoint while pathfinding
    void waypoint_reached(NodePath waypointPath);
    //connects this AIEnemy to the player when they are loaded in
    void connect_to_player(NodePath playerPath);

    /* AI behavior methods */

    //causes this AIEnemy to navigate to the nearest Waypoint in the navmesh
    void return_to_navmesh();
    //causes this AIEnemy to navigate to a Waypoint in a corner of the navmesh
    void return_to_corner();

    //checks the relevant conditions and updates this AIEnemy's state accordingly
    void update_state();
    //determines this AIEnemy's behavior, given its current state
    void determine_behavior();

    //causes this AIEnemy to find a path to the passed-in Waypoint
    void pathfind(NodePath goal);
    //moves this AIEnemy based on the behavior it chose
    void move(float delta);

    //causes this AIEnemy to wander the navmesh (somewhat) randomly
    //only called when this AIEnemy reaches a Waypoint while its state is PATROL
    void patrol(NodePath waypointPath);
    //causes this AIEnemy to navigate along a path in the navmesh towards the player
    //only called when this AIEnemy reaches a Waypoint while its state is PATROL
    void pursue_on_grid();
    //causes this AIEnemy to follow the player, ignoring the navmesh
    //called on every frame while this AIEnemy's state is PLAYER_PURSUE
    void pursue_off_grid();
    //causes this AIEnemy to run away from the player
    //called on every frame while this AIEnemy's state is FLEE
    void run_from_player();

    /* Signal response methods */

    //prevents two AIEnemies from colliding
    void _on_forcefield_entered(Area* other);
};

}

#endif