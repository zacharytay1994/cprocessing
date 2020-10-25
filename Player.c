#include "Player.h"

#include <stdio.h>


PlayerData player1;

void Player_Initialize()
{
	// initialize player
	CP_Vector position = CP_Vector_Set(500, 50.0f);
	player1 = (PlayerData) { 
		(CP_Vector) { position.x,position.y },
		PhyObj_AddAABox( position.x, position.y, PLAYER_MASS, PLAYER_BOUNDING_WIDTH/2.0f, PLAYER_BOUNDING_HEIGHT/2.0f, PLAYER_FRICTION )->super._id,
		Sprite_AddSprite(position, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT, "player_idle.png", 3, 4, 10, 10),
		Sprite_AddSprite(position, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT, "player_run.png", 3, 3, 8, 15),
		Sprite_AddSprite(position, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT, "player_jump.png", 3, 4, 10, 30),
		0,
		0,
		Player_Anim_Idle,
		0
	};
	Sprite_SetVisible(player1._sprite_run, 0);
	Sprite_SetVisible(player1._sprite_jump, 0);
	Sprite_SetRepeat(player1._sprite_jump, 0);
}

void Player_Update(const float dt)
{
	player1._grounded = 0;
	Player_SyncPositionWithBoundingShape();
	// temp test
	PhyObjBoundingShape* shape = PhyObj_GetShape(player1._box);
	for (int i = 0; i < shape->_num_contacts; ++i) {
		// check if there is an upward contact on box
		player1._grounded = shape->_grounded;
	}
	if (!player1._grounded) {
		if ((player1._airborne_counter += dt) > 0.1f) {
			player1._airborne = 1;
		}
	}
	else {
		player1._airborne_counter = 0.0f;
		player1._airborne = 0;
	}
	Player_Input(dt);
}

void Player_Render()
{
}

void Player_Input(const float dt)
{
	PhyObjBoundingShape* shape = PhyObj_GetShape(player1._box);
	int some_input = 0;
	// detect key press
	if (player1._airborne) {
		Player_SwitchAnimationState(Player_Anim_Jump);
		some_input = 1;
	}
	if (CP_Input_KeyDown(KEY_RIGHT)) {
		// check if horizontal velocity has not exceeded velocity limit
		if (shape->_velocity.x < PLAYER_MAX_HVELOCITY) {
			PhyObj_AddVelocity(shape, CP_Vector_Set(PLAYER_APPLIED_HVELOCITY * dt, 0.0f));
		}
		// if not in air, switch animation
		if (player1._grounded) {
			Player_SwitchAnimationState(Player_Anim_Run);
			some_input = !some_input;
		}
		Player_FlipAnimationsH(0);
	}
	if (CP_Input_KeyDown(KEY_LEFT)) {
		if (shape->_velocity.x > -PLAYER_MAX_HVELOCITY) {
			PhyObj_AddVelocity(shape, CP_Vector_Set(-PLAYER_APPLIED_HVELOCITY * dt, 0.0f));
		}
		if (player1._grounded) {
			Player_SwitchAnimationState(Player_Anim_Run);
			some_input = !some_input;
		}
		Player_FlipAnimationsH(1);
	}
	if (CP_Input_KeyDown(KEY_UP)) {
		if (player1._grounded && shape->_velocity.y > -PLAYER_MAX_VVELOCITY) {
			PhyObj_ApplyImpulse(shape, CP_Vector_Set(0.0f, -PLAYER_APPLIED_VVELOCITY));
		}
	}
	if (!some_input) {
		Player_SwitchAnimationState(Player_Anim_Idle);
		Player_Deccelerate(dt);
	}
	if (CP_Input_KeyDown(KEY_L)) {
		PhyObj_SetVisible(player1._box, 1);
	}
	else {
		PhyObj_SetVisible(player1._box, 0);
	}
}

void Player_SyncPositionWithBoundingShape()
{
	player1._position = PhyObj_GetShape(player1._box)->_position;
	// sync sprite position with bounding box
	Sprite_SetPosition(player1._sprite_idle, player1._position);
	Sprite_SetPosition(player1._sprite_run, player1._position);
	Sprite_SetPosition(player1._sprite_jump, player1._position);
}

void Player_SwitchAnimationState(PlayerAnimationState state)
{
	if (player1._current_animation != state) {
		switch (player1._current_animation) {
		case Player_Anim_Idle:
			Sprite_SetVisible(player1._sprite_idle, 0);
			Sprite_Reset(player1._sprite_idle);
			break;
		case Player_Anim_Run:
			Sprite_SetVisible(player1._sprite_run, 0);
			Sprite_Reset(player1._sprite_run);
			break;
		case Player_Anim_Jump:
			Sprite_SetVisible(player1._sprite_jump, 0);
			Sprite_Reset(player1._sprite_jump);
			break;
		}
		switch (state) {
		case Player_Anim_Idle:
			Sprite_SetVisible(player1._sprite_idle, 1);
			break;
		case Player_Anim_Run:
			Sprite_SetVisible(player1._sprite_run, 1);
			break;
		case Player_Anim_Jump:
			Sprite_SetVisible(player1._sprite_jump, 1);
			break;
		}
		player1._current_animation = state;
	}
}

void Player_FlipAnimationsH(int flip)
{
	if (player1._animation_flipped != flip) {
		player1._animation_flipped = flip;
		Sprite_SetFlip(player1._sprite_idle, flip);
		Sprite_SetFlip(player1._sprite_run, flip);
		Sprite_SetFlip(player1._sprite_jump, flip);
	}
}

void Player_Deccelerate(const float dt)
{
	PhyObjBoundingShape* shape = PhyObj_GetShape(player1._box);
	// deccelerate player horizontally if above a certain threshold
	if (shape->_velocity.x > 10.0f) {
		PhyObj_AddVelocity(shape, CP_Vector_Set(-PLAYER_DECCELERATION * dt, 0.0f));
	}
	else if (shape->_velocity.x < -10.0f) {
		PhyObj_AddVelocity(shape, CP_Vector_Set(PLAYER_DECCELERATION * dt, 0.0f));
	}
	else {
		shape->_velocity.x = 0.0f;
	}
}
