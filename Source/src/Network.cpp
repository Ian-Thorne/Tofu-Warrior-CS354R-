#include "Network.h"
#include <SceneTree.hpp>
#include <NetworkedMultiplayerENet.hpp>
#include <ResourceLoader.hpp>
#include "Player.h"
#include "GameTimer.h"

using namespace godot;

void Network::_register_methods()
{
    register_method("_init", &Network::_init, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_ready", &Network::_ready, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("create_server", &Network::create_server, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("leave_server", &Network::leave_server, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("connect_to_server", &Network::connect_to_server, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_connected_to_server", &Network::_connected_to_server, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_player_disconnected", &Network::_on_player_disconnected, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_player_connected", &Network::_on_player_connected, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_request_player_info", &Network::_request_player_info, GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("_request_players", &Network::_request_players, GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("_send_player_info", &Network::_send_player_info, GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("update_position", &Network::update_position, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("append_deleteList", &Network::append_deleteList, GODOT_METHOD_RPC_MODE_REMOTESYNC);
    register_method("reap_node", &Network::reap_node, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("sync_timer", &Network::sync_timer, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("show_server_sword", &Network::show_server_sword, GODOT_METHOD_RPC_MODE_REMOTE);

    register_property<Network, Dictionary>("selfData", &Network::selfData, Dictionary(), GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Network, Dictionary>("players", &Network::players, Dictionary(), GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Network, Array>("deleteList", &Network::deleteList, Array(), GODOT_METHOD_RPC_MODE_PUPPET);
    register_property<Network, bool>("singlePlayerMode",&Network::singlePlayerMode, false, GODOT_METHOD_RPC_MODE_MASTER);
}

Network::Network()
{

}

Network::~Network()
{

}

void Network::_init()
{
    selfData["name"] = "";
    selfData["position"] = Vector3(-24, 10, 0);
    deleteList = Array();
    singlePlayerMode = false;
}

void Network::_ready()
{
    get_tree()->connect("network_peer_disconnected", this, "_on_player_disconnected");
    get_tree()->connect("network_peer_connected", this, "_on_player_connected");
}

void Network::create_server(String playerNickname)
{
    deleteList = Array();
    int numPlayers;
    if(!singlePlayerMode) {
        numPlayers = MAX_PLAYERS;
    } else {
        numPlayers = 1;
    }
    selfData["name"] = playerNickname;
    players[1] = selfData;
    NetworkedMultiplayerENet* peer = NetworkedMultiplayerENet::_new();
    peer->set_bind_ip("*");
    peer->create_server(SERVER_PORT, numPlayers);
    get_tree()->set_network_peer(peer);
}

void Network::leave_server() {
    get_tree()->get_network_peer()->call("close_connection");
}

void Network::connect_to_server(String playerNickname, String serverIP)
{
    selfData["name"] = playerNickname;
    get_tree()->connect("connected_to_server", this, "_connected_to_server");
    NetworkedMultiplayerENet* peer = NetworkedMultiplayerENet::_new();
    peer->create_client(serverIP, SERVER_PORT);
    get_tree()->set_network_peer(peer);
}

void Network::_connected_to_server()
{
    int64_t localPlayerId = get_tree()->get_network_unique_id();
    players[localPlayerId] = selfData;
    rpc("_send_player_info", localPlayerId, selfData);
}

void Network::_on_player_disconnected(int64_t id)
{
    players.erase(id);
}

void Network::_on_player_connected(int64_t connectedPlayerId)
{
    std::cout << "Player connected to server" << std::endl;
    int64_t localPlayerId = get_tree()->get_network_unique_id();
    if(!get_tree()->is_network_server())
    {
        rpc_id(1, "_request_player_info", localPlayerId, connectedPlayerId);
    }
}

void Network::_request_player_info(int64_t requestFromId, int64_t playerId)
{
    if(get_tree()->is_network_server())
    {   
        rpc_id(requestFromId, "_send_player_info", playerId, players[playerId]);
        //delete all nodes that the other player picked up before you joined
        for(int i = 0; i < deleteList.size(); i++) {
            rpc_id(requestFromId, "reap_node", deleteList[i]);
        }
        //make the server's character's sword appear if it should
        Player* serverPlayer = Object::cast_to<Player>(get_node("/root/Spatial/1"));
        if(serverPlayer->get_has_sword()) {
            rpc_id(requestFromId, "show_server_sword");
        }
        //set your time remaining equal to the server's time remaining
        GameTimer* timer = Object::cast_to<GameTimer>(get_node("/root/Spatial/Control/GUI/HBoxContainer/Rows/HBoxContainer/Timer"));
        int timeRemaining = timer->get_time_left();
        rpc_id(requestFromId, "sync_timer", timeRemaining);
    }
}

void Network::_request_players(int64_t requestFromId)
{
    if(get_tree()->is_network_server())
    {
        for(int64_t peerId = 0; peerId < players.size(); peerId++)
        {
            if(peerId != requestFromId)
            {
                rpc_id(requestFromId, "_send_player_info", peerId, players[peerId]);
            }
        }
    }
}

void Network::_send_player_info(int64_t id, Dictionary info)
{
    players[id] = info;

    ResourceLoader* resourceLoader = ResourceLoader::get_singleton();
    PlayerScene = resourceLoader->load("res://Player.tscn");
    godot::Player* player = static_cast<godot::Player*>(PlayerScene->instance());
    player->set_name(std::to_string(id).c_str());
    player->set_network_master(id);
    get_node("/root/Spatial")->add_child(player);
    player->minit(info["name"], info["position"], true);
}

void Network::update_position(int64_t id, Vector3 position)
{
    Dictionary playerInfo = players[id];
    playerInfo["position"] = position;
}

void Network::append_deleteList(String path){
    deleteList.append(path);
}

void Network::reap_node(NodePath path) {
    if(get_node(path)) {
        get_node(path)->queue_free();
    }
}

void Network::sync_timer(int timeRemaining) {
    GameTimer* timer = Object::cast_to<GameTimer>(get_node("/root/Spatial/Control/GUI/HBoxContainer/Rows/HBoxContainer/Timer"));
    timer->set_time_left(timeRemaining);
}

void Network::show_server_sword() {
    get_node("/root/Spatial/1/Sword")->call("show");
}