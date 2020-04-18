#include "GameManager.h"

using namespace godot;

void GameManager::_register_methods() {
    register_method("_init", &GameManager::_init);
    register_method("_ready", &GameManager::_ready);
    register_method("_process", &GameManager::_process);
    register_method("_reset", &GameManager::_reset);
    register_method("start_game", &GameManager::start_game);
    register_method("switch_to_level", &GameManager::switch_to_level);
    register_method("switch_to_shop", &GameManager::switch_to_shop);
    register_method("switch_to_weapon_shop", &GameManager::switch_to_weapon_shop);
    register_method("switch_to_armor_shop", &GameManager::switch_to_armor_shop);
    register_method("switch_to_boots_shop", &GameManager::switch_to_boots_shop);
    register_method("switch_to_misc_shop", &GameManager::switch_to_misc_shop);
    register_method("switch_scene", &GameManager::switch_scene);
    register_method("try_end_round", &GameManager::try_end_round);
    register_method("spawn_entities", &GameManager::spawn_entities);
    register_method("spawn_player", &GameManager::spawn_player);
    register_method("spawn_familiar", &GameManager::spawn_familiar);
    register_method("spawn_enemies", &GameManager::spawn_enemies);
    register_method("spawn_pickups", &GameManager::spawn_pickups);
    register_method("update_gui", &GameManager::update_gui);
    register_method("get_shop_timer", &GameManager::get_shop_timer);
    register_method("set_shop_timer", &GameManager::set_shop_timer);
    register_method("set_round_num", &GameManager::set_round_num);
}

GameManager::GameManager() {}
GameManager::~GameManager() {}

void GameManager::_init() {
    _reset();
}

void GameManager::_ready() {
    playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
}

void GameManager::_process() {
    if(timer > 0) {
        timer--;
        if(timer == 0) {
            if(currRound > numRounds) {
                Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
                player->win();
            } else {
                switch_to_shop();
            }
        }
    }
}

void GameManager::_reset() {
    //if there was a file stream open from a previous play-through, close it
    roundInfo.close();
    resourceLoader = ResourceLoader::get_singleton();
    playerScene = resourceLoader->load("res://Player.tscn");
    familiarScene = resourceLoader->load("res://Familiar.tscn");
    enemyScene = resourceLoader->load("res://AIEnemy.tscn");
    currRound = 1;
    numRounds = 0;
    timer = 0;
    inGame = false;
    roundInfo.open("roundInfo.txt");
    //determine the number of rounds
    std::string line;
    while(std::getline(roundInfo, line)) {
        numRounds++;
    }
    roundInfo.close();
    roundInfo.open("roundInfo.txt");

    Godot::print(std::to_string(numRounds).c_str());
}

//this method exists in case other actions need to be done to start the game
void GameManager::start_game() {
    PlayerStatus* playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
    playerStatus->new_status();
    switch_to_level();
    inGame = true;
}

void GameManager::switch_to_level() {
    //make sure the round info file exists, if it does, get the next round's info
    if(!roundInfo.is_open()) {
        Godot::print("ROUND DATA FILE NOT FOUND!");
    }
    get_next_line();

    //switch to the desired level
    switch_scene(read_string());

    //set the round number on the GUI
    call_deferred("set_round_num", currRound);

    //spawn the entities into the level
    call_deferred("spawn_entities");
    inGame = true;
}

void GameManager::switch_to_shop() {
    if(!inGame) {
        int secondsRemaining = get_shop_timer();
        switch_scene("res://GUI/Shop.tscn");
        call_deferred("set_shop_timer", secondsRemaining);
    } else {
        Input* input = Input::get_singleton();
        input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
        switch_scene("res://GUI/Shop.tscn");
        inGame = false;
    }
}

void GameManager::switch_to_weapon_shop() {
    int secondsRemaining = get_shop_timer();
    switch_scene("res://GUI/WeaponShop.tscn");
    call_deferred("set_shop_timer", secondsRemaining);
}

void GameManager::switch_to_armor_shop() {
    int secondsRemaining = get_shop_timer();
    switch_scene("res://GUI/ArmorShop.tscn");
    call_deferred("set_shop_timer", secondsRemaining);
}

void GameManager::switch_to_boots_shop() {
    int secondsRemaining = get_shop_timer();
    switch_scene("res://GUI/BootShop.tscn");
    call_deferred("set_shop_timer", secondsRemaining);
}

