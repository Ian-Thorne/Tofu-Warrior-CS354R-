#pragma once

#include <Godot.hpp>
#include <Control.hpp>
#include <IP.hpp>

namespace godot
{

class Lobby : public Control
{
    GODOT_CLASS(Lobby, Control)

public:
    static void _register_methods();

    Lobby();
    ~Lobby();

    void _init();
    void _ready();
    void _on_TextField_text_changed(String newText);
    void _on_CreateButton_pressed();
    void _on_JoinButton_pressed();
    void _load_game();

private:
    String playerName;
};

}