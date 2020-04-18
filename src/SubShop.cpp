#include "SubShop.h"

using namespace godot;

void SubShop::_register_methods() {
    register_method("_init", &SubShop::_init);
    register_method("_ready", &SubShop::_ready);
	register_method("update_item_tree", &SubShop::update_item_tree);
    register_method("update_shop", &SubShop::update_shop);
	register_method("get_type", &SubShop::get_type);
	
	register_property<SubShop, int>("type", &SubShop::type, WEAPON_SHOP);
}

SubShop::SubShop() {}
SubShop::~SubShop() {}

void SubShop::_init() {
	type = WEAPON_SHOP;
}

void SubShop::_ready() {
    //connect this SubShop's back to main shop button to the GameManager
    get_node("MainShopContainer/MainShop")->connect("pressed", get_node("/root/GameManager"), "switch_to_shop");

	//update the item tree initially, future updates will happen as items are purchased
    update_item_tree();
}

void SubShop::update_item_tree() {
	//update the gold count
	PlayerStatus* playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
	Label* goldCountText = Object::cast_to<Label>(get_node("/root/Shop/GoldCounter/Background/Number"));
	goldCountText->set_text(std::to_string(playerStatus->get_gold()).c_str());

	Array playerList = Array();
    Array itemNodeList = Array();
    if(type == WEAPON_SHOP) {
    	playerList = get_node("/root/PlayerStatus")->call("get_collected_weapons"); // Grab player's weapon list
	} else if(type == ARMOR_SHOP) {
		playerList = get_node("/root/PlayerStatus")->call("get_collected_armor"); // Grab player's armor list
	} else if(type == BOOT_SHOP) {
		playerList = get_node("/root/PlayerStatus")->call("get_collected_boots"); // Grab player's boot list
	} else if(type == MISC_SHOP) {
		playerList = get_node("/root/PlayerStatus")->call("get_collected_misc"); // Grab player's misc list
	} else {
		Godot::print("Error: Shop not found.");
	}    

    itemNodeList = get_node("Items")->call("get_children"); // Grab weapon shop's item list

    if(playerList.empty() || itemNodeList.empty()) {
    	Godot::print("Player list or shop item list is empty.");
	}

	Array itemList = Array();
	for(int i = 0; i < itemNodeList.size(); i++) {
		Node* temp = itemNodeList[i];
		itemList.append(temp->get_name());
	}

    update_shop(playerList, itemList);
	Godot::print("UPDATE SHOP FINISHED");
}


void SubShop::update_shop(Array playerList, Array itemList){
	if(itemList.empty()) {
		Godot::print("Item list is empty");
	}
	if(playerList.empty()) {
		Godot::print("Player list is empty");
	}

	for(int i = 0; i < playerList.size(); i++) {
		String temp = playerList[i];
		Godot::print(temp);
	}
	//item list is an array of nodes
	for(int i = 0; i < itemList.size(); i++) {
		String temp = itemList[i];
		Godot::print(temp);
	}

	String currPlayerItem = "";
	//ShopItem currShopItem = {};
	
	// Iterate through the player's inventory and change item's 
	// purchased boolean to true
	for(int i = 0; i < playerList.size(); i++){
		currPlayerItem = playerList[i]; // *****MIGHT NEED TO CAST TO A GODOT STRING*****
		int itemIndex = itemList.find(currPlayerItem);
		if(itemIndex > -1){
			String itemName = itemList[itemIndex];
			get_node(NodePath("/root/Shop/Items/" + itemName))->call("set_purchased", true);
		} else {
			Godot::print("Shop item not found");
		}
	}
	
	//determine if the player has purchased a final item of the current type
	bool playerHasFinalItem = false;
	String playerFinalItem = "";
	if(type == WEAPON_SHOP) {
    	playerHasFinalItem = get_node("/root/PlayerStatus")->call("check_has_final_weapon");
		playerFinalItem = get_node("/root/PlayerStatus")->call("check_which_final_weapon");
	} else if(type == ARMOR_SHOP) {
		playerHasFinalItem = get_node("/root/PlayerStatus")->call("check_has_final_armor");
		playerFinalItem = get_node("/root/PlayerStatus")->call("check_which_final_armor");
	} else if(type == BOOT_SHOP) {
		playerHasFinalItem = get_node("/root/PlayerStatus")->call("check_has_final_boots");
		playerFinalItem = get_node("/root/PlayerStatus")->call("check_which_final_boots");
	} else if(type == MISC_SHOP) {
		playerHasFinalItem = false;
	} else {
		Godot::print("Error: Shop not found.");
	}  

	// Iterate through the shop's list of items and try to set
	// them to purchased
	for(int i = 0; i < itemList.size(); i++){
		String itemName = itemList[i];
		ShopItem* item = Object::cast_to<ShopItem>(get_node(NodePath("/root/Shop/Items/" + itemName)));
		if(item->is_final() && playerHasFinalItem) {
			if(itemName == playerFinalItem) {
				item->set_purchasable(true);
				item->set_purchased(true);
			} else {
				item->set_purchasable(false);
			}
		} else {
			item->try_set_purchasable();
		}
		if(item->is_purchasable()) {
			item->set_image(item->get_light_image());
			if(item->is_purchased()) {
				item->change_bg_color();
			}
		}
		else {
			item->set_image(item->get_dark_image());
		}
	}
}

int SubShop::get_type() {
	return type;
}