void GameManager::switch_to_misc_shop() {
    int secondsRemaining = get_shop_timer();
    switch_scene("res://GUI/MiscShop.tscn");
    call_deferred("set_shop_timer", secondsRemaining);
}

void GameManager::switch_scene(String path) {
    Node* oldScene = get_tree()->get_current_scene();
    get_tree()->change_scene(path);
    oldScene->queue_free();
}

void GameManager::try_end_round() {
    Array enemies = get_tree()->get_nodes_in_group("Enemies");
    Godot::print(std::to_string(enemies.size()).c_str());
    if(enemies.size() == 0) {
        currRound++;
        timer = 60;
    }
}

void GameManager::spawn_entities() {
    //new order should be:
        //Familiar
        //Player
        //Pickups
        //Enemies

    Vector3 playerSpawnPoint = read_vector();

    spawn_player(playerSpawnPoint);

    if(playerStatus->check_has_familiar()) {
        Vector3 familiarSpawnPoint = playerSpawnPoint;
        familiarSpawnPoint = playerSpawnPoint;
        familiarSpawnPoint.z -= 2;
        spawn_familiar(familiarSpawnPoint);
    }

    spawn_pickups();

    //since this is happening after spawning the player, they're not getting
    //connected, so they're not interacting with the player properly!
    //however, putting this before spawning the player will also cause problems,
    //because the line will be read out of order, so you'll need to figure that out
    spawn_enemies();

    // spawn_player(playerSpawnPoint);

    // call_deferred("spawn_pickups");
}

void GameManager::spawn_player(Vector3 spawnLocation) {
    Spatial* level = Object::cast_to<Spatial>(get_node("/root/Spatial"));
    Player* player = Object::cast_to<Player>(playerScene->instance());
    player->set_name(std::to_string(get_tree()->get_network_unique_id()).c_str());
    player->set("nodeName", get_tree()->get_network_unique_id());
    level->add_child(player);
    player->translate(spawnLocation);
    call_deferred("update_gui");
}

void GameManager::spawn_familiar(Vector3 spawnLocation) {
    Spatial* level = Object::cast_to<Spatial>(get_node("/root/Spatial"));
    Familiar* familiar = Object::cast_to<Familiar>(familiarScene->instance());
    if(playerStatus->check_has_familiar()) {
        familiar->set_state(F_FREE_PURSUE);
    }
    level->add_child(familiar);
    familiar->set_translation(spawnLocation);
}

void GameManager::spawn_enemies() {
    // int numToSpawn = numEnemies[roundNum -1];
    // Spatial* enemies = Object::cast_to<Spatial>(get_node("/root/Spatial/Enemies"));
    // for(int i = 0; i < numToSpawn; i++) {
    //     AIEnemy* enemy = Object::cast_to<AIEnemy>(enemyScene->instance());
    //     enemies->add_child(enemy);
    //     enemy->add_to_group("Enemies");
    //     enemy->translate(spawnLocations[i]);
    // }


    int numEnemies = read_int();
    int hp = read_int();
    int attack = read_int();
    int goldValue = read_int();
    Spatial* enemies = Object::cast_to<Spatial>(get_node("/root/Spatial/Enemies"));
    for(int i = 0; i < numEnemies; i++) {
        Vector3 spawnPoint = read_vector();
        AIEnemy* enemy = Object::cast_to<AIEnemy>(enemyScene->instance());
        enemy->set_hp(hp);
        enemy->set_damage_dealt(attack);
        enemy->set_gold_value(goldValue);
        enemies->add_child(enemy);
        enemy->add_to_group("Enemies");
        enemy->translate(spawnPoint);
    }
}

void GameManager::spawn_pickups() {
    Spatial* level = Object::cast_to<Spatial>(get_node("/root/Spatial"));
    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    Vector3 swordSpawnPoint = read_vector();
    Vector3 gliderSpawnPoint = read_vector();
    if(!player->get_has_glider()) {
        Ref<PackedScene> gliderPickupScene = resourceLoader->load("res://GliderPickup.tscn");
        GliderPickup* gliderPickup = Object::cast_to<GliderPickup>(gliderPickupScene->instance());
        level->add_child(gliderPickup);
        // gliderPickup->set_translation(Vector3(-26, 18, -26));
        gliderPickup->set_translation(gliderSpawnPoint);
        gliderPickup->set_scale(Vector3(0.5, 0.5, 0.5));
    }
    if(!player->get_has_sword()) {
        Ref<PackedScene> swordPickupScene = resourceLoader->load("res://SwordPickup.tscn");
        SwordPickup* swordPickup = Object::cast_to<SwordPickup>(swordPickupScene->instance());
        level->add_child(swordPickup);
        // swordPickup->set_translation(Vector3(-26, 10, 0));
        swordPickup->set_translation(swordSpawnPoint);
        swordPickup->set_scale(Vector3(2, 2, 2));
    }
}

