#ifndef PLAYER
#define PLAYER

#include "constants.h"
#include "Coin.h"
#include "GliderPickup.h"
#include "SwordPickup.h"
#include "Enemy.h"
#include "AIEnemy.h"
#include "Network.h"
#include "PlayerStatus.h"
#include <Godot.hpp>
#include <KinematicBody.hpp>
#include <MeshInstance.hpp>
#include <Camera.hpp>
#include <Input.hpp>
#include <RayCast.hpp>
#include <Transform.hpp>
#include <AudioServer.hpp>
#include <AudioStreamPlayer.hpp>
#include <SceneTree.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <Control.hpp>
#include <Area.hpp>
#include <NetworkedMultiplayerENet.hpp>
#include "FamiliarPickup.h"
#include "Familiar.h"
#include <Viewport.hpp>
#include <Spatial.hpp>
#include <CollisionShape.hpp>

namespace godot{

	class Player: public KinematicBody{
		GODOT_CLASS(Player, KinematicBody);

	private:
		//stats and inventory
		PlayerState state;
		PlayerStatus* myStatus;
		//movement-related members
		Vector3 velocity;
		Vector3 lastCoordinates;
		float currSpeed;
		float walkableAngle;
		//jumping-related members
		float gravity;
		float currGravity;
		float airControl;
		float jumpHeight;
		int currJumps;
		int gliderFactor;
		int gliderFallLimit;
		//sub-nodes
		RayCast* ray;
		//flags
		bool didIWin;
		bool hasStrafeControls;
		//cooldowns
		int jumpCD;
		int attackCD;
		int soundControlCD;
		int waypointControlCD;
		int damageTakenCD;
		//multiplayer variables
		Vector3 slavePosition;
		int64_t nodeName;
		Vector3 slaveVelocity;
		//ai variables
		NodePath closestWaypoint;
		Spatial* sword;
		//only used in multiplayer
		int mpHp;
		int mpMaxHp;
		int mpGold;
		int mpAttack;
		int mpMaxJumps;
		float mpMaxSpeed;
		bool mpHasGlider;
		bool mpHasSword;
		Area* attackArea;
		RayCast* attackRay;

	public:
		static void _register_methods();		

		Player();
		~Player();

		void _init();

		void _ready();

		void _physics_process(float delta);

		//helper methods for processing player input
		void update_cooldowns();
		Vector3 compute_translation(Vector3 direction, Input* input, float delta);
		Vector3 compute_jumping(Vector3 direction, Input* input);
		Vector3 compute_air_mobility(Vector3 direction, Input* input, float delta);
		Vector3 compute_ledges(Vector3 direction, Input* input);
		void compute_attack(Input* input);
		void compute_sound_controls(Input* input);

		//Multiplayer things
		void minit(String name, Vector3 startPosition, bool isSlave);
		bool is_singleplayer();

		//getters and setters
		void change_hp(int delta);
		int get_hp();
		void change_max_hp(int delta);
		int get_max_hp();
		void change_gold(int delta);
		int get_gold();
		void change_attack(int delta);
		int get_attack();
		int get_attack_cd();
		void change_attack_range(int delta);
		int get_attack_range();
		int get_max_jumps();
		float get_max_move_speed();
		bool get_has_sword();
		bool get_has_glider();
		bool get_has_familiar_bell();
		void set_has_familiar();
		bool get_has_familiar();
		void set_win_status(bool newStatus);
		bool get_win_status();
		void stop_moving();
		NodePath get_closest_waypoint();
		Vector3 get_velocity();
		void set_velocity(Vector3 newVelocity);

		//stimulus response methods
		void pickup_item(int itemType);
		void _on_area_entered(Area* other);
		void play_oof();
		void go_to_end_screen();
		void lose();
		void win();

		//ai methods
		void find_closest_waypoint();

		//other methods
		void turn_y(float degrees);
	};

}

#endif
