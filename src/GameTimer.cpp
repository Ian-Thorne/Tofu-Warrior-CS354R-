#include "GameTimer.h"

const int DEFAULT_TIME = 300;

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void GameTimer::_register_methods() {
    register_property<GameTimer, int>("secondsRemaining", &GameTimer::secondsRemaining, DEFAULT_TIME);
    register_method("_ready", &GameTimer::_ready);
    register_method("_decrement_timer", &GameTimer::_decrement_timer);
    register_method("get_time_left", &GameTimer::get_time_left, GODOT_METHOD_RPC_MODE_PUPPET);
    register_method("set_time_left", &GameTimer::get_time_left, GODOT_METHOD_RPC_MODE_PUPPET);
}

GameTimer::GameTimer() {}

GameTimer::~GameTimer() {}

void GameTimer::_init() {}

void GameTimer::_ready() {
    //set the timer's starting value
    secondsRemaining = DEFAULT_TIME;

    //find all of the GameTimer's associated nodes
    myTimer = Object::cast_to<Timer>(get_node("Background/Label/Timer"));
    myLabel = Object::cast_to<Label>(get_node("Background/Label"));

    //connect the timer node's timeout signal to decrement the GameTimer
    myTimer->connect("timeout", this, "_decrement_timer");
    myTimer->set_wait_time(1);
    myTimer->start();
}

void GameTimer::_decrement_timer() {
    secondsRemaining--;
    int minutes = secondsRemaining / 60;
    int seconds = secondsRemaining % 60;
    String temp;
    if(seconds < 10) {
        temp = String(std::to_string(minutes).c_str()) + ":0" + String(std::to_string(seconds).c_str());
    } else {
        temp = String(std::to_string(minutes).c_str()) + ":" + String(std::to_string(seconds).c_str());
    }
    myLabel->set_text(temp);
    //if time runs out, end the game
    if(secondsRemaining <= 0) {
        // Control* gameOver = Object::cast_to<Control>(get_node("../../../../../../GameOverScreen"));
        // gameOver->show();
        if(is_network_master()) {
            Network* theNetwork = Object::cast_to<Network>(get_node("/root/Network"));
            if(theNetwork->singlePlayerMode) {
                //if the game is in single player mode and time runs out, the player loses
                get_node("/root/Spatial/1")->call("lose");
            } else {
                //if the game is in multiplayer mode and time runs out, the player with the most gold
                //wins and the rest of the players lose
                Array players = get_tree()->get_nodes_in_group("Players");
                Player* loser;
                int leastGold = 1000000;
                for(int i = 0; i < players.size(); i++) {
                    Player* currPlayer = Object::cast_to<Player>(players[i]);
                    Godot::print(currPlayer->get_name());
                    Godot::print(std::to_string(currPlayer->get_gold()).c_str());
                    if(currPlayer->get_gold() < leastGold) {
                        leastGold = currPlayer->get_gold();
                        loser = currPlayer;
                    }
                }
                //loser is the instance of that player in the server's game, thus we'll
                //see if the loser is the server's player to determine the loser; then
                //use the loser Player node to call lose() or win() accordgingly
                Player* myPlayer = Object::cast_to<Player>(get_node("/root/Spatial/1"));
                if(loser->get_name() == "1") {
                    myPlayer->lose();
                } else {
                    myPlayer->win();
                }
            }
        }
    }
}

int GameTimer::get_time_left() {
    return secondsRemaining;
}

void GameTimer::set_time_left(int timeRemaining) {
    secondsRemaining = timeRemaining;
}