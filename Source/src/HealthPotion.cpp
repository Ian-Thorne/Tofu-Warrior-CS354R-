#include "HealthPotion.h"

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void HealthPotion::_register_methods() {
    register_method("_init", &HealthPotion::_init);
    register_method("_ready", &HealthPotion::_ready);
    register_method("_process", &HealthPotion::_process);
    register_method("restore_hp", &HealthPotion::restore_hp);
}

HealthPotion::HealthPotion() {}
HealthPotion::~HealthPotion() {}

void HealthPotion::_init() {

}

void HealthPotion::_ready() {
    imgBox = Object::cast_to<ColorRect>(get_node("ItemBG"));
    descBox = Object::cast_to<ColorRect>(get_node("DescBG"));

    //connect this HealthPotion's texture button to increase the player's HP by 5
    get_node("ItemBG/ItemButton")->connect("pressed", this, "restore_hp");
}

void HealthPotion::_process() {
    Input* input = Input::get_singleton();
    //Godot::print("HELLO");
    //if mouse.x > min_x and mouse.y > min_y and mouse.x < max_x and mouse.y < max_y:
    Vector2 mousePos = descBox->get_global_mouse_position();
    float mX = mousePos.x;
    float mY = mousePos.y;
    Vector2 imgPos = imgBox->get_global_rect().position;
    Vector2 imgPosSize = imgBox->get_global_rect().size;
    float iXT = imgPos.x;
    float iYT = imgPos.y;
    float iXB = iXT + imgPosSize.x;
    float iYB = iYT + imgPosSize.y;
    // Godot::print(std::to_string(mX).c_str());
    // Godot::print(std::to_string(iXB).c_str());
    if(mX > iXT && mX < iXB && mY > iYT && mY < iYB){
        descBox->show();
    }
    else{
        descBox->hide();
    }
}

void HealthPotion::restore_hp() {
    PlayerStatus* playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
    if(playerStatus->get_gold() >= HEALTH_POTION_COST) {
    	playerStatus->restore_hp();
	    Label* goldCountText = Object::cast_to<Label>(get_node("/root/Shop/GoldCounter/Background/Number"));
		goldCountText->set_text(std::to_string(playerStatus->get_gold()).c_str());
    }
}
