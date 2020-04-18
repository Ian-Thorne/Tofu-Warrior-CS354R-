#include "ShopItem.h"

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void ShopItem::_register_methods() {
    register_method("_init", &ShopItem::_init);
    register_method("_ready", &ShopItem::_ready);
    register_method("_process", &ShopItem::_process);
    register_method("try_set_purchasable", &ShopItem::try_set_purchasable);
    register_method("try_purchase", &ShopItem::try_purchase);
    register_method("purchase", &ShopItem::purchase);
    register_method("apply_stats_to_player", &ShopItem::apply_stats_to_player);
    register_method("is_purchasable", &ShopItem::is_purchasable);
    register_method("set_purchasable", &ShopItem::set_purchasable);
    register_method("is_purchased", &ShopItem::is_purchased);
    register_method("set_purchased", &ShopItem::set_purchased);
    register_method("is_final", &ShopItem::is_final);
    register_method("get_dark_image", &ShopItem::get_dark_image);
    register_method("get_light_image",&ShopItem::get_light_image);
    register_method("set_image", &ShopItem::set_image);
    register_method("change_bg_color", &ShopItem::change_bg_color);

    register_property<ShopItem, float>("attackDamage", &ShopItem::attDamage, 0);
    register_property<ShopItem, float>("range", &ShopItem::range, 0);
    register_property<ShopItem, float>("attackSpeed", &ShopItem::attSpeed, 0);
    register_property<ShopItem, float>("maxHP", &ShopItem::maxHP, 0);
    register_property<ShopItem, float>("mvmtSpeed", &ShopItem::mvmtSpeed, 0);
    register_property<ShopItem, int>("numJumps", &ShopItem::numJumps, 0);
    register_property<ShopItem, int>("cost", &ShopItem::cost, 0);
    register_property<ShopItem, bool>("finalItem", &ShopItem::finalItem, false);
    register_property<ShopItem, bool>("purchased", &ShopItem::purchased, false);
    register_property<ShopItem, bool>("purchasable", &ShopItem::purchasable, false);
    register_property<ShopItem, Array>("requirements", &ShopItem::requirements, Array());
    register_property<ShopItem, String>("darkImagePath", &ShopItem::darkImagePath, "res://icon.png");
    register_property<ShopItem, String>("lightImagePath", &ShopItem::lightImagePath, "res://icon.png");

    register_signal<ShopItem>((char*) "item_purchased");
    register_signal<ShopItem>((char*) "final_item_purchased");
}

ShopItem::ShopItem() {}

ShopItem::~ShopItem() {}

void ShopItem::_init() {
    attDamage = 0;
    range = 0;
    attSpeed = 0;
    maxHP = 0;
    mvmtSpeed = 0;
    numJumps = 0;
    cost = 0;
    finalItem = false;
    purchased = false;
    purchasable = false;
    requirements = Array();
}

void ShopItem::_ready() {
    imgBox = Object::cast_to<ColorRect>(get_node("ItemBG"));
    descBox = Object::cast_to<ColorRect>(get_node("DescBG"));
    descText = Object::cast_to<RichTextLabel>(get_node("DescBG/Desc"));
    descBox->hide();
    //darkImagePath = "res://icon.png";
    //lightImagePath = "res://icon.png";
    resourceLoader = ResourceLoader::get_singleton();

    //connect button to purchase(), so clicking this ShopItem will purchase it
    get_node("ItemBG/ItemButton")->connect("pressed", this, "try_purchase");

    //connect this ShopItem's item_purchased signal to the SubShop, so the tree
    //can update if this ShopItem is purchased
    connect("item_purchased", get_node("/root/Shop"), "update_item_tree");

    // //connect this ShopItem's final_item_purchased signal to the PlayerStatus
    // //singleton, so no more than one final item can be purchased
    // connect("final_item_purchased", get_node("/root/PlayerStatus"), "collect_final_item");
}

void ShopItem::_process(){
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

void ShopItem::try_set_purchasable(){
    int num = requirements.size();
    bool canPurchase = true;
    for(int i = 0; i < num; i++){
        String pathString = requirements[i];
        Node* temp = get_node(NodePath(pathString));
        canPurchase = canPurchase && Object::cast_to<ShopItem>(temp)->is_purchased();
    }
    purchasable = canPurchase;
}

void ShopItem::try_purchase() {
    int playerGold = get_node("/root/PlayerStatus")->call("get_gold");
    if(purchasable && (playerGold >= cost) && !purchased) {
        purchase();
    }
}

void ShopItem::purchase() {
    PlayerStatus* playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
    playerStatus->change_gold(cost * -1);
    apply_stats_to_player();
    SubShop* subShop = Object::cast_to<SubShop>(get_node("/root/Shop"));
    int shopType = subShop->get_type();
    if(shopType == WEAPON_SHOP) {
        playerStatus->add_to_collected_weapons(get_name());
        if(finalItem) {
            playerStatus->collect_final_weapon(get_name());
        }
    } else if(shopType == ARMOR_SHOP) {
        playerStatus->add_to_collected_armor(get_name());
        if(finalItem) {
            playerStatus->collect_final_armor(get_name());
        }
    } else if(shopType == BOOT_SHOP) {
        playerStatus->add_to_collected_boots(get_name());
        if(finalItem) {
            playerStatus->collect_final_boots(get_name());
        }
    } else if(shopType == MISC_SHOP) {
        playerStatus->add_to_collected_misc(get_name());
    } else {
        Godot::print("Error: SubShop seems to have invalid type");
    }
    emit_signal("item_purchased");
}

void ShopItem::apply_stats_to_player() {
    PlayerStatus* playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
    playerStatus->change_attack(attDamage);
    playerStatus->change_attack_range(range);
    playerStatus->change_attack_speed(attSpeed);
    playerStatus->change_max_hp(maxHP);
    playerStatus->change_hp(maxHP);
    playerStatus->change_move_speed(mvmtSpeed);
    playerStatus->change_max_jumps(numJumps);
}

bool ShopItem::is_purchasable(){
    return purchasable;
}

void ShopItem::set_purchasable(bool nPurchasable){
    purchasable = nPurchasable;
}

bool ShopItem::is_purchased() {
    return purchased;
}

void ShopItem::set_purchased(bool nPurchased){
    purchased = nPurchased;
}

bool ShopItem::is_final() {
    return finalItem;
}

String ShopItem::get_dark_image(){
    return darkImagePath;
}

String ShopItem::get_light_image(){
    return lightImagePath;
}

void ShopItem::set_image(String imagePath){
    //Image *img = new Image();
    Ref<Image> img;
    ImageTexture *tex = new ImageTexture();
    //img->load(imagePath);
    img = resourceLoader->load(imagePath);
    //tex->create_from_image(img);
    TextureButton* button = Object::cast_to<TextureButton>(get_node("ItemBG/ItemButton"));
    button->set_normal_texture(img);
}

void ShopItem::change_bg_color(){
    ColorRect* rect = Object::cast_to<ColorRect>(get_node("ItemBG"));
    rect->set_frame_color(Color(255,239,0));
}