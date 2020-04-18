#ifndef CAMERA_FOLLOW
#define CAMERA_FOLLOW

#include <Godot.hpp>
#include <Camera.hpp>

namespace godot{

	class Camera_Follow: public Camera{
		GODOT_CLASS(Camera_Follow, Camera)

	private:
		float distance;
		float height;

	public:
		static void _register_methods();

		Camera_Follow();
		~Camera_Follow();

		void _init();
		void _ready();

		void _physics_process(float delta);

	};

}

#endif
