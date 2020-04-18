#include "GUI.h"

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void GUI::_register_methods() {
    register_method("_ready", &GUI::_ready);
    register_method("_on_hp_changed", &GUI::_on_hp_changed);
    register_method("_on_gold_count_changed", &GUI::_on_gold_count_changed);
    register_method("_on_item_collected", &GUI::_on_item_collected);
}

GUI::GUI() {}

GUI::~GUI() {}

void GUI::_init() {}

void GUI::_ready() {
    //find all relevant nodes and sub-nodes
    // myHPBar = Object::cast_to<TextureProgress>(get_node("GUI/HBoxContainer/Rows/HBoxContainer/LifeBar/Gauge"));
    myGoldCounter = Object::cast_to<Label>(get_node("GUI/HBoxContainer/Rows/Items/GoldCounter/Background/Number"));
    mySwordIcon = Object::cast_to<Control>(get_node("GUI/HBoxContainer/Rows/Items/SwordItem"));
    myGliderIcon = Object::cast_to<Control>(get_node("GUI/HBoxContainer/Rows/Items/GliderItem"));

    //connect the player's HP and gold to the GUI
    Player* player = Object::cast_to<Player>(get_node("../Player"));
    player->connect("health_changed", this, "_on_hp_changed");
    player->connect("gold_count_changed", this, "_on_gold_count_changed");
    myGoldCounter->set_text(String(std::to_string(player->get_gold()).c_str()));
    player->connect("item_collected", this, "_on_item_collected");
}

void GUI::_on_hp_changed(int newHp) {
    //update the hp bar
    
}

void GUI::_on_gold_count_changed(int newGoldCount) {
    myGoldCounter->set_text(String(std::to_string(newGoldCount).c_str()));
}

void GUI::_on_item_collected(int itemType) {
    if(itemType == GLIDER) {
        myGliderIcon->show();
    } else if(itemType == SWORD) {
        mySwordIcon->show();
    }
}