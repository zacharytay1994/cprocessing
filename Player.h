#pragma once

#include "CProcessing/inc/cprocessing.h"
#include "PhyObj.h"
#include "Sprite.h"

#define PLAYER_MASS 10.0f
#define PLAYER_BOUNDING_WIDTH 45.0f
#define PLAYER_BOUNDING_HEIGHT 110.0f
#define PLAYER_SPRITE_WIDTH 120.0f
#define PLAYER_SPRITE_HEIGHT 120.0f
#define PLAYER_FRICTION 0.0f

#define PLAYER_MAX_HVELOCITY 150.0f // maximum velocity after which the player will no longer be able to exert valocity onto itself
#define PLAYER_MAX_VVELOCITY 300.0f
#define PLAYER_APPLIED_HVELOCITY 3000.0f
#define PLAYER_APPLIED_VVELOCITY 3300.0f

#define PLAYER_DECCELERATION 900.0f

typedef enum PlayerAnimationState {
	Player_Anim_Idle,
	Player_Anim_Run,
	Player_Anim_Jump
} PlayerAnimationState;

typedef struct PlayerData {
	CP_Vector _position;
	int _box;
	int _sprite_idle;
	int _sprite_run;
	int _sprite_jump;
	int _grounded;
	int _airborne;
	float _airborne_counter;
	PlayerAnimationState _current_animation;
	int _animation_flipped;
} PlayerData;

extern PlayerData player;

void Player_Initialize();
void Player_Update(const float dt);
void Player_Render();
void Player_Input(const float dt);
void Player_SyncPositionWithBoundingShape();
void Player_SwitchAnimationState(PlayerAnimationState state);
void Player_FlipAnimationsH(int flip);
void Player_Deccelerate(const float dt);