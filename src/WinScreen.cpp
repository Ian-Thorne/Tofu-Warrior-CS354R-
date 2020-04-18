#include "WinScreen.h"

using namespace godot;

void WinScreen::_register_methods() {
    register_method("_ready", &WinScreen::_ready);
    register_method("switch_screen", &WinScreen::switch_screen);
}

WinScreen::WinScreen() {}
WinScreen::~WinScreen() {}

void WinScreen::_init() {}

void WinScreen::_ready() {
    get_node("./Button")->connect("pressed", this, "switch_screen");
    theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    if(theNetwork->singlePlayerMode) {
        get_node("./Button")->call("set_text", "Main Menu");
    } else {
        get_node("./Button")->call("set_text", "Lobby");
    }
}

void WinScreen::switch_screen() {
    get_node("/root/Network")->call("leave_server");
    if(theNetwork->singlePlayerMode) {
        get_tree()->change_scene("res://MainMenu.tscn");
    } else {
        get_tree()->change_scene("res://Lobby.tscn");
    }
}