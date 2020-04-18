#include "Lobby.h"
#include "PlayerStatus.h"
#include <SceneTree.hpp>
#include <ResourceLoader.hpp>
#include <Engine.hpp>
#include <ClassDB.hpp>
#include <ProjectSettings.hpp>

using namespace godot;

void Lobby::_register_methods()
{
    register_method("_init", &Lobby::_init, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_ready", &Lobby::_ready, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_TextField_text_changed", &Lobby::_on_TextField_text_changed, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_load_game", &Lobby::_load_game, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_CreateButton_pressed", &Lobby::_on_CreateButton_pressed, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_JoinButton_pressed", &Lobby::_on_JoinButton_pressed, GODOT_METHOD_RPC_MODE_DISABLED);

    register_property<Lobby, String>("playerName", &Lobby::playerName, "", GODOT_METHOD_RPC_MODE_DISABLED);
}

Lobby::Lobby()
{

}

Lobby::~Lobby()
{

}

void Lobby::_init()
{
    playerName = "";
}

void Lobby::_ready()
{
    get_node("./VBoxContainer/HBoxContainer/createServerB")->connect("pressed", this, "_on_CreateButton_pressed");
    get_node("./VBoxContainer/HBoxContainer/joinServerB")->connect("pressed", this, "_on_JoinButton_pressed");
    get_node("./VBoxContainer/nameField")->connect("text_changed", this, "_on_TextField_text_changed");

    IP* myIPSingleton = IP::get_singleton();
    Array mip = myIPSingleton->get_local_addresses();

    get_node("./myIPLabel")->call("set_text", mip[1]);
}

void Lobby::_on_TextField_text_changed(String newText)
{
    playerName = newText;
}

void Lobby::_on_CreateButton_pressed()
{
    if(playerName == "")
    {
        return;
    }

    get_node("/root/Network")->call("create_server", playerName);
    _load_game();
}

void Lobby::_on_JoinButton_pressed()
{
    if(playerName == "")
    {
        return;
    }

    String targIP = get_node("./VBoxContainer/ipField")->call("get_text");
    Godot::print(targIP);

    if(targIP != "") {
        get_node("/root/Network")->call("connect_to_server", playerName, targIP);
    }
    else {
        return;
    }
    
    _load_game();
}

void Lobby::_load_game()
{
    PlayerStatus* playerStatus = Object::cast_to<PlayerStatus>(get_node("/root/PlayerStatus"));
    playerStatus->new_status();
    get_tree()->change_scene("res://MultiMain.tscn");
}