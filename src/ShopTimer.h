#ifndef _SHOP_TIMER_H_
#define _SHOP_TIMER_H_

#include "constants.h"
#include <Godot.hpp>
#include <MarginContainer.hpp>
#include <Label.hpp>
#include <Timer.hpp>

namespace godot {

class ShopTimer : public MarginContainer {
    GODOT_CLASS(ShopTimer, MarginContainer)

private:
    int secondsRemaining;
    Timer* myTimer;
    Label* myLabel;

public:
    static void _register_methods();

    ShopTimer();
    ~ShopTimer();

    void _init();
    void _ready();

    void _decrement_timer();
    int get_seconds_remaining();
    void set_seconds_remaining(int newSecondsRemaining);
};

}

#endif //_SHOP_TIMER_H_