#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <Ref.hpp>
#include <PackedScene.hpp>

namespace godot {

class Game : public Node {
    GODOT_CLASS(Game, Node)

private:
    Ref<PackedScene> PlayerScene;
    Ref<PackedScene> EnemyScene;
    Array numEnemies;
    Array spawnLocations;
    int currRoundNumber;
    int timer;

public:
    static void _register_methods();

    Game();
    ~Game();

    void _init();
    void _ready();
    void _process();
    void _on_player_disconnected(int64_t id);
    void _on_server_disconnected(int64_t id);
    void spawn_enemies(int roundNumber);
    void try_end_round();
    void parse_file(std::string file, int roundNumber);

};

}