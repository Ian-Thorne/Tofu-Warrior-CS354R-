#ifndef GAMETIMER_H
#define GAMETIMER_H

#include "Network.h"
#include "Player.h"
#include <Godot.hpp>
#include <Object.hpp>
#include <MarginContainer.hpp>
#include <Label.hpp>
#include <Timer.hpp>
#include <String.hpp>
#include <Control.hpp>
#include <SceneTree.hpp>
#include <Array.hpp>
#include <string>

namespace godot {

class GameTimer : public MarginContainer {
    GODOT_CLASS(GameTimer, MarginContainer);

private:
    int secondsRemaining;
    Timer* myTimer;
    Label* myLabel;

public:
    static void _register_methods();

    GameTimer();
    ~GameTimer();

    void _init();
    void _ready();

    int get_time_left();
    void set_time_left(int timeRemaining);

    void _decrement_timer();
};

}

#endif