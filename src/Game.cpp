#include "Game.h"
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include "Player.h"
#include <Dictionary.hpp>
#include <string>

using namespace godot;

void Game::_register_methods() {
    register_method("_init", &Game::_init, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_ready", &Game::_ready, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_player_disconnected", &Game::_on_player_disconnected, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_server_disconnected", &Game::_on_server_disconnected, GODOT_METHOD_RPC_MODE_DISABLED);
}

Game::Game() {}

Game::~Game() {}

void Game::_init() {
    ResourceLoader* resourceLoader = ResourceLoader::get_singleton();
    PlayerScene = resourceLoader->load("res://Player.tscn");
}

void Game::_ready() {
    get_tree()->connect("network_peer_disconnected", this, "_on_player_disconnected");
    get_tree()->connect("server_disconnected", this, "_on_server_disconnected");

    godot::Player* player = static_cast<godot::Player*>(PlayerScene->instance());

    player->set_name(std::to_string(get_tree()->get_network_unique_id()).c_str());
    player->set("nodeName", get_tree()->get_network_unique_id());
    player->set_network_master(get_tree()->get_network_unique_id());
    add_child(player);
    player->translate(Vector3(0, 10, 0));

    Dictionary selfData = Dictionary(get_node("/root/Network")->get("selfData"));
}

void Game::_on_player_disconnected(int64_t id) {
    if(get_node(std::to_string(id).c_str())) {
        Player* disconnectee = Object::cast_to<Player>(get_node(std::to_string(id).c_str()));
        disconnectee->remove_from_group("Players");
        disconnectee->stop_moving();
        disconnectee->hide();
        disconnectee->set_pause_mode(true);
    }
}

void Game::_on_server_disconnected(int64_t id) {
    get_tree()->change_scene("res://Lobby.tscn");
}