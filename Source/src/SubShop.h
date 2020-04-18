#ifndef _SUB_SHOP_H_
#define _SUB_SHOP_H_

#include <Godot.hpp>
#include <Control.hpp>
#include <TextureButton.hpp>
#include <NodePath.hpp>
#include <Label.hpp>
#include "constants.h"
#include "ShopItem.h"

namespace godot {

class SubShop : public Control {
    GODOT_CLASS(SubShop, Control)

private:
    int type;

public:
    static void _register_methods();

    SubShop();
    ~SubShop();

    void _init();
    void _ready();
    void _physics_process(float delta);

    // Update shop methods
    void update_item_tree();
    void update_shop(Array playerList, Array itemList);

    //getters and setters
    int get_type();
};

}

#endif //_SUB_SHOP_H_