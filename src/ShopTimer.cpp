#include "ShopTimer.h"

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void ShopTimer::_register_methods() {
    register_method("_init", &ShopTimer::_init);
    register_method("_ready", &ShopTimer::_ready);
    register_method("_decrement_timer", &ShopTimer::_decrement_timer);
    register_method("get_seconds_remaining", &ShopTimer::get_seconds_remaining);
    register_method("set_seconds_remaining", &ShopTimer::set_seconds_remaining);

    register_signal<ShopTimer>((char*) "shop_time_up");
}

ShopTimer::ShopTimer() {}
ShopTimer::~ShopTimer() {}

void ShopTimer::_init() {
    secondsRemaining = DEFAULT_SHOP_TIME_LIMIT;
}

void ShopTimer::_ready() {
    //find all of the ShopTimer's associated nodes
    myTimer = Object::cast_to<Timer>(get_node("Background/Label/Timer"));
    myLabel = Object::cast_to<Label>(get_node("Background/Label"));

    //connect the timer node's timeout signal to decrement this ShopTimer
    myTimer->connect("timeout", this, "_decrement_timer");
    myTimer->set_wait_time(1);
    myTimer->start();

    //connect this ShopTimer's shop_time_up signal to the GameManager
    connect("shop_time_up", get_node("/root/GameManager"), "switch_to_level");
}

void ShopTimer::_decrement_timer() {
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
    if(secondsRemaining == 0) {
        emit_signal("shop_time_up");
    }
}

int ShopTimer::get_seconds_remaining() {
    return secondsRemaining;
}

void ShopTimer::set_seconds_remaining(int newSecondsRemaining) {
    secondsRemaining = newSecondsRemaining;
}