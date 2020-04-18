//Insert imports for each script here
#include "Player.h"
#include "PlayerStatus.h"
#include "CameraScript.h"
#include "Camera_Follow.h"
#include "GUI.h"
#include "GameTimer.h"
#include "Coin.h"
#include "SwordPickup.h"
#include "GliderPickup.h"
#include "Enemy.h"
#include "menuScreen.h"
#include "Lobby.h"
#include "Network.h"
#include "Game.h"
#include "GameManager.h"
#include "WinScreen.h"
#include "LoseScreen.h"
#include "AIEnemy.h"
#include "Waypoint.h"
#include "Familiar.h"
#include "FamiliarPickup.h"
#include "PauseMenu.h"
#include "Shop.h"
#include "SubShop.h"
#include "ShopTimer.h"
#include "ShopItem.h"
#include "HealthPotion.h"
#include "Credits.h"
#include "HowTo.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);
	godot::register_class<godot::Player>();
	godot::register_class<godot::PlayerStatus>();
	godot::register_class<godot::CameraScript>();
	godot::register_class<godot::Camera_Follow>();
	godot::register_class<godot::GUI>();
	godot::register_class<godot::GameTimer>();
	godot::register_class<godot::Coin>();
	godot::register_class<godot::SwordPickup>();
	godot::register_class<godot::GliderPickup>();
	godot::register_class<godot::Enemy>();
    godot::register_class<godot::menuScreen>();
    godot::register_class<godot::Lobby>();
	godot::register_class<godot::Network>();
	godot::register_class<godot::Game>();
	godot::register_class<godot::GameManager>();
	godot::register_class<godot::WinScreen>();
	godot::register_class<godot::LoseScreen>();
	godot::register_class<godot::AIEnemy>();
	godot::register_class<godot::Waypoint>();
	godot::register_class<godot::Familiar>();
	godot::register_class<godot::FamiliarPickup>();
	godot::register_class<godot::PauseMenu>();
	godot::register_class<godot::Shop>();
	godot::register_class<godot::SubShop>();
	godot::register_class<godot::ShopTimer>();
	godot::register_class<godot::ShopItem>();
	godot::register_class<godot::HealthPotion>();
	godot::register_class<godot::Credits>();
	godot::register_class<godot::HowTo>();
}
