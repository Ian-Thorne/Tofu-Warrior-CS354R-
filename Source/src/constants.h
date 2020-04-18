#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Enumeration Area: */

enum PlayerState {
    IDLE = 0,
    WALKING = 1,
    JUMPING = 2,
    FALLING = 3,
    SLIDING = 4,
    HANGING = 5
};

enum EnemyState {
	PATROL = 0,
	GRID_PURSUE = 1,
	PLAYER_PURSUE = 2,
	FLEE = 3
};

enum FamiliarState {
    F_PATROL = 0,
    F_GRID_PURSUE = 1,
    F_FREE_PURSUE = 3,
    F_COLLECT_COIN = 4
};

/* Constant Area: */

//Game-Wide constants:
const int SINGLE_PLAYER_WINNING_GOLD = 100;
const int MULTIPLAYER_WINNING_GOLD = 51;
const int LOSING_HP = 0;
const float DEFAULT_GRAVITY = -0.5;

//Player constants:
const int DEFAULT_PLAYER_HP = 10;
const int DEFAULT_PLAYER_SPEED = 6;
const int DEFAULT_PLAYER_ACCELERATION = 3;
const int DEFAULT_PLAYER_DECELERATION = 5;
const int DEFAULT_PLAYER_JUMP_HEIGHT = 10;
const int DEFAULT_PLAYER_MAX_EXTRA_JUMPS = 1;
const int DEFAULT_PLAYER_FALL_LIMIT = -8;
const int DEFAULT_PLAYER_WALKABLE_ANGLE = 45;
const int DEFAULT_GLIDER_FACTOR = 2;
const int DEFAULT_GLIDER_FALL_LIMIT = -2;
const int DEFAULT_JUMP_COOLDOWN = 20;
const int DEFAULT_ATTACK_COOLDOWN = 30;
const float DEFAULT_ATTACK_RANGE = 0.8;
const float DEFAULT_ATTACK_RANGE_POSITION_Z = -1;
const float DEFAULT_ATTACK_RANGE_POSITION_Z_MODIFIER = 0.8;
const int DEFAULT_SOUND_CONTROL_COOLDOWN = 60;
const int DEFAULT_WAYPOINT_CONTROL_COOLDOWN = 60;
const int DEFAULT_DAMAGE_TAKEN_COOLDOWN = 90;
const float DEFAULT_PLAYER_AIR_CONTROL = 1;

//Enemy constants:
const int DEFAULT_ENEMY_HP = 3;
const int DEFAULT_ENEMY_DAMAGE_DEALT = 2;
const int DEFAULT_ENEMY_GOLD_VALUE = 5;
const int ENEMY_DEFAULT_DAMAGE_TAKEN_COOLDOWN = 15;
const float DEFAULT_ENEMY_SPEED = 3.15;
const float CLOSE_DISTANCE = 7.08;
const float MEDIUM_DISTANCE = 17.5;
const float ENEMY_MAX_CORNER_DISTANCE = 15;
const float LOW_HP_RANGE = 0.34;

//Familiar constants:
const int FAMILIAR_FOLLOW_DISTANCE = 5;
const int FAMILIAR_COIN_SEARCH_DISTANCE = 10;
const int FAMILIAR_MOVEMENT_SPEED = 4;
const int FAMILIAR_CHASE_DISTANCE = 2;

//Shop constants:
const int DEFAULT_SHOP_TIME_LIMIT = 60;
const int WEAPON_SHOP = 0;
const int ARMOR_SHOP = 1;
const int BOOT_SHOP = 2;
const int MISC_SHOP = 3;
const int HEALTH_POTION_COST = 10;
const int HEALTH_POTION_HP_RESTORE = 5;

//Item constants:
const int GLIDER = 0;
const int SWORD = 1;
const int FAMILIAR_BELL = 2;

//Audio constants:
const int BUS_SOUND_EFFECTS = 1;
const int BUS_BACKGROUND_MUSIC = 2;
const int MIN_VOLUME = -12;
const int MAX_VOLUME = 42;

#endif
