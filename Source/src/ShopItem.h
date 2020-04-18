#ifndef ShopItem_H
#define ShopItem_H

#include "constants.h"
#include "PlayerStatus.h"
#include "SubShop.h"
#include <Godot.hpp>
#include <Object.hpp>
#include <Control.hpp>
#include <Timer.hpp>
#include <Label.hpp>
#include <TextureProgress.hpp>
#include <string>
#include <ColorRect.hpp>
#include <RichTextLabel.hpp>
#include <InputEventMouseMotion.hpp>
#include <Input.hpp>
#include <NodePath.hpp>
#include <TextureButton.hpp>
#include <Color.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>
#include <Ref.hpp>
#include <PackedScene.hpp>
#include <ResourceLoader.hpp>

namespace godot {

    class ShopItem : public Control {
        GODOT_CLASS(ShopItem, Control);

    private:
        float attDamage;
        float range;
        float attSpeed;
        float maxHP;
        float mvmtSpeed;
        int numJumps;
        int cost;
        bool finalItem;
        bool purchased;
        bool purchasable;
        Array requirements;
        ColorRect* imgBox;
        ColorRect* descBox;
        RichTextLabel* descText;
        String darkImagePath;
        String lightImagePath;
        ResourceLoader* resourceLoader; 

    public:
        static void _register_methods();

        ShopItem();
        ~ShopItem();

        void _init();
        void _ready();
        void _process();

        void try_set_purchasable();
        void try_purchase();
        void purchase();
        void apply_stats_to_player();
        bool is_purchasable();
        void set_purchasable(bool nPurchasable);
        bool is_purchased();
        void set_purchased(bool nPurchased);
        bool is_final();
        String get_dark_image();
        String get_light_image();
        void set_image(String imagePath);
        void change_bg_color();
    };
}

#endif