#include "Coin.h"
#include <SceneTree.hpp>
#include <Viewport.hpp>

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void Coin::_register_methods() {
    register_method("_ready", &Coin::_ready);
    register_method("_process", &Coin::_process);
    register_method("destroy", &Coin::destroy, GODOT_METHOD_RPC_MODE_REMOTESYNC);
    register_method("get_position", &Coin::get_position, GODOT_METHOD_RPC_MODE_REMOTESYNC);
}

Coin::Coin() {}
Coin::~Coin() {}

void Coin::_init() {}

void Coin::_ready() {
    //set up the signal to send when the coin is touched
    add_to_group("Coins");
}

void Coin::_process() {
    rotate_y(0.05);
}

void Coin::destroy(){
	get_node("/root/Network")->rpc("append_deleteList", get_path());
    queue_free();
}

Vector3 Coin::get_position(){
    return get_global_transform().origin;
}