#include "SwordPickup.h"

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void SwordPickup::_register_methods() {
    register_method("_ready", &SwordPickup::_ready);
    register_method("_process", &SwordPickup::_process);
    register_method("destroy", &SwordPickup::destroy);
}

SwordPickup::SwordPickup() {}

SwordPickup::~SwordPickup() {}

void SwordPickup::_init() {}

void SwordPickup::_ready() {
    //set up the signal to send when the sword pickup is touched
}

void SwordPickup::_process() {
    rotate_y(0.05);
}

void SwordPickup::destroy(){
    queue_free();
}