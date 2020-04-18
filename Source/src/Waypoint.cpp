#include "Waypoint.h"

using namespace godot;

/*
 * Register all methods and properties that Godot will need to access.
 */
void Waypoint::_register_methods() {
    register_method("_ready", &Waypoint::_ready);
    register_method("get_dist", &Waypoint::get_dist);
    register_method("get_connected_waypoints", &Waypoint::get_connected_waypoints);
    register_method("_on_area_entered", &Waypoint::_on_area_entered);
    register_method("_on_area_exited", &Waypoint::_on_area_exited);
    register_method("take", &Waypoint::take);
    register_method("untake", &Waypoint::untake);
    register_method("is_taken", &Waypoint::is_taken);
    register_method("toggle_visibility", &Waypoint::toggle_visibility);

    register_property<Waypoint, Array>("connectedWaypoints", &Waypoint::connectedWaypoints, Array());
    register_property<Waypoint, Vector3>("pos", &Waypoint::pos, Vector3(0,0,0));
}

Waypoint::Waypoint() {}
Waypoint::~Waypoint() {}

void Waypoint::_init() {
    pos = Vector3(0,0,0);
    connectedWaypoints = Array();
    taken = false;
    visible = false;
}

void Waypoint::_ready() {
    pos = get_global_transform().origin;
    add_to_group("Waypoints");
    get_node("./Area")->connect("area_entered", this, "_on_area_entered");
    get_node("./Area")->connect("area_exited", this, "_on_area_exited");
}

float Waypoint::get_dist(Vector3 other){
    return get_global_transform().origin.distance_to(other);
}

void Waypoint::take() {
    taken = true;
}

void Waypoint::untake() {
    taken = false;
}

bool Waypoint::is_taken() {
    return taken;
}

Array Waypoint::get_connected_waypoints(){
    return connectedWaypoints;
}

void Waypoint::_on_area_entered(Area* other){
    if(other->get_name() == "AIEnemy") {
        other->get_parent()->call("set_on_waypoint", true);
 		other->get_parent()->call("waypoint_reached", get_path());
 	}
    if(other->get_name() == "Familiar") {
 		other->get_parent()->call("waypoint_reached", get_path());
    }
}

void Waypoint::_on_area_exited(Area* other) {
    if(other->get_name() == "AIEnemy") {
        other->get_parent()->call("set_on_waypoint", false);
    }
}

void Waypoint::toggle_visibility() {
    if(visible) {
        hide();
    } else {
        show();
    }
    visible = !visible;
}