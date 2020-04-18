#include "AIEnemy.h"

using namespace godot;

void AIEnemy::_register_methods() {
    register_method("_ready", &AIEnemy::_ready);
    register_method("_process", &AIEnemy::_process);
    register_method("_on_damage_taken", &AIEnemy::_on_damage_taken);
    register_method("get_damage_dealt", &AIEnemy::get_damage_dealt);
    register_method("connect_to_player", &AIEnemy::connect_to_player);
    register_method("set_hp", &AIEnemy::set_hp, GODOT_METHOD_RPC_MODE_REMOTESYNC);
    register_method("get_hp", &AIEnemy::get_hp);
    register_method("get_state", &AIEnemy::get_state);
    register_method("set_on_waypoint", &AIEnemy::set_on_waypoint);
    register_method("waypoint_reached", &AIEnemy::waypoint_reached);
    register_method("_on_forcefield_entered", &AIEnemy::_on_forcefield_entered);

    register_property<AIEnemy, int>("hp", &AIEnemy::hp, DEFAULT_ENEMY_HP);
    register_property<AIEnemy, int>("damageDealt", &AIEnemy::damageDealt, DEFAULT_ENEMY_DAMAGE_DEALT);
    register_property<AIEnemy, int>("goldValue", &AIEnemy::goldValue, DEFAULT_ENEMY_GOLD_VALUE);
    register_property<AIEnemy, float>("speed", &AIEnemy::speed, DEFAULT_ENEMY_SPEED);

    register_signal<AIEnemy>((char*) "enemy_died");
}

AIEnemy::AIEnemy() {}
AIEnemy::~AIEnemy() {}

void AIEnemy::_init() {
    hp = DEFAULT_ENEMY_HP;
    damageDealt = DEFAULT_ENEMY_DAMAGE_DEALT;
    goldValue = DEFAULT_ENEMY_GOLD_VALUE;
    speed = DEFAULT_ENEMY_SPEED;
    targetNode = "";
    prevWaypoint = "";
    playerJoined = false;
    patrolClockwise = true;
    onWaypoint = false;
    state = PATROL;
    lastState = state;
}

void AIEnemy::_ready() {

    //set maxHp to whatever the in-editor HP was set to
    maxHp = hp;

    //set the initial patrol target to the closest corner
    Array corners = get_tree()->get_nodes_in_group("CornerWaypoints");
    Vector3 myPos = get_global_transform().origin;
    Waypoint* closestCorner = Object::cast_to<Waypoint>(corners[0]);
    for(int i = 0; i < corners.size(); i++) {
        Waypoint* tempCorner = Object::cast_to<Waypoint>(corners[i]);
        if(tempCorner->get_dist(myPos) < closestCorner->get_dist(myPos)) {
            closestCorner = tempCorner;
        }
    }
    patrolTarget = closestCorner->get_path();
    set_direction(patrolTarget);

    //set up the signal that prevents enemies from colliding
    Area* forceField = Object::cast_to<Area>(get_node("./ForceField"));
    forceField->connect("area_entered", this, "_on_forcefield_entered");

    //set up the signal to the game when this enemy dies
    connect("enemy_died", get_node("/root/GameManager"), "try_end_round");

    baseColor = cast_to<SpatialMaterial>((*(cast_to<MeshInstance>(get_node("MeshInstance"))->get_surface_material(0))))->get_albedo();
    newColor = Color(0.94902, 0.266667, 0.537255);
    meshInstance = Object::cast_to<MeshInstance>(get_node("MeshInstance"));
    myMaterial = SpatialMaterial::_new();
    myMaterial->set_albedo(baseColor);
    get_node("MeshInstance")->call("set_surface_material", 0, myMaterial);

    if(get_node("/root/Spatial/1")) {
        playerJoined = true;
    }
}

void AIEnemy::_process(float delta) {
    if(targetNode == "") {
        return_to_navmesh();
    }

    set_direction(targetNode);

    update_state();

    determine_behavior();

    move(delta);

    update_cooldowns();

    //prevent the player from riding around on this AIEnemy, throwing them off
    Area* standStopper = Object::cast_to<Area>(get_node("./StandStopper"));
    if(standStopper->overlaps_area(get_node("/root/Spatial/1/Area"))) {
        Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
        player->set_velocity(player->get_velocity() + Vector3(0, 5, 0));
        player->change_hp(-damageDealt);
    }
}

