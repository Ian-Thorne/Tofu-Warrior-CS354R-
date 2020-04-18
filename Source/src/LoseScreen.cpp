#include "LoseScreen.h"

using namespace godot;

void LoseScreen::_register_methods() {
    register_method("_ready", &LoseScreen::_ready);
    register_method("switch_screen", &LoseScreen::switch_screen);
}

LoseScreen::LoseScreen() {}
LoseScreen::~LoseScreen() {}

void LoseScreen::_init() {}

void LoseScreen::_ready() {
    get_node("./Button")->connect("pressed", this, "switch_screen");
    theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    if(theNetwork->singlePlayerMode) {
        get_node("./Button")->call("set_text", "Main Menu");
    } else {
        get_node("./Button")->call("set_text", "Lobby");
    }
}

void LoseScreen::switch_screen() {
    get_node("/root/Network")->call("leave_server");
    if(theNetwork->singlePlayerMode) {
        get_tree()->change_scene("res://MainMenu.tscn");
    } else {
        get_tree()->change_scene("res://Lobby.tscn");
    }
}