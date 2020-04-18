#include "PlayerStatus.h"

using namespace godot;

void PlayerStatus::_register_methods() {
    register_method("_init", &PlayerStatus::_init);
    register_method("_ready", &PlayerStatus::_ready);
    register_method("new_status", &PlayerStatus::new_status);
    register_method("reset_status", &PlayerStatus::reset_status);
    register_method("get_hp", &PlayerStatus::get_hp);
    register_method("change_hp", &PlayerStatus::change_hp);
    register_method("restore_hp", &PlayerStatus::restore_hp);
    register_method("get_max_hp", &PlayerStatus::get_max_hp);
    register_method("change_max_hp", &PlayerStatus::change_max_hp);
    register_method("get_gold", &PlayerStatus::get_gold);
    register_method("change_gold", &PlayerStatus::change_gold);
    register_method("get_attack", &PlayerStatus::get_attack);
    register_method("change_attack", &PlayerStatus::change_attack);
    register_method("get_attack_speed", &PlayerStatus::get_attack_speed);
    register_method("change_attack_speed", &PlayerStatus::change_attack_speed);
    register_method("get_attack_range", &PlayerStatus::get_attack_range);
    register_method("change_attack_range", &PlayerStatus::change_attack_range);
    register_method("get_max_jumps", &PlayerStatus::get_max_jumps);
    register_method("change_max_jumps", &PlayerStatus::change_max_jumps);
    register_method("get_move_speed", &PlayerStatus::get_move_speed);
    register_method("change_move_speed", &PlayerStatus::change_move_speed);
    register_method("get_collected_weapons", &PlayerStatus::get_collected_weapons);
    register_method("get_collected_armor", &PlayerStatus::get_collected_armor);
    register_method("get_collected_boots", &PlayerStatus::get_collected_boots);
    register_method("get_collected_misc", &PlayerStatus::get_collected_misc);
    register_method("add_to_collected_weapons", &PlayerStatus::add_to_collected_weapons);
    register_method("add_to_collected_armor", &PlayerStatus::add_to_collected_armor);
    register_method("add_to_collected_boots", &PlayerStatus::add_to_collected_boots);
    register_method("add_to_collected_misc", &PlayerStatus::add_to_collected_misc);
    register_method("get_collected_armor", &PlayerStatus::get_collected_armor);
    register_method("get_collected_boots", &PlayerStatus::get_collected_boots);
    register_method("get_collected_misc", &PlayerStatus::get_collected_misc);
    register_method("check_has_glider", &PlayerStatus::check_has_glider);
    register_method("pickup_glider", &PlayerStatus::pickup_glider);
    register_method("check_has_sword", &PlayerStatus::check_has_sword);
    register_method("pickup_sword", &PlayerStatus::pickup_sword);
    register_method("check_has_familiar_bell", &PlayerStatus::check_has_familiar_bell);
    register_method("pickup_familiar_bell", &PlayerStatus::pickup_familiar_bell);
    register_method("check_has_familiar", &PlayerStatus::check_has_familiar);
    register_method("collect_familiar", &PlayerStatus::collect_familiar);
    register_method("check_has_final_weapon", &PlayerStatus::check_has_final_weapon);
    register_method("check_which_final_weapon", &PlayerStatus::check_which_final_weapon);
    register_method("collect_final_weapon", &PlayerStatus::collect_final_weapon);
    register_method("check_has_final_armor", &PlayerStatus::check_has_final_armor);
    register_method("check_which_final_armor", &PlayerStatus::check_which_final_armor);
    register_method("collect_final_armor", &PlayerStatus::collect_final_armor);
    register_method("check_has_final_boots", &PlayerStatus::check_has_final_boots);
    register_method("check_which_final_boots", &PlayerStatus::check_which_final_boots);
    register_method("collect_final_boots", &PlayerStatus::collect_final_boots);
}

PlayerStatus::PlayerStatus() {}
PlayerStatus::~PlayerStatus() {}

void PlayerStatus::_init() {
    reset_status();
}

void PlayerStatus::_ready() {}

void PlayerStatus::new_status() {
    hp = DEFAULT_PLAYER_HP;
    maxHp = DEFAULT_PLAYER_HP;
    gold = 0;
    attack = 0;
    attackSpeed = 0;
    attackRange = 0;
    maxJumps = DEFAULT_PLAYER_MAX_EXTRA_JUMPS;
    moveSpeed = DEFAULT_PLAYER_SPEED;
    hasGlider = false;
    hasSword = false;
    hasFamiliarBell = false;
    hasFamiliar = false;
    hasFinalWeapon = false;
    hasFinalArmor = false;
    hasFinalBoots = false;
    collectedWeapons = Array();
    collectedArmor = Array();
    collectedBoots = Array();
    collectedMisc = Array();
}

