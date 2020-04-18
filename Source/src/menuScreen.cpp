#include "menuScreen.h"
#include "Network.h"
#include "PlayerStatus.h"

using namespace godot;

void menuScreen::_register_methods() {
    register_method("_ready", &menuScreen::_ready);
    register_method("switch_single", &menuScreen::switch_single);
    register_method("switch_multi", &menuScreen::switch_multi);
    register_method("switch_credits", &menuScreen::switch_credits);
}

menuScreen::menuScreen() {}
menuScreen::~menuScreen() {}

void menuScreen::_init() {}

void menuScreen::_ready() {
    get_node("./VBoxContainer/SinglePlayerButton")->connect("pressed", this, "switch_single");
    get_node("./VBoxContainer/MultiplayerButton")->connect("pressed", this, "switch_multi");
    get_node("./VBoxContainer/CreditButton")->connect("pressed", this, "switch_credits");
    //get_node("./VBoxContainer/HowToPlayButton")->connect("pressed", this, "switch_howto");
    Array tmp = IP::get_singleton()->get_local_addresses();
    INT_IP = tmp[0];
    EXT_IP = tmp[1];
}

void menuScreen::switch_single() {
    Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    theNetwork->singlePlayerMode = true;
    theNetwork->call("create_server", String("PLAYER"));
    get_node("/root/GameManager")->call("start_game");
}

void menuScreen::switch_multi() {
    Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    theNetwork->singlePlayerMode = false;
    get_tree()->change_scene("res://Lobby.tscn");
}

void menuScreen::switch_credits() {
    Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    get_tree()->change_scene("res://GUI/Credits.tscn");
}

void menuScreen::switch_howto() {
    Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    get_tree()->change_scene("res://HowToPlayScreen.tscn");
}



