/*
 * PlayerStatus is a class created to keep track of the Player's State,
 * outside of the Player object itself. This will allow a Player's state
 * to persist between scene changes, which will be necessary for implementing
 * a round system that the game will use.
 * 
 * This class will be responsible for maintaining player information like HP,
 * gold, attack, collected items, and stuff like that. The Player object itself
 * will no longer contain that information and will instead get it from this
 * "singleton" when it needs it.
 * 
 * This "singleton" will be auto-loaded at the start of the game. When it is,
 * all of its member variables will be set to zero or their zero-equivalent.
 * When starting a single player game, creating a multiplayer game, or joining
 * a multiplayer game, this "singleton" will have its member variables initialized
 * to their default values. When that game is left for any reason (winning or losing),
 * this "singleton" will have its member variables reset to zero or their
 * zero-equivalent. This will ensure that state persists between rounds (if the game
 * mode has them), but not between "play-throughs" of the game.
 */

#ifndef _PLAYER_STATUS_
#define _PLAYER_STATUS_

#include "constants.h"
#include <Godot.hpp>
#include <Node.hpp>
#include <Array.hpp>

namespace godot {

class PlayerStatus : public Node {
    GODOT_CLASS(PlayerStatus, Node)

private:
    /* stats */
    int hp;
    int maxHp;
    int gold;
    int attack;
    int attackSpeed;
    int attackRange;
    int maxJumps;
    int moveSpeed;
    /* inventory */
    bool hasGlider;
    bool hasSword;
    bool hasFamiliarBell;
    bool hasFamiliar;
    bool hasFinalWeapon;
    String finalWeapon;
    bool hasFinalArmor;
    String finalArmor;
    bool hasFinalBoots;
    String finalBoots;
    Array collectedWeapons;
    Array collectedArmor;
    Array collectedBoots;
    Array collectedMisc;

public:
    static void _register_methods();

    PlayerStatus();
    ~PlayerStatus();

    void _init();
    void _ready();

    //this is used to start using the PlayerStatus
    void new_status();
    //this is used to reset the PlayerStatus, so the game
    //can be played again without restarting it
    void reset_status();

    /* stat modifiers and getters */
    int get_hp();
    void change_hp(int delta);
    void restore_hp();
    int get_max_hp();
    void change_max_hp(int delta);
    int get_gold();
    void change_gold(int delta);
    int get_attack();
    void change_attack(int delta);
    int get_attack_speed();
    void change_attack_speed(int delta);
    int get_attack_range();
    void change_attack_range(int delta);
    int get_max_jumps();
    void change_max_jumps(int delta);
    int get_move_speed();
    void change_move_speed(int delta);

    /* inventory list functions */
    Array get_collected_weapons();
    void add_to_collected_weapons(String newWeapon);
    Array get_collected_armor();
    void add_to_collected_armor(String newArmor);
    Array get_collected_boots();
    void add_to_collected_boots(String newBoots);
    Array get_collected_misc();
    void add_to_collected_misc(String newMisc);

    /* item flag getters and setters */
    bool check_has_glider();
    void pickup_glider();
    bool check_has_sword();
    void pickup_sword();
    bool check_has_familiar_bell();
    void pickup_familiar_bell();
    bool check_has_familiar();
    void collect_familiar();
    bool check_has_final_weapon();
    String check_which_final_weapon();
    void collect_final_weapon(String newFinalWeapon);
    bool check_has_final_armor();
    String check_which_final_armor();
    void collect_final_armor(String newFinalArmor);
    bool check_has_final_boots();
    String check_which_final_boots();
    void collect_final_boots(String newFinalBoots);
};

}

#endif //_PLAYER_STATUS_