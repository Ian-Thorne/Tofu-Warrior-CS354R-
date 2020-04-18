#include "Credits.h"
#include "Network.h"

using namespace godot;

void Credits::_register_methods() {
    register_method("_ready", &Credits::_ready);
    register_method("switch_main", &Credits::switch_main);
}

Credits::Credits() {}
Credits::~Credits() {}

void Credits::_init() {}

void Credits::_ready() {
    get_node("Button")->connect("pressed", this, "switch_main");
}

void Credits::switch_main() {
    Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    get_tree()->change_scene("res://MainMenu.tscn");
}
