#pragma once

#include "CProcessing/inc/cprocessing.h"
#include "PhyObj.h"
#include "Sprite.h"

#define PLAYER_MAX_PLAYERS			2		// max number of players in scene

#define PLAYER1_MASS				10.0f	// lighter mass means less effect on other physics objects
#define PLAYER1_BOUNDING_WIDTH		45.0f	// width of bounding box
#define PLAYER1_BOUNDING_HEIGHT		110.0f	// height of bounding box
#define PLAYER1_SPRITE_WIDTH		120.0f	// width of sprite
#define PLAYER1_SPRITE_HEIGHT		120.0f	// height of sprite
#define PLAYER1_FRICTION			0.0f	// increase in friction prohibits vertical movement when touching a vertical surface

#define PLAYER_MAX_HVELOCITY		150.0f	// maximum velocity after which the player will no longer be able to exert velocity onto itself
#define PLAYER_MAX_VVELOCITY		600.0f	// maximum velocity vertical ...
#define PLAYER_APPLIED_HVELOCITY	3000.0f	// velocity applied for horizontal movement
#define PLAYER_APPLIED_VVELOCITY	6300.0f	// impulse applied for vertical movement

#define PLAYER_DECCELERATION		900.0f	// horizontal decceleration applied
#define PLAYER_DECCELERATION_THRES	20.0f

typedef enum PlayerAnimationState {
	Player_Anim_Idle,
	Player_Anim_Run,
	Player_Anim_Jump
} PlayerAnimationState;

typedef struct Player_Projectile {
	int _id;
	int _dead;
	CP_Vector _velocity;
} Player_Projectile;

typedef struct PlayerData {
	CP_Vector	_position;							// position: the position is synced to the box position
	int			_box;								// box: the bounding box handled by PhyObj
	int			_sprite_idle;						// the id to the idle spritesheet
	int			_sprite_run;						// the id to the run spritesheet
	int			_sprite_jump;						// the id to the jump spritesheet
	int			_grounded;							// boolean flagged when there is an upward force acting on the bounding box
	int			_airborne;							// boolean flagged when !grounded for a set amount of time
	float		_airborne_counter;					// amount of time passed before airborne flagged
	PlayerAnimationState _current_animation;		// current animation state, i.e. idle/run/jump
	int			_animation_flipped;					// boolean flagged for horizontal flip
	CP_KEY		_up;
	CP_KEY		_down;
	CP_KEY		_left;
	CP_KEY		_right;
} PlayerData;

void Player_Initialize();
void Player_AddPlayer(const CP_Vector pos, const float mass, const float bwidth, const float bheight, const float friction,
	const float swidth, const float sheight,
	const char* idle, const int icol, const int irow, const int iframes, const int ifps,
	const char* run, const int rcol, const int rrow, const int rframes, const int rfps,
	const char* jump, const int jcol, const int jrow, const int jframes, const int jfps,
	const CP_KEY up, const CP_KEY down, const CP_KEY left, const CP_KEY right);
void Player_Update(const float dt);
void Player_Render();
void Player_Input(const float dt);
void Player_SyncPositionWithBoundingShape();
void Player_SwitchAnimationState(const int id, PlayerAnimationState state);
void Player_FlipAnimationsH(const int id, const int flip);
void Player_Deccelerate(const float dt);
CP_Vector Player_GetPosition(const int id);
CP_Vector* Player_GetPosition_P(const int id);

void Player_SetCameraFocus(const int id);
void Player_temp();

// WEAPON CODE
void Player_ShowWeapon();
void Player_HideWeapon(const float dt);
void Player_WeaponUpdate(const float dt);

void Player_SpawnProjectile(const float dt);
void Player_ProjectileUpdate(const float dt);

void Player_RenderProjectileArc(const CP_Vector position, const CP_Vector direction, const int lineDensity, const float fallOff, const float dt);

// GAMEPLAY CODE
void Player_Add_Health(int x);
void Player_Lose_Health(int x);
void Player_Add_MaxHealth(int x);
void Player_Lose_MaxHealth(int x);

// POWERUP CODE
float speed_multiplier;
float powerup_timer[99];
int powerup_type[99];
void Player_Powerup_Update(const float dt);
void Player_Add_Powerup(int x, float duration);