void AIEnemy::_on_damage_taken(int damage, NodePath damageSource) {
    //find the player and calculate the AIEnemy's distance from them
    Player* player = Object::cast_to<Player>(get_node(damageSource));
    Area* attackArea = Object::cast_to<Area>(get_node(NodePath((String(damageSource) + "/AttackArea"))));
    Area* enemyArea = Object::cast_to<Area>(get_node("AIEnemy"));

    //if the player is close enough, update the AIEnemy's hp based on
    //the damage taken
    //if(enemyArea->overlaps_area(attackArea)) {
        //play the sound effect
        player->play_oof();

        //temporary damage color
        myMaterial->set_albedo(newColor);
        damageCD = 15;

        //update the AIEnemy's health accordingly
        int newHp = get_hp() - damage;
        if(newHp <= 0) {
            //if this hit will kill the enemy, give the player gold and end the round
            //if this was the last enemy to die
            player->change_gold(goldValue);
            remove_from_group("Enemies");
            call_deferred("emit_signal", "enemy_died");
            queue_free();
        }
        set_hp(newHp);
    //}  
}

void AIEnemy::update_cooldowns() {
    if(damageCD > 0) {
        damageCD--;
        if(damageCD <= 0){
            myMaterial->set_albedo(baseColor);
        }
    }
}

void AIEnemy::set_direction(NodePath target){
    Vector3 change = Object::cast_to<Spatial>(get_node(target))->get_global_transform().origin;
    look_at(Vector3(change.x, get_global_transform().origin.y, change.z), Vector3(0, 1, 0));
}

void AIEnemy::set_hp(int newHp) {
    hp = newHp;
    if(newHp <= 0) {
        queue_free();
    }
}

int AIEnemy::get_hp() {
    return hp;
}

void AIEnemy::set_damage_dealt(int newDamageDealt) {
    damageDealt = newDamageDealt;
}

int AIEnemy::get_damage_dealt() {
    return damageDealt;
}

void AIEnemy::set_gold_value(int newGoldValue) {
    goldValue = newGoldValue;
}

EnemyState AIEnemy::get_state() {
    return state;
}

void AIEnemy::set_prev_waypoint(NodePath newPrevWaypoint) {
    prevWaypoint = newPrevWaypoint;
}

void AIEnemy::set_on_waypoint(bool on) {
    onWaypoint = on;
}

void AIEnemy::waypoint_reached(NodePath waypointPath) {
    //reaching waypoints should only matter when formally pathfinding
    if(state == GRID_PURSUE && lastState == PLAYER_PURSUE) {
        lastState = state;
    }
    if(state == PATROL || state == GRID_PURSUE) {
        set_prev_waypoint(waypointPath);
        if(state == PATROL) {
            patrol(waypointPath);
        } else if(state == GRID_PURSUE) {
            pursue_on_grid();
        }
    }
}

void AIEnemy::connect_to_player(NodePath playerPath) {
    Godot::print("AI CONNECTING");
    playerJoined = true;
    get_node(playerPath)->connect("attacking", this, "_on_damage_taken");
}

//sets target back to nearest navmesh
void AIEnemy::return_to_navmesh() {
    Array navMesh = get_tree()->get_nodes_in_group("Waypoints");
    Vector3 myPos = get_global_transform().origin;

    //if the navmesh is empty, do nothing; shouldn't happen
    if(navMesh.size() < 1)
        return;

    //find the closest Waypoint in the navmesh
    Waypoint* closestWaypoint = Object::cast_to<Waypoint>(navMesh[0]);
    for(int i = 0; i < navMesh.size(); i++) {
        Waypoint* tempWaypoint = Object::cast_to<Waypoint>(navMesh[i]);
        if(tempWaypoint->get_dist(myPos) < closestWaypoint->get_dist(myPos) && !tempWaypoint->is_taken()) {
            closestWaypoint = tempWaypoint;
        }
    }

    closestWaypoint->take();
    if(!(targetNode == "")) {
        get_node(targetNode)->call("untake");
    }
    targetNode = closestWaypoint->get_path();
}

