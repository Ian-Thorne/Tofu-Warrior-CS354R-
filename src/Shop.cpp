#include "Shop.h"

using namespace godot;

void Shop::_register_methods() {
    register_method("_init", &Shop::_init);
    register_method("_ready", &Shop::_ready);
}

Shop::Shop() {}
Shop::~Shop() {}

void Shop::_init() {

}

void Shop::_ready() {
    //update the gold counter to be accurate
    PlayerStatus* playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
    Label* label = Object::cast_to<Label>(get_node("/root/Shop/GoldCounter/Background/Number"));
    label->set_text(std::to_string(playerStatus->get_gold()).c_str());

    //update the player's stats to be accurate
    get_node("/root/Shop/StatsContainer/VBoxContainer/MaxHP")->call("set_text", "Max HP: " + String(std::to_string(playerStatus->get_max_hp()).c_str()));
    get_node("/root/Shop/StatsContainer/VBoxContainer/CurrHP")->call("set_text", "Current HP: " + String(std::to_string(playerStatus->get_hp()).c_str()));
    get_node("/root/Shop/StatsContainer/VBoxContainer/Attack")->call("set_text", "Attack: " + String(std::to_string(playerStatus->get_attack()).c_str()));
    get_node("/root/Shop/StatsContainer/VBoxContainer/AttackSpeed")->call("set_text", "Attack Speed: " + String(std::to_string(playerStatus->get_attack_speed()).c_str()));
    get_node("/root/Shop/StatsContainer/VBoxContainer/AttackRange")->call("set_text", "Attack Range: " + String(std::to_string(playerStatus->get_attack_range()).c_str()));
    get_node("/root/Shop/StatsContainer/VBoxContainer/Jumps")->call("set_text", "Jumps: " + String(std::to_string(playerStatus->get_max_jumps() + 1).c_str()));
    get_node("/root/Shop/StatsContainer/VBoxContainer/MovementSpeed")->call("set_text", "Movement Speed: " + String(std::to_string(playerStatus->get_move_speed()).c_str()));

    //connect this Shop's buttons to the GameManager so it can manage scene changes
    get_node("ButtonContainer/StartContainer/StartRound")->connect("pressed", get_node("/root/GameManager"), "switch_to_level");
    get_node("ButtonContainer/UpgradeWeaponContainer/UpgradeWeapon")->connect("pressed", get_node("/root/GameManager"), "switch_to_weapon_shop");
    get_node("ButtonContainer/UpgradeArmorContainer/UpgradeArmor")->connect("pressed", get_node("/root/GameManager"), "switch_to_armor_shop");
    get_node("ButtonContainer/UpgradeBootsContainer/UpgradeBoots")->connect("pressed", get_node("/root/GameManager"), "switch_to_boots_shop");
    get_node("ButtonContainer/MiscShopContainer/MiscShop")->connect("pressed", get_node("/root/GameManager"), "switch_to_misc_shop");
}