#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "Player.h"
#include "PlayerStatus.h"
#include "AIEnemy.h"
#include "GliderPickup.h"
#include "SwordPickup.h"
#include "FamiliarPickup.h"
#include <Godot.hpp>
#include <Node.hpp>
#include <Spatial.hpp>
#include <Input.hpp>
#include <Ref.hpp>
#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <ios>

namespace godot {

class GameManager : public Node {
    GODOT_CLASS(GameManager, Node)

private:
    PlayerStatus* playerStatus;
    ResourceLoader* resourceLoader;
    Ref<PackedScene> playerScene;
    Ref<PackedScene> familiarScene;
    Ref<PackedScene> enemyScene;
    int currRound;
    int numRounds;
    int timer;
    bool inGame;

    //file scanning variables
    std::ifstream roundInfo;
    std::stringstream currRoundInfo;

public:
    static void _register_methods();

    GameManager();
    ~GameManager();

    void _init();
    void _ready();
    void _process();
    void _reset();
    
    //game management functions
    void start_game();
    void switch_to_level();
    void switch_to_shop();
    void switch_to_weapon_shop();
    void switch_to_armor_shop();
    void switch_to_boots_shop();
    void switch_to_misc_shop();
    void switch_scene(String path);
    void try_end_round();
    void spawn_entities();
    void spawn_player(Vector3 spawnLocation);
    void spawn_familiar(Vector3 spawnLocation);
    void spawn_enemies();
    void spawn_pickups();
    void update_gui();
    void set_round_num(int roundNum);
    int get_shop_timer();
    void set_shop_timer(int secondsRemaining);

    //file parsing functions
    void parse_file(std::string file, int roundNum);
    /* reads the next line in from the round info file */
    void get_next_line();
    /* creates a String out of a string in the round info file */
    String read_string();
    /* creates an int out of an integer in the round info file */
    int read_int();
    /* creates a vector out of a triplet of numbers in the round
    info file organized like x|y|z */
    Vector3 read_vector();
};

}

#endif //_GAME_MANAGER_H_