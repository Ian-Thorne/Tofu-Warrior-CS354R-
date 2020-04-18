#include "GliderPickup.h"

using namespace godot;

/*
 * Register all methods that Godot needs to call.
 */
void GliderPickup::_register_methods() {
    register_method("_ready", &GliderPickup::_ready);
    register_method("_process", &GliderPickup::_process);
    register_method("destroy", &GliderPickup::destroy, GODOT_METHOD_RPC_MODE_DISABLED);
}

GliderPickup::GliderPickup() {}

GliderPickup::~GliderPickup() {}

void GliderPickup::_init() {}

void GliderPickup::_ready() {
    
}

void GliderPickup::_process() {
    rotate_y(0.05);
}

void GliderPickup::destroy(){
    queue_free();
}
