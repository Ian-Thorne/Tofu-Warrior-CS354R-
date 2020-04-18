#include "CameraScript.h"


////////////////////////////////////////
///Settings Defaults declaration area///
////////////////////////////////////////

#define MOUSE_SENSITIVITY 1 //.005
#define TOPBOUND -30
#define BOTTOMBOUND 5

///////////////////////////////

using namespace godot;

void CameraScript::_register_methods() {
	register_method("_input", &CameraScript::_input);
	register_method("_ready", &CameraScript::_ready);
	register_method("_process", &CameraScript::_process);
	register_property<CameraScript, float>("sensitivity", &CameraScript::sensitivity, MOUSE_SENSITIVITY);
}

CameraScript::CameraScript() {}
CameraScript::~CameraScript() {}

void CameraScript::_init() {
	sensitivity = MOUSE_SENSITIVITY;
}

void CameraScript::_ready() {
	myRay = Object::cast_to<RayCast>(get_node("InnerGimbal/Camera/RayCast"));
	//add an exception for the player
	myRay->add_exception(get_node(".."));

	hidden = nullptr;
	lastHidden = nullptr;
	sensitivity = sensitivity / 200;
}

void CameraScript::_process(float delta) {
	if(is_network_master()){
		if(myRay->is_colliding()) {
			lastHidden = hidden;
			hidden = Object::cast_to<Spatial>(myRay->get_collider());
			if(hidden != nullptr && lastHidden != nullptr && hidden != lastHidden) {
				lastHidden->show();
			}
			hidden->hide();
		} else {
			if(hidden != nullptr) {
				hidden->show();
			}
		}
	}
}

void CameraScript::_input(InputEvent* event){
	InputEventMouseMotion* mEvent = Object::cast_to<InputEventMouseMotion>(event);
	Player* player = Object::cast_to<Player>(get_node(".."));
	if(mEvent != NULL){
		Spatial* innerGimbal = Object::cast_to<Spatial>(get_node("InnerGimbal"));
		Vector2 mMotion = mEvent->get_relative();

		//rotate(Vector3(0,1,0), mMotion.x * sensitivity);
		innerGimbal->rotate(Vector3(1,0,0), mMotion.y * sensitivity * -1);
		if(player->is_network_master()) {
			player->rpc("turn_y", mMotion.x * sensitivity * -1);
		}

		if(innerGimbal->get_rotation_degrees().x > BOTTOMBOUND){
			Vector3 newLoc = innerGimbal->get_rotation_degrees();
			newLoc.x = BOTTOMBOUND;
			innerGimbal->set_rotation_degrees(newLoc);
		}

		if(innerGimbal->get_rotation_degrees().x < TOPBOUND){
			Vector3 newLoc = innerGimbal->get_rotation_degrees();
			newLoc.x = TOPBOUND;
			innerGimbal->set_rotation_degrees(newLoc);
		}
	}

}

