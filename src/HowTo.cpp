#include "HowTo.h"
#include "Network.h"

using namespace godot;

void HowTo::_register_methods() {
    register_method("_ready", &HowTo::_ready);
    register_method("switch_main", &HowTo::switch_main);
}

HowTo::HowTo() {}
HowTo::~HowTo() {}

void HowTo::_init() {}

void HowTo::_ready() {
    get_node("Button")->connect("pressed", this, "switch_main");
}

void HowTo::switch_main() {
    Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    get_tree()->change_scene("res://MainMenu.tscn");
}
