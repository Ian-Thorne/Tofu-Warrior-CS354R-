#include "Familiar.h"

using namespace godot;

void Familiar::_register_methods() {
    register_method("_ready", &Familiar::_ready);
    register_method("_process", &Familiar::_process);
    register_method("_process", &Familiar::_process);
    register_method("waypoint_reached", &Familiar::waypoint_reached);
    register_method("connect_to_player", &Familiar::connect_to_player);
    register_method("_on_area_entered", &Familiar::_on_area_entered);
}

Familiar::Familiar() {}
Familiar::~Familiar() {}

void Familiar::_init() {
    targetNode = "";
    prevWaypoint = "";
    playerJoined = false;
    fallSpeed = 0;
    state = F_PATROL;
    waitTimer = 0;
}

void Familiar::_ready() {
    get_node("./Familiar")->connect("area_entered", this, "_on_area_entered");

    connect_to_player("/root/Spatial/1");
}

void Familiar::_process(float delta) {
    if(targetNode == "") {
        return_to_navmesh();
    }

    if(playerJoined){
        state_check();
    }
    
    Player* player; 
    NodePath closestToPlayer;
    RayCast* floorFinder = Object::cast_to<RayCast>(get_node("./FloorFinder"));

    switch(state){
        case F_PATROL:
            waitTimer += 1;
            if(waitTimer >= 50){
                waitTimer = 0;
            }
            else{
                targetNode = get_next_nearest_node();
            }
        break;

        case F_FREE_PURSUE:
            targetNode = get_node("/root/Spatial/1")->get_path();
        break;

        case F_GRID_PURSUE:
            if(!(floorFinder->is_colliding())) {
                move_and_slide(Vector3(0, -1, 0));
            }
            player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
            closestToPlayer = player->get_closest_waypoint();
            pathfind(closestToPlayer);
        break;

        case F_COLLECT_COIN:
            collect_a_coin();
        break;
    }

    //find the node referred to by targetNode
    Spatial* target = Object::cast_to<Spatial>(get_node(targetNode));
    //calculate the direction between this AIEnemy and the target
    Vector3 targetPos = target->get_global_transform().origin;
    Vector3 myPos = get_global_transform().origin;
    if(state == F_PATROL || state == F_GRID_PURSUE) {
        targetPos.y = 0;
        myPos.y = 0;
    }
    Vector3 dir = (targetPos - myPos).normalized();
    if(state == F_FREE_PURSUE && myPos.distance_to(targetPos) <= FAMILIAR_CHASE_DISTANCE) {
        return;
    }
    Area* cheatStopper = Object::cast_to<Area>(get_node("./CheatStopper"));
    if(get_node("/root/Spatial/1/Area") && cheatStopper->overlaps_area(get_node("/root/Spatial/1/Area"))) {
        return;
    }
    //move towards the target
    move_and_slide(dir * FAMILIAR_MOVEMENT_SPEED, Vector3(0, 1, 0), false, 4, DEFAULT_PLAYER_WALKABLE_ANGLE);
}

//the "Finite State Machine"
void Familiar::state_check(){
    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    Vector3 playerPos = player->get_global_transform().origin;
    playerPos.y = 0;
    Vector3 myPos = get_global_transform().origin;
    myPos.y = 0;
    float playerDist = myPos.distance_to(playerPos);

    switch(state){
        case F_PATROL:
            if(playerDist < FAMILIAR_FOLLOW_DISTANCE && player->get_has_familiar_bell()){
                return_to_navmesh();
                player->set_has_familiar();
                // state = F_GRID_PURSUE;
                state = F_FREE_PURSUE;
            }
        break;

        case F_GRID_PURSUE:
            if(playerDist < FAMILIAR_COIN_SEARCH_DISTANCE){
                state = F_COLLECT_COIN;
            }
        break;

        case F_FREE_PURSUE:
            if(playerDist > FAMILIAR_COIN_SEARCH_DISTANCE){
                return_to_navmesh();
                // state = F_GRID_PURSUE;
                state = F_FREE_PURSUE;
            }
            else if(get_valid_coins().size() != 0){
                state = F_COLLECT_COIN;
            }
        break;

        case F_COLLECT_COIN:
            if(playerDist > FAMILIAR_COIN_SEARCH_DISTANCE){
                return_to_navmesh();
                // state = F_GRID_PURSUE;
                state = F_FREE_PURSUE;
            }
            else if(get_valid_coins().size() == 0){
                state = F_FREE_PURSUE;
            }
        break;
    }
}

