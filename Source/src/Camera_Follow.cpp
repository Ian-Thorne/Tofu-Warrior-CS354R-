#include "Camera_Follow.h"
#include <iostream>

using namespace godot;

void Camera_Follow::_register_methods() {
	register_method("_physics_process", &Camera_Follow::_physics_process);
	register_method("_ready", &Camera_Follow::_ready);
	register_property<Camera_Follow, float>("distance", &Camera_Follow::distance, 4.0);
	register_property<Camera_Follow, float>("height",&Camera_Follow::height, 2.0);
}


Camera_Follow::Camera_Follow() {
}

Camera_Follow::~Camera_Follow() {
	// add your cleanup here
}

void Camera_Follow::_init() {
	// initialize any variables here
}

void Camera_Follow::_ready(){
	distance = 4.0;
	height = 2.0;
	set_physics_process(true);
	set_as_toplevel(true);
}

void Camera_Follow::_physics_process(float delta) {
	Spatial* parent = Object::cast_to<Spatial>(this->get_parent());
	Vector3 target = parent->get_global_transform().origin;
	Vector3 pos = this->get_global_transform().origin;
	Vector3 up = Vector3(0,1,0);

	Vector3 offset = pos - target;

	offset = offset.normalized() * distance;
	offset.y = height;

	pos = target + offset;

	look_at_from_position(pos, target, up);

}

