#include "PauseMenu.h"
#include <CanvasItem.hpp>

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void PauseMenu::_register_methods() {
    register_method("_ready", &PauseMenu::_ready);
    register_method("_process", &PauseMenu::_process);
}

PauseMenu::PauseMenu() {}

PauseMenu::~PauseMenu() {}

void PauseMenu::_init() {}

void PauseMenu::_ready() {
    input = Input::get_singleton();
}

void PauseMenu::_process(float delta){
    Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
    if(input->is_action_just_pressed("ui_cancel") && theNetwork->singlePlayerMode){
        if(get_tree()->is_paused()) {
            input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
            get_tree()->set_pause(false);
            Object::cast_to<CanvasItem>(this)->hide();
        } else{
            input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
            get_tree()->set_pause(true);
            //update the player's stats to be accurate
            PlayerStatus* playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
            get_node("/root/Spatial/PauseMenu/Background/StatsContainer/VBoxContainer/MaxHP")->call("set_text", "Max HP: " + String(std::to_string(playerStatus->get_max_hp()).c_str()));
            get_node("/root/Spatial/PauseMenu/Background/StatsContainer/VBoxContainer/CurrHP")->call("set_text", "Current HP: " + String(std::to_string(playerStatus->get_hp()).c_str()));
            get_node("/root/Spatial/PauseMenu/Background/StatsContainer/VBoxContainer/Attack")->call("set_text", "Attack: " + String(std::to_string(playerStatus->get_attack()).c_str()));
            get_node("/root/Spatial/PauseMenu/Background/StatsContainer/VBoxContainer/AttackSpeed")->call("set_text", "Attack Speed: " + String(std::to_string(playerStatus->get_attack_speed()).c_str()));
            get_node("/root/Spatial/PauseMenu/Background/StatsContainer/VBoxContainer/AttackRange")->call("set_text", "Attack Range: " + String(std::to_string(playerStatus->get_attack_range()).c_str()));
            get_node("/root/Spatial/PauseMenu/Background/StatsContainer/VBoxContainer/Jumps")->call("set_text", "Jumps: " + String(std::to_string(playerStatus->get_max_jumps() + 1).c_str()));
            get_node("/root/Spatial/PauseMenu/Background/StatsContainer/VBoxContainer/MovementSpeed")->call("set_text", "Movement Speed: " + String(std::to_string(playerStatus->get_move_speed()).c_str()));
            Object::cast_to<CanvasItem>(this)->show();
        }
    }

    if(input->is_action_pressed("quit") && get_tree()->is_paused()){
        get_tree()->set_pause(false);
        get_tree()->quit();
    }
}