void Familiar::pathfind(NodePath goal) {
    Waypoint* goalWaypoint = Object::cast_to<Waypoint>(get_node(goal));
    Waypoint* previous = Object::cast_to<Waypoint>(get_node(prevWaypoint));
    
    //if this AIEnemy already reached the target Waypoint, stop moving
    if(goalWaypoint != previous){
        Array connectedWaypoints = previous->get_connected_waypoints();
        Waypoint* next = Object::cast_to<Waypoint>(get_node(connectedWaypoints[0]));
        for(int i = 0; i < connectedWaypoints.size(); i++){
            Waypoint* tempWaypoint = Object::cast_to<Waypoint>(get_node(connectedWaypoints[i]));
            if(tempWaypoint->get_dist(goalWaypoint->get_global_transform().origin) < next->get_dist(goalWaypoint->get_global_transform().origin)) {
                next = tempWaypoint;
            }
        }
        targetNode = next->get_path();
    }
}

Array Familiar::get_valid_coins(){
    Array tmp = get_tree()->get_nodes_in_group("Coins");
    Array ans = Array();

    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    for(int i = 0; i<tmp.size(); i++)
    {
        Coin* coin = Object::cast_to<Coin>(tmp[i]);
        if(coin->get_position().distance_to(get_global_transform().origin) < FAMILIAR_COIN_SEARCH_DISTANCE - player->get_global_transform().origin.distance_to(get_global_transform().origin)){
            ans.append(coin);
        };
    }

    return ans;
}

void Familiar::collect_a_coin(){
    Array coins = get_valid_coins();
    if(coins.size() != 0){
        Coin* coin = Object::cast_to<Coin>(coins[0]);
        NodePath goal = coin->get_path();
        targetNode = goal;
    }
}

void Familiar::waypoint_reached(NodePath waypointPath) {
    prevWaypoint = waypointPath;
}

//sets target back to nearest navmesh
void Familiar::return_to_navmesh() {
    Array navMesh = get_tree()->get_nodes_in_group("Waypoints");
    Vector3 myPos = get_global_transform().origin;

    if(navMesh.size() < 1)
        return;

    Waypoint* closestWaypoint = Object::cast_to<Waypoint>(navMesh[0]);
    for(int i = 0; i < navMesh.size(); i++) {
        Waypoint* tempWaypoint = Object::cast_to<Waypoint>(navMesh[i]);
        if(tempWaypoint->get_dist(myPos) < closestWaypoint->get_dist(myPos)){
            closestWaypoint = tempWaypoint;
        }
    }

    targetNode = closestWaypoint->get_path();
}

void Familiar::pursue_on_grid() {
    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    NodePath closestToPlayer = player->get_closest_waypoint();
    //pathfind(closestToPlayer);
}

NodePath Familiar::get_next_nearest_node(){
    Array navmesh = get_tree()->get_nodes_in_group("Waypoints");
    if(navmesh.size() == 0)
        return NULL;
    Waypoint* ans =  Object::cast_to<Waypoint>(navmesh[0]);
    for(int i=1; i<navmesh.size(); i++){
        Waypoint* waypoint = Object::cast_to<Waypoint>(navmesh[i]);
        if(waypoint->get_dist(get_global_transform().origin) < ans->get_dist(get_global_transform().origin)
            && !(waypoint->get_path() == prevWaypoint)){

            ans = waypoint;
        }
    }
    return ans->get_path();
}

void Familiar::_on_area_entered(Area* other){
    if(other->get_name() == "Coin") {
        Coin* coin = Object::cast_to<Coin>(other->get_parent());
        Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
        player->change_gold(1);
        coin->destroy();
        Object::cast_to<AudioStreamPlayer>(get_node("/root/Spatial/1/ChaChing"))->call("play", 0.0);
    }

}

Vector3 Familiar::apply_gravity(Vector3 direction) {
    if(fallSpeed + DEFAULT_GRAVITY < DEFAULT_PLAYER_FALL_LIMIT) {
        fallSpeed += DEFAULT_GRAVITY;
    } else {
        fallSpeed = DEFAULT_PLAYER_FALL_LIMIT;
    }
    direction.y = fallSpeed;
    return direction;
}

void Familiar::set_state(FamiliarState newState) {
    state = newState;
}

FamiliarState Familiar::get_state() {
    return state;
}

void Familiar::connect_to_player(NodePath playerPath) {
    Godot::print("FAMILIAR AI CONNECTING");
    playerJoined = true;
}