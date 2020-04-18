#ifndef CAMERASCRIPT
#define CAMERASCRIPT

#include <Godot.hpp>
#include <Spatial.hpp>
#include <InputEvent.hpp>
#include <InputEventMouseMotion.hpp>
#include <RayCast.hpp>
#include "Player.h"


namespace godot{

class CameraScript : public Spatial{
	GODOT_CLASS(CameraScript, Spatial)

private:
	RayCast* myRay;
	Spatial* hidden;
	Spatial* lastHidden;
	float sensitivity;

public:
	static void _register_methods();

	CameraScript();
	~CameraScript();

	void _init();
	void _ready();
	void _process(float delta);
	void _input(InputEvent* event);
};

}

#endif
