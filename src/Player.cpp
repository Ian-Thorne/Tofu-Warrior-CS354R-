#include "Player.h"

using namespace godot;

void Player::_register_methods(){
	register_method("_ready", &Player::_ready, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_physics_process", &Player::_physics_process, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("minit", &Player::minit, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_area_entered", &Player::_on_area_entered);
    register_method("win", &Player::win);
    register_method("lose", &Player::lose);
    register_method("go_to_end_screen", &Player::go_to_end_screen, GODOT_METHOD_RPC_MODE_REMOTESYNC);
    register_method("set_win_status", &Player::set_win_status, GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("get_win_status", &Player::get_win_status);
    register_method("get_has_sword", &Player::get_has_sword);
    register_method("get_has_familiar_bell", &Player::get_has_familiar_bell);
    register_method("set_has_familiar", &Player::set_has_familiar);
    register_method("get_has_familiar", &Player::get_has_familiar);
    register_method("stop_moving", &Player::stop_moving);
    register_method("find_closest_waypoint", &Player::find_closest_waypoint);
    register_method("get_closest_waypoint", &Player::get_closest_waypoint);
    register_method("set_velocity", &Player::set_velocity);
    register_method("get_velocity", &Player::get_velocity);
    register_method("turn_y", &Player::turn_y, GODOT_METHOD_RPC_MODE_PUPPETSYNC);

	register_property<Player, int>("hp", &Player::change_hp, &Player::get_hp, DEFAULT_PLAYER_HP, GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Player, float>("walkableAngle", &Player::walkableAngle, DEFAULT_PLAYER_WALKABLE_ANGLE, GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Player, float>("gravity", &Player::gravity, DEFAULT_GRAVITY, GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Player, float>("airControl", &Player::airControl, DEFAULT_PLAYER_AIR_CONTROL, GODOT_METHOD_RPC_MODE_DISABLED);
	register_property<Player, float>("jumpHeight", &Player::jumpHeight, DEFAULT_PLAYER_JUMP_HEIGHT, GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Player, int>("gliderFactor", &Player::gliderFactor, DEFAULT_GLIDER_FACTOR, GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Player, int>("gliderFallLimit", &Player::gliderFallLimit, DEFAULT_GLIDER_FALL_LIMIT, GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Player, bool>("hasStrafeControls", &Player::hasStrafeControls, true, GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Player, Vector3>("slavePosition", &Player::slavePosition, Vector3(0,0,0), GODOT_METHOD_RPC_MODE_PUPPET);
    register_property<Player, int64_t>("nodeName", &Player::nodeName, 0, GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Player, Vector3>("slaveVelocity", &Player::slaveVelocity, Vector3(0,0,0), GODOT_METHOD_RPC_MODE_PUPPET);
    register_property<Player, NodePath>("closestWaypoint", &Player::closestWaypoint, "");

    register_signal<Player>((char*) "attacking", "damage", GODOT_VARIANT_TYPE_INT, "path", GODOT_VARIANT_TYPE_NODE_PATH);
    register_signal<Player>((char*) "health_changed", "newHp", GODOT_VARIANT_TYPE_INT, "maxHp", GODOT_VARIANT_TYPE_INT);
    register_signal<Player>((char*) "gold_count_changed", "newGoldCount", GODOT_VARIANT_TYPE_INT);
    register_signal<Player>((char*) "item_collected", "item", GODOT_VARIANT_TYPE_INT);
}

Player::Player() {}
Player::~Player() {}

void Player::_init() {
    state = IDLE;
    velocity = Vector3(0, 0, 0);
    lastCoordinates = Vector3(0, 10, 0);
    currSpeed = 0;
    walkableAngle = DEFAULT_PLAYER_WALKABLE_ANGLE;
    gravity = DEFAULT_GRAVITY;
    currGravity = DEFAULT_GRAVITY;
    airControl = DEFAULT_PLAYER_AIR_CONTROL;
    jumpHeight = DEFAULT_PLAYER_JUMP_HEIGHT;
    currJumps = 0;
    gliderFactor = DEFAULT_GLIDER_FACTOR;
    gliderFallLimit = DEFAULT_GLIDER_FALL_LIMIT;
    didIWin = false;
    hasStrafeControls = true;
    jumpCD = 0;
    attackCD = 0;
    soundControlCD = 0;
    damageTakenCD = 0;
    waypointControlCD = 0;
    slavePosition = Vector3(0,0,0);
    nodeName = 0;
    closestWaypoint = "";
    mpHp = DEFAULT_PLAYER_HP;
    mpMaxHp = DEFAULT_PLAYER_HP;
    mpGold = 0;
    mpAttack = 0;
    mpMaxJumps = DEFAULT_PLAYER_MAX_EXTRA_JUMPS;
    mpMaxSpeed = DEFAULT_PLAYER_SPEED;
    mpHasGlider = false;
    mpHasSword = false;
}

void Player::_ready() {
    myStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));

    walkableAngle = (walkableAngle * 3.14159265358979) / 180;
    ray = Object::cast_to<RayCast>(get_node("RayCast"));
    sword = Object::cast_to<Spatial>(get_node("Sword"));
    attackArea = Object::cast_to<Area>(get_node("AttackArea"));
    attackRay = Object::cast_to<RayCast>(get_node("AttackArea/RayCast"));
    

    //start the background music
    AudioStreamPlayer* myPlayer = Object::cast_to<AudioStreamPlayer>(get_node("BackGroundMusic"));
    myPlayer->play(0.0);

    get_node("./Area")->connect("area_entered", this, "_on_area_entered");

    //connect signals to the GUI
    Node* GUI = get_node("/root/Spatial/Control");
    this->connect("health_changed", GUI, "_on_hp_changed");
    this->connect("gold_count_changed", GUI, "_on_gold_count_changed");
    this->connect("item_collected", GUI, "_on_item_collected");

    //connect signals to the enemies
    get_tree()->call_group("Enemies", "connect_to_player", get_path());

    //add this player to the group of players (might need to remove at some point)
    add_to_group("Players");

    Input* input = Input::get_singleton();
    input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);

    //if playing in single player mode perform required checks
    if(is_singleplayer()) {
        //update model to be consistent with PlayerState
        if(get_has_sword()) {
            sword->show();
        }
        if(get_attack_range() > 1) {
            change_attack_range(0);
        }
    }
}

void Player::_physics_process(float delta){
    if(is_network_master())
    {

        //new stuff
        update_cooldowns();
        Input* input = Input::get_singleton();

        //if the player is mid-jump or mid-fall, update their state accordingly
        if(velocity.y > 0 && !is_on_floor() && state != HANGING) {
            state = JUMPING;
        } 
        
        else if(velocity.y < 0 && !is_on_floor() && state != HANGING) {
            state = FALLING;
        }

        velocity = compute_translation(velocity, input, delta);
        velocity = compute_jumping(velocity, input);
        velocity = compute_air_mobility(velocity, input, delta);
        velocity = compute_ledges(velocity, input);

        rset_unreliable("slavePosition", get_translation());
        rset("slaveVelocity", velocity);

        velocity = move_and_slide(velocity, Vector3(0, 1, 0), false, 4, walkableAngle);

        compute_attack(input);
        compute_sound_controls(input);
        find_closest_waypoint();
        //Godot::print(closestWaypoint);

        //if the player has fallen too far, end the game
        if(get_translation().y < -20) {
            lose();
        }

        if(get_node("/root/Network")->get("singlePlayerMode") && input->is_action_pressed("ui_toggle_waypoints") && waypointControlCD <= 0) {
            get_tree()->call_group("Waypoints", "toggle_visibility");
            waypointControlCD = DEFAULT_WAYPOINT_CONTROL_COOLDOWN;
        }
    }
    else{
        move_and_slide(slaveVelocity, Vector3(0, 1, 0), false, 4, walkableAngle);
        set_translation(slavePosition);
    }

    if(get_tree()->is_network_server()){
        get_node("/root/Network")->call("update_position", get_name().to_int(), get_translation());
    }
}

/*
 * Decrement all cooldowns that the Player is keeping track of, if
 * they haven't yet reached 0.
 */
void Player::update_cooldowns() {
    if(jumpCD > 0) {
        jumpCD--;
    }
    if(attackCD > 0) {
        attackCD--;
        if(attackCD <= 3){
            sword->set_rotation_degrees(Vector3(34.158, 153.972, -6.835));
            sword->set_translation(Vector3(0.6, 0.314, -0.448));
        }
    }
    if(soundControlCD > 0) {
        soundControlCD--;
    }
    if(damageTakenCD > 0) { 
        damageTakenCD--;
    }
    if(waypointControlCD > 0) {
        waypointControlCD--;
    }
}

/*
 * Takes in the player's velocity and input and a delta and completes all
 * WASD movement computations, then returns a Vector3 representing the
 * player's updated velocity.
 */
Vector3 Player::compute_translation(Vector3 direction, Input* input, float delta) {
    Vector3 newVelocity = direction;
    if(state != HANGING) {
        Vector3 dir = Vector3(0,0,0);
        Transform cam = Object::cast_to<Camera>(get_node("OuterGimbal/InnerGimbal/Camera"))->get_global_transform();
        //if the player isn't inputting anything and they're not jumping or falling, set state to IDLE
        if(!(input->is_action_pressed("ui_up")) && !(input->is_action_pressed("ui_down")) &&
                !(input->is_action_pressed("ui_left")) && !(input->is_action_pressed("ui_right")) &&
                !(input->is_action_pressed("ui_select")) && is_on_floor()) {
            state = IDLE;
        } 
        else if(is_on_floor()) {
            state = WALKING;
        }
        //update the player's velocity and ledge detecting ray cast location,
        //according to user input
        if(input->is_action_pressed("ui_up")) {
            dir -= cam.basis[2];
            dir.y = 0;
            dir = dir.normalized();
            Transform rayTransform = ray->get_global_transform();
            Vector3 rayTranslation = get_global_transform().origin + (dir / 3);
            rayTransform.origin = rayTranslation;
            ray->set_global_transform(rayTransform);
        }
        if(input->is_action_pressed("ui_down")) {
            dir += cam.basis[2];
            dir.y = 0;
            dir = dir.normalized();
            Transform rayTransform = ray->get_global_transform();
            Vector3 rayTranslation = get_global_transform().origin + (dir / 3);
            rayTransform.origin = rayTranslation;
            ray->set_global_transform(rayTransform);
        }
        if(input->is_action_pressed("ui_left")) {
            //if the player has strafe controls on, move them, if not
            //just rotate them
            if(hasStrafeControls) {
                dir -= cam.basis[0];
                dir.y = 0;
                dir = dir.normalized();
                Transform rayTransform = ray->get_global_transform();
                Vector3 rayTranslation = get_global_transform().origin + (dir / 3);
                rayTransform.origin = rayTranslation;
                ray->set_global_transform(rayTransform);
            } else {
                rotate_y(0.1);
            }
        }
        if(input->is_action_pressed("ui_right")) {
            //if the player has strafe controls on, move them, if not
            //just rotate them
            if(hasStrafeControls) {
                dir += cam.basis[0];
                dir.y = 0;
                dir = dir.normalized();
                Transform rayTransform = ray->get_global_transform();
                Vector3 rayTranslation = get_global_transform().origin + (dir / 3);
                rayTransform.origin = rayTranslation;
                ray->set_global_transform(rayTransform);
            } else {
                rotate_y(-0.1);
            }
        }
        //using the dir relative to camera position calculated above,
        //change the player's velocity accordingly
        Vector3 horizontalVelocity = newVelocity;
        horizontalVelocity.y = 0;
        Vector3 newPos = dir * currSpeed;
        int accel;
        if(dir.dot(horizontalVelocity) > 0) {
            accel = DEFAULT_PLAYER_ACCELERATION;
        } else {
            accel = DEFAULT_PLAYER_DECELERATION;
        }
        horizontalVelocity = horizontalVelocity.linear_interpolate(newPos, accel * delta);
        newVelocity.x = horizontalVelocity.x;
        newVelocity.z = horizontalVelocity.z;
    }
    return newVelocity;
}

/*
 * Takes in the player's velocity and input, and completes all
 * jumping-related computations, then returns a Vector3 representing
 * the player's updated velocity.
 */
Vector3 Player::compute_jumping(Vector3 direction, Input* input) {
    Vector3 newVelocity = direction;
    //if the player is trying to jump, has jumps left, and the jump cooldown
    //has finished, make them jump
    if(input->is_action_pressed("ui_select") && currJumps < get_max_jumps() && jumpCD <= 0){
        //if the player is hanging from a ledge, reset their gravity
        if(state == HANGING){
            currGravity = gravity;
        }
        state = JUMPING;
        newVelocity.y = jumpHeight;
        if(!is_on_floor()) {
            currJumps++;
        }
        jumpCD = DEFAULT_JUMP_COOLDOWN;
    }
    return newVelocity;
}

/*
 * Takes in the player's velocity and input and completes all
 * air-control-related computations, including: gravity calculations,
 * air-resistance, and gliding. Returns a Vector3 representing the
 * player's updated velocity.
 */
Vector3 Player::compute_air_mobility(Vector3 direction, Input* input, float delta) {
    Vector3 newVelocity = direction;
    //don't calculate any air-mobility if the player is hanging from a ledge
    if(state != HANGING) {
        if(is_on_floor()) {
            //if the player is idle, walking, or sliding, reset their gravity
            //and jumps and revert their speed from the modified air-control version
            currGravity = gravity;
            currSpeed = get_max_move_speed();
            currJumps = 0;
            direction.y = 0;
        } else {
            //if the player is in the air, impede their speed to reduce air-control
            currSpeed = get_max_move_speed() * airControl;
            //apply gravity to the player, up to a terminal speed
            int terminalVelocity;
            if(get_has_glider()) {
                //if the player has the glider, limit their terminal speed
                terminalVelocity = gliderFallLimit;
            } else {
                terminalVelocity = DEFAULT_PLAYER_FALL_LIMIT;
            }
            if(newVelocity.y + currGravity > terminalVelocity) {
                newVelocity.y += currGravity;
            } else {
                newVelocity.y = terminalVelocity;
            }
        }
        //if the player has the glider, make sure they only glide when falling
        if(get_has_glider() && state == FALLING) {
            currGravity = gravity / gliderFactor;
        } else if(get_has_glider() && state == JUMPING) {
            currGravity = gravity;
        }
        //if the player presses SHIFT while jumping or falling, make them fast-fall
        if(input->is_action_pressed("ui_crouch") &&
                (state == JUMPING || state == FALLING)) {
            state = FALLING;
            //currGravity will be restored to normal gravity when the player lands
            currGravity = gravity * 500;
        }
        newVelocity.y += delta * currGravity;
    }
    return newVelocity;    
}

/*
 * Takes in the player's velocity and input, and completes all
 * ledge-related computations, then returns a Vector3 representing
 * the player's updated velocity.
 */
Vector3 Player::compute_ledges(Vector3 direction, Input* input) {
    Vector3 newVelocity = direction;
    //if the player is hanging and presses TAB, put them back above the ledge
    if(state == HANGING && input->is_action_just_pressed("ui_focus_next")) {
            set_translation(lastCoordinates);
            currGravity = gravity;
            state = IDLE;
    }
    //if the player is on a surface and comes across a ledge, stop them from
    //walking off of it
    if(!(ray->is_colliding()) && is_on_floor()) {
        newVelocity.x = 0;
        newVelocity.z = 0;
        //if the player presses TAB at the ledge, move them into a ledge-grabbing
        //position
        if (input->is_action_pressed("ui_focus_next") && state != HANGING){
            //store the player's current position as their lastCoordinates
            Vector3 currTranslation = get_translation();
            lastCoordinates = currTranslation * Vector3(1, 1.1, 1);
            //calculate the offset the player should hang from the ledge at
            Vector3 offset = ray->get_cast_to() + Vector3(0, 2.5, 0);
            currTranslation = currTranslation + offset;
            //set the player's gravity to 0, move them to that location, and update
            //their state
            currGravity = 0;
            //need to set velocity's y-component to 0, because it was updated before
            //this method was called
            newVelocity.y = currGravity;
            set_translation(currTranslation);
            state = HANGING;
        }
    }
    return newVelocity;
}

/*
 * Takes in the player's input and behaves accordingly if the player
 * tried to attack. If the player has the sword, the "attacking" signal
 * will be sent out; if not, an error sound effect will be played.
 */
void Player::compute_attack(Input* input) {
    //if the attack cooldown hasn't finished or the player isn't trying
    //to attack, don't compute any attacking logic
    if(attackCD <= 0 && input->is_action_pressed("ui_click") && get_has_sword()) {
        if(is_singleplayer()) {
            attackCD = get_attack_cd();
        } else {
            attackCD = DEFAULT_ATTACK_COOLDOWN;
        }
        if(get_has_sword()) {
            sword->set_rotation_degrees(Vector3(54.5, 38.7, -138.3));
            sword->set_translation(Vector3(0.6, -0.16, -0.44));
            // if(attackRay->is_colliding()){
            //     Object* other = attackRay->get_collider();
            //     Area* tempCast = Object::cast_to<Area>(other);
            //     if(tempCast != nullptr && tempCast->get_name() == "AIEnemy"){
            //         AIEnemy* enemy = Object::cast_to<AIEnemy>(tempCast->get_parent());
            //         if(enemy != nullptr){
            //             Godot::print("Collision is an AIEnemy");
            //             enemy->_on_damage_taken(get_attack(), get_path());
            //         }
            //     }
            // }
            Array hitEnemies = attackArea->get_overlapping_areas();
            for(int i = 0; i < hitEnemies.size(); i++) {
                Area* maybeEnemy = Object::cast_to<Area>(hitEnemies[i]);
                if(maybeEnemy->get_name() == "Enemy") {
                    maybeEnemy->get_parent()->call("_on_damage_taken", get_attack(), get_path());
                } else if(maybeEnemy->get_name() == "AIEnemy") {
                    maybeEnemy->get_parent()->call("_on_damage_taken", get_attack(), get_path());
                }
            }
        } else {
            //if the player doesn't have the sword, play an error sound effect
            get_node("./Error")->call("play", 0.0);
            //if that doesn't work, do this
            // AudioStreamPlayer* errorSound = Object::cast_to<AudioStreamPlayer>(get_node("./Error"));
            // errorSound->play(0.0);
        }
    }
}

/*
 * Takes in the player's input and updates audio busses if the player
 * input any of the audio controls.
 */
void Player::compute_sound_controls(Input* input) {
    AudioServer* server = AudioServer::get_singleton();
	//if the player pushed O, turn the background music's volume down
    if(input->is_action_pressed("ui_volume_down")) {
        if(server->get_bus_volume_db(BUS_BACKGROUND_MUSIC) > MIN_VOLUME) {
            server->set_bus_volume_db(BUS_BACKGROUND_MUSIC, server->get_bus_volume_db(BUS_BACKGROUND_MUSIC) - 1);
        } else {
            server->set_bus_mute(BUS_BACKGROUND_MUSIC, true);
        }
    }
    //if the player pushed P, turn the background music's volume up
    if(input->is_action_pressed("ui_volume_up") && server->get_bus_volume_db(BUS_BACKGROUND_MUSIC) < MAX_VOLUME) {
        if(server->is_bus_mute(BUS_BACKGROUND_MUSIC)) {
            server->set_bus_mute(BUS_BACKGROUND_MUSIC, false);
        } else {
            server->set_bus_volume_db(BUS_BACKGROUND_MUSIC, server->get_bus_volume_db(BUS_BACKGROUND_MUSIC) + 1);
        }
    }
    //if the player pushed I and the sound control cooldown has finished,
    //toggle sound effects on or off
    if(input->is_action_pressed("ui_sfx_toggle") && soundControlCD <= 0) {
        soundControlCD = DEFAULT_SOUND_CONTROL_COOLDOWN;
        if(server->is_bus_mute(BUS_SOUND_EFFECTS)) {
            server->set_bus_mute(BUS_SOUND_EFFECTS, false);
        } else {
            server->set_bus_mute(BUS_SOUND_EFFECTS, true);
        }
    }
}

/*
 * Sets the player's hp according to a passed-in value, emits a signal
 * to make the GUI update the health bar, and ends the game if the
 * player's hp reaches 0.
 */
void Player::change_hp(int delta) {
    if(damageTakenCD <= 0) {
        if(is_singleplayer()) {
            myStatus->change_hp(delta);
        } else {
            if(is_network_master()) {
                mpHp += delta;
            }
        }
        damageTakenCD = DEFAULT_DAMAGE_TAKEN_COOLDOWN;
        if(is_network_master()) {
            emit_signal("health_changed", get_hp(), get_max_hp());
        }
    }
    //this should maybe be in the above if statement
    if(get_hp() <= LOSING_HP) {
        lose();
    }
}

/*
 * Returns the player's current hp.
 */
int Player::get_hp() {
    if(is_singleplayer()) {
        return myStatus->get_hp();
    } else {
        return mpHp;
    }
}

void Player::change_max_hp(int delta) {
    if(is_singleplayer()) {
        myStatus->change_max_hp(delta);
    } else {
        mpMaxHp += delta;
    }
}

int Player::get_max_hp() {
    if(is_singleplayer()) {
        return myStatus->get_max_hp();
    } else {
        return mpMaxHp;
    }
}

/*
 * Sets the player's gold according to a passed-in value, emits a signal
 * to make the GUI update the player's gold count, and ends the game if
 * the player's gold reaches 100;
 */
void Player::change_gold(int delta) {
    if(is_singleplayer()) {
        myStatus->change_gold(delta);
    } else {
        if(is_network_master()) {
            mpGold += delta;
        }
    }
    if(is_network_master()) {
        emit_signal("gold_count_changed", get_gold());
    }
    if(!(is_singleplayer()) && get_gold() >= MULTIPLAYER_WINNING_GOLD) {
        win();
    }
}

int Player::get_gold() {
    if(is_singleplayer()) {
        return myStatus->get_gold();
    } else {
        return mpGold;
    }
}

void Player::change_attack(int delta) {
    if(is_singleplayer()) {
        myStatus->change_attack(delta);
    } else {
        mpAttack += delta;
    }
}

int Player::get_attack() {
    if(is_singleplayer()) {
        return myStatus->get_attack();
    } else {
        return mpAttack;
    }
}

int Player::get_attack_cd() {
    int myAttackSpeed = myStatus->get_attack_speed();
    return DEFAULT_ATTACK_COOLDOWN / myAttackSpeed;
}

void Player::change_attack_range(int delta) {
    myStatus->change_attack_range(delta);
    Vector3 newScale = attackArea->get_scale();
    newScale.x = newScale.x * 1.2;
    newScale.y = newScale.y * 1.5;
    newScale.z = DEFAULT_ATTACK_RANGE * get_attack_range();
    attackArea->set_scale(newScale);
    Vector3 newPos = attackArea->get_translation();
    newPos.z = DEFAULT_ATTACK_RANGE_POSITION_Z - ((((float) get_attack_range()) / 2) * DEFAULT_ATTACK_RANGE_POSITION_Z_MODIFIER);
    attackArea->set_translation(newPos);
}

int Player::get_attack_range() {
    return myStatus->get_attack_range();
}

int Player::get_max_jumps() {
    if(is_singleplayer()) {
        return myStatus->get_max_jumps();
    } else {
        return mpMaxJumps;
    }
}

float Player::get_max_move_speed() {
    if(is_singleplayer()) {
        return (float) myStatus->get_move_speed();
    } else {
        return mpMaxSpeed;
    }
}

bool Player::get_has_sword() {
    if(is_singleplayer()) {
        return myStatus->check_has_sword();
    } else {
        return mpHasSword;
    }
}

bool Player::get_has_glider() {
    if(is_singleplayer()) {
        return myStatus->check_has_glider();
    } else {
        return mpHasGlider;
    }
}

bool Player::get_has_familiar_bell() {
    return myStatus->check_has_familiar_bell();
}

void Player::set_has_familiar() {
    myStatus->collect_familiar();
}

bool Player::get_has_familiar() {
    return myStatus->check_has_familiar();
}

void Player::set_win_status(bool newStatus) {
    didIWin = newStatus;
}

bool Player::get_win_status() {
    return didIWin;
}

void Player::stop_moving() {
    slaveVelocity = Vector3(0, 0, 0);
    slavePosition = Vector3(0, 0, 0);
}

NodePath Player::get_closest_waypoint(){
    return closestWaypoint;
}

Vector3 Player::get_velocity() {
    return velocity;
}

void Player::set_velocity(Vector3 newVelocity) {
    velocity = newVelocity;
}

void Player::pickup_item(int itemType) {

    if(itemType == GLIDER) {
        if(is_singleplayer()) {
            myStatus->pickup_glider();
        } else {
            mpHasGlider = true;
        }
        if(is_network_master()) {
            emit_signal("item_collected", GLIDER);
        }
    } else if(itemType == SWORD) {
        if(is_singleplayer()) {
            myStatus->pickup_sword();
        } else {
            mpHasSword = true;
            change_attack(1);
        }
        if(is_network_master()) {
            emit_signal("item_collected", SWORD);
        }
    } else if(itemType == FAMILIAR_BELL) {
        //doesn't check for multiplayer information, because the familiar
        //bell is only available in singleplayer mode
        myStatus->pickup_familiar_bell();
        emit_signal("item_collected", FAMILIAR_BELL);
    }
}

void Player::minit(String name, Vector3 startPosition, bool isSlave){
    set_translation(startPosition);
}

bool Player::is_singleplayer() {
    Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    return theNetwork->singlePlayerMode;
}

void Player::_on_area_entered(Area* other) {
    if(other->get_name() == "Coin") {
        Coin* coin = Object::cast_to<Coin>(other->get_parent());
        change_gold(1);
        coin->destroy();
        Object::cast_to<AudioStreamPlayer>(get_node("./ChaChing"))->call("play", 0.0);
    }

    else if(other->get_name() == "Glider" && is_network_master()){
        GliderPickup* glider = Object::cast_to<GliderPickup>(other->get_parent());
        pickup_item(GLIDER);
        glider->destroy();
    }

    else if(other->get_name() == "Sword"){
        if(is_network_master()){
            SwordPickup* sword = Object::cast_to<SwordPickup>(other->get_parent());
            pickup_item(SWORD);
            sword->destroy();
        }
        Object::cast_to<Spatial>(get_node("Sword"))->show();
    }

    else if(other->get_name() == "FamiliarPickup") {
        FamiliarPickup* pickup = Object::cast_to<FamiliarPickup>(other->get_parent());
        pickup->destroy();

        pickup_item(FAMILIAR_BELL);

    }
    
    else if(other->get_name() == "Enemy") {
        Enemy* enemy = Object::cast_to<Enemy>(other->get_parent());
        change_hp(-enemy->get_damage_dealt());
    }

    else if(other->get_name() == "AIEnemy") {
        AIEnemy* aiEnemy = Object::cast_to<AIEnemy>(other->get_parent());
        change_hp(-aiEnemy->get_damage_dealt());
    }
}

void Player::play_oof() {
    get_node("./Oof")->call("play", 0.0);
}

void Player::go_to_end_screen() {
    myStatus->reset_status();
    if(is_singleplayer()) {
        get_node("/root/GameManager")->call("_reset");
    }
    if(didIWin) {
        get_tree()->change_scene("res://VictoryScreen.tscn");
    } else {
        get_tree()->change_scene("res://GameOver.tscn");
    }
}

void Player::lose() {
    Input* input = Input::get_singleton();
    input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
    rpc("set_win_status", true);
    rpc("go_to_end_screen");
}

void Player::win() {
    Input* input = Input::get_singleton();
    input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
    set_win_status(true);
    rpc("go_to_end_screen");
}

void Player::find_closest_waypoint(){
    Array navNodes = get_tree()->get_nodes_in_group("Waypoints");
    Vector3 myPos = get_global_transform().origin;

    if(navNodes.size() < 1)
        return;

    //find the closest Waypoint in the navmesh
    Waypoint* targNode = Object::cast_to<Waypoint>(navNodes[0]);
    for(int i = 0; i < navNodes.size(); i++){
        Waypoint* tmpNode = Object::cast_to<Waypoint>(navNodes[i]);
        if(tmpNode->get_dist(myPos) < targNode->get_dist(myPos)){
            targNode = tmpNode;
        }
    }

    closestWaypoint = targNode->get_path();
}

void Player::turn_y(float degrees) {
    rotate_y(degrees);
}