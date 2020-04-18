#include "Enemy.h"

using namespace godot;

void Enemy::_register_methods() {
    register_method("_ready", &Enemy::_ready);
    register_method("_process", &Enemy::_process);
    register_method("_on_damage_taken", &Enemy::_on_damage_taken);
    register_method("get_damage_dealt", &Enemy::get_damage_dealt);
    register_method("connect_to_player", &Enemy::connect_to_player);
    register_method("set_hp", &Enemy::set_hp, GODOT_METHOD_RPC_MODE_REMOTESYNC);
    register_method("get_hp", &Enemy::get_hp);
    register_property<Enemy, int>("hp", &Enemy::hp, DEFAULT_ENEMY_HP);
    register_property<Enemy, int>("damageDealt", &Enemy::damageDealt, DEFAULT_ENEMY_DAMAGE_DEALT);
    register_property<Enemy, int>("goldValue", &Enemy::goldValue, DEFAULT_ENEMY_GOLD_VALUE);
}

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::_init() {
    //define the Enemy's properties
    hp = DEFAULT_ENEMY_HP;
    damageDealt = DEFAULT_ENEMY_DAMAGE_DEALT;
    goldValue = DEFAULT_ENEMY_GOLD_VALUE;
}

void Enemy::_ready() {
    //get_node("../Player")->connect("attacking", this, "_on_damage_taken");
}

void Enemy::_process(float delta) {

}

void Enemy::_on_damage_taken(int damage, NodePath damageSource) {
    //find the player and calculate the Enemy's distance from them
    Player* player = Object::cast_to<Player>(get_node(damageSource));
    Vector3 playerPos = player->get_global_transform().origin;
    Vector3 myPos = get_global_transform().origin;
    float distanceToPlayer = myPos.distance_to(playerPos);
    //if the player is close enough, update the Enemy's hp based
    //on the damage taken
    if(distanceToPlayer < get_scale().x + 2) {
        //play the sound effect
        player->play_oof();
        //update the Enemy's health accordingly
        int newHp = get_hp() - damage;
        if(newHp <= 0) {
            //if this hit will kill the enemy, give the player gold
            player->change_gold(goldValue);
        }
        //make sure the enemies in all players' games update their hp and/or die
        rpc("set_hp", newHp);
    }
}

void Enemy::set_hp(int newHp) {
    hp = newHp;
    if(newHp <= 0) {
        // do the dying logic
        get_node("/root/Network")->rpc("append_deleteList", get_path());
        set_translation(Vector3(0, -50, 0));
        hide();
    }
}

int Enemy::get_hp() {
    return hp;
}

int Enemy::get_damage_dealt() {
    return damageDealt;
}

void Enemy::connect_to_player(NodePath playerPath) {
    Godot::print("CONNECTING");
    get_node(playerPath)->connect("attacking", this, "_on_damage_taken");
}