void GameManager::update_gui() {
    Player* player = Object::cast_to<Player>(get_node("/root/Spatial/1"));
    player->emit_signal("health_changed", player->get_hp(), player->get_max_hp());
    player->emit_signal("gold_count_changed", player->get_gold());
    if(player->get_has_glider()) {
        player->emit_signal("item_collected", GLIDER);
    }
    if(player->get_has_sword()) {
        player->emit_signal("item_collected", SWORD);
    }
    if(player->get_has_familiar_bell()) {
        player->emit_signal("item_collected", FAMILIAR_BELL);
    }
}

void GameManager::set_round_num(int roundNum) {
    get_node("/root/Spatial/Control")->call("set_round_num", roundNum);
}

int GameManager::get_shop_timer() {
    return get_node("/root/Shop/Timer")->call("get_seconds_remaining");
}

void GameManager::set_shop_timer(int secondsRemaining) {
    get_node("/root/Shop/Timer")->call("set_seconds_remaining", secondsRemaining);
}

//YOU CAN'T PUT A STANDARD LIBRARY STRING IN A GODOT ARRAY
void GameManager::parse_file(std::string file, int roundNum){
    // File pointer 
    std::ifstream fin; 
    Godot::print(String(file.c_str()));
    // Open an existing file 
    fin.open("test.txt"); 
    if(!fin.is_open()) {
        Godot::print("Failure!");
    }
    // Get the roll number 
    // of which the data is required 
    int currRoundNumber = 0;
    int count = 0; 
  
    // Read the Data from the file 
    // as String Vector 
    std::vector<std::string> row; 
    std::string line, word, temp; 
  
    while (std::getline(fin, line)) { 
        Godot::print("searching");
        row.clear(); 
  
        // read an entire row and 
        // store it in a string variable 'line' 
        Godot::print(String(line.c_str()));
        // used for breaking words 
        std::stringstream s(line); 
  
        // read every column data of a row and 
        // store it in a string variable, 'word' 
        while (std::getline(s, word, ',')) {   
            Godot::print("looping through row");
            // add all the column data 
            // of a row to a vector 
            row.push_back(word); 
            Godot::print(String(word.c_str()));
        } 
        if(row.empty()) {
            Godot::print("Is empty");
        }
  
        //convert string to integer for comparision 
        currRoundNumber = std::stoi(row[0]); 
        Godot::print(String(row[0].c_str()));
  
        // Compare the roll number 
        if (roundNum == currRoundNumber) { 
  
            // Print the found data 
            count = 1; 
            Godot::print("Found the right row:");
            Godot::print(String(std::to_string(currRoundNumber).c_str()));
            break; 
        } 
    } 
    if (count == 0) {
        Godot::print("Row not found");
    }
}

void GameManager::get_next_line() {
    //get the next line in the roundInfo file
    std::string line;
    std::getline(roundInfo, line);
    //make that line into a stream
    currRoundInfo = std::stringstream(line);
}

/*
 * Strings are organized like ...,string,.. in the round info file.
 */
String GameManager::read_string() {
    //get the next string in the roundInfo file
    std::string word;
    std::getline(currRoundInfo, word, ',');
    //return it as a Godot string
    return String(word.c_str());
}

/*
 * Integers are organized like ...,integer,... in the round info file.
 */
int GameManager::read_int() {
    //get the next string in the roundInfo file
    std::string word;
    std::getline(currRoundInfo, word, ',');
    //return it as an int
    return std::stoi(word);
}

/*
 * Vectors are organized in ...,x|y|z,... order in the round info file.
 */
Vector3 GameManager::read_vector() {
    float x;
    float y;
    float z;
    std::string temp;
    std::getline(currRoundInfo, temp, '|');
    x = std::stof(temp);
    std::getline(currRoundInfo, temp, '|');
    y = std::stof(temp);
    std::getline(currRoundInfo, temp, ',');
    z = std::stof(temp);
    return Vector3(x, y, z);
}