void AIEnemy::return_to_corner() {
    Array corners = get_tree()->get_nodes_in_group("CornerWaypoints");
    Vector3 myPos = get_global_transform().origin;
    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    Vector3 playerPos = player->get_global_transform().origin;

    Array closeCorners;
    Waypoint* targetWaypoint;

    //if there are no corner waypoints, do nothing; shouldn't happen
    if(corners.size() < 1) {
        return;
    }

    //compile an array of corner Waypoints within a certain distance
    Waypoint* currCorner;
    for(int i = 0; i < corners.size(); i++) {
        currCorner = Object::cast_to<Waypoint>(corners[i]);
        if(currCorner->get_dist(myPos) <= ENEMY_MAX_CORNER_DISTANCE) {
            closeCorners.append(currCorner);
        }
    }

    if(closeCorners.size() == 0) {
        return_to_navmesh();
    } else {
        //move towards the close corner that is furthest from the player
        Waypoint* furthestCorner = Object::cast_to<Waypoint>(closeCorners[0]);
        targetWaypoint = furthestCorner;
        for(int i = 0; i < closeCorners.size(); i++) {
            Waypoint* tempCorner = Object::cast_to<Waypoint>(closeCorners[i]);
            if(tempCorner->get_dist(playerPos) > furthestCorner->get_dist(playerPos) && !tempCorner->is_taken()) {
                targetWaypoint = tempCorner;
            }
        }

        targetWaypoint->take();
        if(!(targetNode == "")) {
            get_node(targetNode)->call("untake");
        }
        targetNode = targetWaypoint->get_path();
        patrolTarget = targetWaypoint->get_path();
    }
}

//the "Finite State Machine"
void AIEnemy::update_state() {
    if(playerJoined) {
        //calculate required values
        Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
        Vector3 playerPos = player->get_global_transform().origin;
        Vector3 myPos = get_global_transform().origin;
        bool playerOnRaisedPlatform = playerPos.y - myPos.y >= 5;
        playerPos.y = 0;
        myPos.y = 0;
        float playerDistance = myPos.distance_to(playerPos);
        float percentHp = ((float) hp) / ((float) maxHp);

        switch(state) {
            case PATROL:
                if(percentHp > LOW_HP_RANGE) {
                    if(playerDistance <= CLOSE_DISTANCE && !playerOnRaisedPlatform) {
                        get_node(targetNode)->call("untake");
                        lastState = state;
                        state = PLAYER_PURSUE;
                    } else if(playerDistance <= MEDIUM_DISTANCE && !playerOnRaisedPlatform) {
                        lastState = state;
                        state = GRID_PURSUE;
                    }
                } else {
                    if(playerDistance <= CLOSE_DISTANCE && !playerOnRaisedPlatform) {
                        get_node(targetNode)->call("untake");
                        lastState = state;
                        state = FLEE;
                    }
                }
                break;
            case GRID_PURSUE:
                if(playerDistance > MEDIUM_DISTANCE || playerOnRaisedPlatform) {
                    patrolClockwise = !patrolClockwise;
                    lastState = state;
                    state = PATROL;
                } else if(playerDistance <= CLOSE_DISTANCE && !playerOnRaisedPlatform) {
                    if(percentHp > LOW_HP_RANGE) {
                        get_node(targetNode)->call("untake");
                        lastState = state;
                        state = PLAYER_PURSUE;
                    } else {
                        get_node(targetNode)->call("untake");
                        lastState = state;
                        state = FLEE;
                    }
                }
                break;
            case PLAYER_PURSUE:
                
                if(playerDistance > MEDIUM_DISTANCE || playerOnRaisedPlatform) {
                    patrolClockwise = !patrolClockwise;
                    lastState = state;
                    state = PATROL;
                    return_to_navmesh();
                }

                if(playerDistance > CLOSE_DISTANCE || playerOnRaisedPlatform) {
                    lastState = state;
                    state = GRID_PURSUE;
                    return_to_navmesh();
                }
                if(percentHp < LOW_HP_RANGE) {
                    lastState = state;
                    state = FLEE;
                }
                break;
            case FLEE:
                if(playerDistance > CLOSE_DISTANCE && !playerOnRaisedPlatform) {
                    patrolClockwise = !patrolClockwise;
                    lastState = state;
                    state = PATROL;
                    return_to_navmesh();
                }
                break;
            default:
                //this shouldn't happen
                Godot::print("WHAT HAPPENED?");
                break;
        }
    }
}

void AIEnemy::determine_behavior() {
    switch(state) {
        case PATROL:
            // Godot::print("PATROLING");
            if(onWaypoint) {
                pathfind(patrolTarget);
            }
            break;
        case GRID_PURSUE:
            // Godot::print("GRID PURSUING");
            if(onWaypoint) {
                pursue_on_grid();
            }
            break;
        case PLAYER_PURSUE:
            // Godot::print("PLAYER PURSUING");
            pursue_off_grid();
            break;
        case FLEE:
            // Godot::print("FLEEING");
            run_from_player();
            break;
        default:
            //this shouldn't happen
            Godot::print("WHAT HAPPENED?");
            break;
    }
}

