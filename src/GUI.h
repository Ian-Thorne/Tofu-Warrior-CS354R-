#ifndef GUI_H
#define GUI_H

#include "constants.h"
#include "Player.h"
#include <Godot.hpp>
#include <Object.hpp>
#include <Control.hpp>
#include <Timer.hpp>
#include <Label.hpp>
#include <TextureProgress.hpp>
#include <string>

namespace godot {

class GUI : public Control {
    GODOT_CLASS(GUI, Control);

private:
    TextureProgress* myHPBar;
    Label* myGoldCounter;
    Control* mySwordIcon;
    Control* myGliderIcon;

public:
    static void _register_methods();

    GUI();
    ~GUI();

    void _init();
    void _ready();
    // void _process(float delta);  //might not need _process()
    
    //this will be signalled when the player's hp changes and it
    //will be passed the change in hp
    void _on_hp_changed(int newHp);
    //this will be signalled when the player's gold count changes
    //and it will be passed the change in gold
    void _on_gold_count_changed(int newGoldCount);
    //this will be signalled when the player picks up an item
    void _on_item_collected(int itemType);
};

}

#endif