void PlayerStatus::reset_status() {
    hp = 0;
    maxHp = 0;
    gold = 0;
    attack = 0;
    attackSpeed = 0;
    attackRange = 0;
    maxJumps = 0;
    moveSpeed = 0;
    hasGlider = false;
    hasSword = false;
    hasFamiliarBell = false;
    hasFamiliar = false;
    hasFinalWeapon = false;
    hasFinalArmor = false;
    hasFinalBoots = false;
    collectedWeapons = Array();
    collectedArmor = Array();
    collectedBoots = Array();
    collectedMisc = Array();
}

int PlayerStatus::get_hp() {
    return hp;
}

void PlayerStatus::change_hp(int delta) {
    hp += delta;
}

void PlayerStatus::restore_hp() {
    int toRestore = HEALTH_POTION_HP_RESTORE;
    int diff = get_max_hp() - get_hp();
    if(diff < toRestore) {
        toRestore = diff;
    }
    change_hp(toRestore);
    change_gold(-HEALTH_POTION_COST);
}

int PlayerStatus::get_max_hp() {
    return maxHp;
}

void PlayerStatus::change_max_hp(int delta) {
    maxHp += delta;
}

int PlayerStatus::get_gold() {
    return gold;
}

void PlayerStatus::change_gold(int delta) {
    gold += delta;
}

int PlayerStatus::get_attack() {
    return attack;
}

void PlayerStatus::change_attack(int delta) {
    attack += delta;
}

int PlayerStatus::get_attack_speed() {
    return attackSpeed;
}

void PlayerStatus::change_attack_speed(int delta) {
    attackSpeed += delta;
}

int PlayerStatus::get_attack_range() {
    return attackRange;
}

void PlayerStatus::change_attack_range(int delta) {
    attackRange += delta;
}

int PlayerStatus::get_max_jumps() {
    return maxJumps;
}

void PlayerStatus::change_max_jumps(int delta) {
    maxJumps += delta;
}

int PlayerStatus::get_move_speed() {
    return moveSpeed;
}

void PlayerStatus::change_move_speed(int delta) {
    moveSpeed += delta;
}

Array PlayerStatus::get_collected_weapons() {
    return collectedWeapons;
}

void PlayerStatus::add_to_collected_weapons(String newWeapon) {
    collectedWeapons.append(newWeapon);
}

Array PlayerStatus::get_collected_armor() {
    return collectedArmor;
}

void PlayerStatus::add_to_collected_armor(String newArmor) {
    collectedArmor.append(newArmor);
}

Array PlayerStatus::get_collected_boots() {
    return collectedBoots;
}

void PlayerStatus::add_to_collected_boots(String newBoots) {
    collectedBoots.append(newBoots);
}

Array PlayerStatus::get_collected_misc() {
    return collectedMisc;
}

void PlayerStatus::add_to_collected_misc(String newMisc) {
    collectedMisc.append(newMisc);
    if(newMisc == "Familiar Bell") {
        pickup_familiar_bell();
        collect_familiar();
    }
}

bool PlayerStatus::check_has_glider() {
    return hasGlider;
}

void PlayerStatus::pickup_glider() {
    hasGlider = true;
    add_to_collected_boots("Glider");
}

bool PlayerStatus::check_has_sword() {
    return hasSword;
}

void PlayerStatus::pickup_sword() {
    hasSword = true;
    change_attack(1);
    change_attack_speed(1);
    change_attack_range(1);
    add_to_collected_weapons("Base Sword");
}

bool PlayerStatus::check_has_familiar_bell() {
    return hasFamiliarBell;
}

void PlayerStatus::pickup_familiar_bell() {
    hasFamiliarBell = true;
}

bool PlayerStatus::check_has_familiar() {
    return hasFamiliar;
}

void PlayerStatus::collect_familiar() {
    hasFamiliar = true;
}

bool PlayerStatus::check_has_final_weapon() {
    return hasFinalWeapon;
}

String PlayerStatus::check_which_final_weapon() {
    return finalWeapon;
}

void PlayerStatus::collect_final_weapon(String newFinalWeapon) {
    hasFinalWeapon = true;
    finalWeapon = newFinalWeapon;
}

bool PlayerStatus::check_has_final_armor() {
    return hasFinalArmor;
}

String PlayerStatus::check_which_final_armor() {
    return finalArmor;
}

void PlayerStatus::collect_final_armor(String newFinalArmor) {
    hasFinalArmor = true;
    finalArmor = newFinalArmor;
}

bool PlayerStatus::check_has_final_boots() {
    return hasFinalBoots;
}
String PlayerStatus::check_which_final_boots() {
    return finalBoots;
}

void PlayerStatus::collect_final_boots(String newFinalBoots) {
    hasFinalBoots = true;
    finalBoots = newFinalBoots;
}