void AIEnemy::move(float delta) {
    //find the node referred to by targetNode
    Spatial* target = Object::cast_to<Spatial>(get_node(targetNode));
    //calculate the direction between this AIEnemy and the target
    Vector3 targetPos = target->get_global_transform().origin;
    targetPos.y = 0;
    Vector3 myPos = get_global_transform().origin;
    myPos.y = 0;
    Vector3 dir = (targetPos - myPos).normalized();

    //if the enemy is fleeing, it should move in the opposite direction
    if(state == FLEE) {
        dir = dir * -1;
    }

    //apply gravity
    dir.y = DEFAULT_GRAVITY;

    //move the RayCast in front of this AIEnemy in whatever direction it's moving
    RayCast* myRayCast = Object::cast_to<RayCast>(get_node("./RayCast"));
    myRayCast->set_translation(dir / 2);

    //move this AIEnemy, as long as it won't run off of the level
    if(myRayCast->is_colliding()) {
        move_and_slide(dir * speed, Vector3(0, 1, 0), false, 4, DEFAULT_PLAYER_WALKABLE_ANGLE);
    }
}

void AIEnemy::pathfind(NodePath goal) {
    Waypoint* goalWaypoint = Object::cast_to<Waypoint>(get_node(goal));
    Waypoint* previous = Object::cast_to<Waypoint>(get_node(prevWaypoint));
    
    //if this AIEnemy already reached the target Waypoint, stop moving
    if(goalWaypoint != previous){
        Array connectedWaypoints = previous->get_connected_waypoints();
        Waypoint* next = Object::cast_to<Waypoint>(get_node(connectedWaypoints[0]));
        for(int i = 0; i < connectedWaypoints.size(); i++){
            Waypoint* tempWaypoint = Object::cast_to<Waypoint>(get_node(connectedWaypoints[i]));
            if(tempWaypoint->get_dist(goalWaypoint->get_global_transform().origin) < next->get_dist(goalWaypoint->get_global_transform().origin)
                    && !tempWaypoint->is_taken()) {
                next = tempWaypoint;
            }
        }
        next->take();
        if(!(targetNode == "")) {
            get_node(targetNode)->call("untake");
        }
        targetNode = next->get_path();
    }
}

void AIEnemy::patrol(NodePath waypointPath) {
    if(waypointPath == "/root/Spatial/Waypoints/Waypoint1") {
        if(patrolClockwise) {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint9";
        } else {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint73";
        }
    } else if(waypointPath == "/root/Spatial/Waypoints/Waypoint9") {
        if(patrolClockwise) {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint81";
        } else {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint1";
        }
    } else if(waypointPath == "/root/Spatial/Waypoints/Waypoint81") {
        if(patrolClockwise) {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint73";
        } else {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint9";
        }
    } else if(waypointPath == "/root/Spatial/Waypoints/Waypoint73") {
        if(patrolClockwise) {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint1";
        } else {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint81";
        }
    } else if(waypointPath == "/root/Spatial/Waypoints/Waypoint21") {
        if(patrolClockwise) {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint25";
        } else {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint57";
        }
    } else if(waypointPath == "/root/Spatial/Waypoints/Waypoint25") {
        if(patrolClockwise) {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint61";
        } else {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint21";
        }
    } else if(waypointPath == "/root/Spatial/Waypoints/Waypoint61") {
        if(patrolClockwise) {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint57";
        } else {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint25";
        }
    } else if(waypointPath == "/root/Spatial/Waypoints/Waypoint57") {
        if(patrolClockwise) {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint21";
        } else {
            patrolTarget = "/root/Spatial/Waypoints/Waypoint61";
        }
    }
    pathfind(patrolTarget);
}

void AIEnemy::pursue_on_grid() {
    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    NodePath closestToPlayer = player->get_closest_waypoint();
    pathfind(closestToPlayer);
}

//AKA: attack
void AIEnemy::pursue_off_grid() {
    //set the player as the target
    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    targetNode = player->get_path();
}

void AIEnemy::run_from_player() {
    //set the player as the target (since state is FLEE when move() is called
    //this AIEnemy will move in the opposite direction of the target)
    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    targetNode = player->get_path();
}

void AIEnemy::_on_forcefield_entered(Area* other) {
    if(other->get_name() == "AIEnemy" || other->get_name() == "ForceField") {
        AIEnemy* otherAgent = Object::cast_to<AIEnemy>(other->get_parent());
        Vector3 otherPos = otherAgent->get_global_transform().origin;
        Vector3 myPos = get_global_transform().origin;
        Vector3 runAway = (myPos - otherPos).normalized();
        translate(runAway / 20);
    }
}