#include "FamiliarPickup.h"

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void FamiliarPickup::_register_methods() {
    register_method("_ready", &FamiliarPickup::_ready);
    register_method("_process", &FamiliarPickup::_process);
    register_method("destroy", &FamiliarPickup::destroy);
}

FamiliarPickup::FamiliarPickup() {}

FamiliarPickup::~FamiliarPickup() {}

void FamiliarPickup::_init() {}

void FamiliarPickup::_ready() {}

void FamiliarPickup::_process() {
    rotate_y(0.05);
}

void FamiliarPickup::destroy(){
    queue_free();
}