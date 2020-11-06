#include "Player.h"

#include <stdio.h>

PlayerData Player_players[PLAYER_MAX_PLAYERS] = { 0 };
int Player_players_size = 0;

void Player_Initialize()
{
	// initialize player at the center of the screen
	CP_Vector position = CP_Vector_Set(200.0f, 20.0f);

	// Adding a player into the scene
	Player_AddPlayer(position,
		PLAYER1_MASS, PLAYER1_BOUNDING_WIDTH, PLAYER1_BOUNDING_HEIGHT, PLAYER1_FRICTION, PLAYER1_SPRITE_WIDTH, PLAYER1_SPRITE_HEIGHT,
		"player_idle.png", 3, 4, 10, 10,
		"player_run.png", 3, 3, 8, 15,
		"player_jump.png", 3, 4, 10, 30,
		KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT);

	// Adding player 2 into the scene
	/*Player_AddPlayer(position, 10.0f, 22.5f, 55.0f, 0.0f, 60.0f, 60.0f,
		"player_idle.png", 3, 4, 10, 10,
		"player_run.png", 3, 3, 8, 15,
		"player_jump.png", 3, 4, 10, 30,
		KEY_W,KEY_S,KEY_A,KEY_D);*/
}

void Player_AddPlayer(const CP_Vector pos, const float mass, const float bwidth, const float bheight, const float friction,
	const float swidth, const float sheight, 
	const char* idle, const int icol, const int irow, const int iframes, const int ifps,
	const char* run, const int rcol, const int rrow, const int rframes, const int rfps, 
	const char* jump, const int jcol, const int jrow, const int jframes, const int jfps,
	const CP_KEY up, const CP_KEY down, const CP_KEY left, const CP_KEY right)
{
	if (Player_players_size < PLAYER_MAX_PLAYERS) {
			Player_players[Player_players_size] = (PlayerData) { 
			(CP_Vector) { pos.x,pos.y },
			PhyObj_AddAABox( pos.x, pos.y, mass, bwidth/2.0f, bheight/2.0f, friction )->super._id,
			Sprite_AddSprite(pos, swidth, sheight, idle, icol, irow, iframes, ifps),
			Sprite_AddSprite(pos, swidth, sheight, run, rcol, rrow, rframes, rfps),
			Sprite_AddSprite(pos, swidth, sheight, jump, jcol, jrow, jframes, jfps),
			0,
			0,
			0.0f,
			Player_Anim_Idle,
			0,
			up,
			down,
			left,
			right
		};
		Sprite_SetVisible(Player_players[Player_players_size]._sprite_run, 0);
		Sprite_SetVisible(Player_players[Player_players_size]._sprite_jump, 0);
		Sprite_SetRepeat(Player_players[Player_players_size]._sprite_jump, 0);
		++Player_players_size;
	}
}

void Player_Update(const float dt)
{
	for (int i = 0; i < Player_players_size; ++i) {
		Player_players[i]._grounded = 0;
		Player_SyncPositionWithBoundingShape();
		PhyObjBoundingShape* shape = PhyObj_GetShape(Player_players[i]._box);
		Player_players[i]._grounded = shape->_grounded;
		// flag airborne is grounded more then set time
		if (!Player_players[i]._grounded) {
			if ((Player_players[i]._airborne_counter += dt) > 0.1f) {
				Player_players[i]._airborne = 1;
			}
		}
		else {
			Player_players[i]._airborne_counter = 0.0f;
			Player_players[i]._airborne = 0;
		}
	}
	Player_Input(dt);
}

void Player_Render()
{
	// nothing to render yet as sprite.c handles rendering of sprites,
	// even player sprites
}

void Player_Input(const float dt)
{
	for (int i = 0; i < Player_players_size; ++i) {
		PhyObjBoundingShape* shape = PhyObj_GetShape(Player_players[i]._box);
		if (CP_Input_KeyDown(KEY_O)) {
			shape->_position = CP_Vector_Set(200.0f, 20.0f);
		}
		int some_input = 0;
		// detect key press
		if (Player_players[i]._airborne) {
			Player_SwitchAnimationState(i,Player_Anim_Jump);
			some_input = 1;
		}
		if (CP_Input_KeyDown(Player_players[i]._right)) {
			// check if horizontal velocity has not exceeded velocity limit
			if (shape->_velocity.x < PLAYER_MAX_HVELOCITY) {
				PhyObj_AddVelocity(shape, CP_Vector_Set(PLAYER_APPLIED_HVELOCITY * dt, 0.0f));
			}
			// if not in air, switch animation
			if (Player_players[i]._grounded) {
				Player_SwitchAnimationState(i,Player_Anim_Run);
				some_input = !some_input;
			}
			Player_FlipAnimationsH(i,0);
		}
		if (CP_Input_KeyDown(Player_players[i]._left)) {
			if (shape->_velocity.x > -PLAYER_MAX_HVELOCITY) {
				PhyObj_AddVelocity(shape, CP_Vector_Set(-PLAYER_APPLIED_HVELOCITY * dt, 0.0f));
			}
			if (Player_players[i]._grounded) {
				Player_SwitchAnimationState(i,Player_Anim_Run);
				some_input = !some_input;
			}
			Player_FlipAnimationsH(i,1);
		}
		if (CP_Input_KeyDown(Player_players[i]._up)) {
			if (Player_players[i]._grounded && shape->_velocity.y > -PLAYER_MAX_VVELOCITY) {
				PhyObj_ApplyImpulse(shape, CP_Vector_Set(0.0f, -PLAYER_APPLIED_VVELOCITY));
			}
		}
		if (!some_input) {
			Player_SwitchAnimationState(i,Player_Anim_Idle);
			Player_Deccelerate(dt);
		}
		if (CP_Input_KeyDown(KEY_L)) {
			PhyObj_SetVisible(Player_players[i]._box, 1);
		}
		else {
			PhyObj_SetVisible(Player_players[i]._box, 0);
		}
	}
}

void Player_SyncPositionWithBoundingShape()
{
	for (int i = 0; i < Player_players_size; ++i) {
		Player_players[i]._position = PhyObj_GetShape(Player_players[i]._box)->_position;
		// sync sprite position with bounding box
		Sprite_SetPosition(Player_players[i]._sprite_idle, Player_players[i]._position);
		Sprite_SetPosition(Player_players[i]._sprite_run, Player_players[i]._position);
		Sprite_SetPosition(Player_players[i]._sprite_jump, Player_players[i]._position);
	}
}

void Player_SwitchAnimationState(const int id, PlayerAnimationState state)
{
	if (id < Player_players_size) {
		if (Player_players[id]._current_animation != state) {
			switch (Player_players[id]._current_animation) {
			case Player_Anim_Idle:
				Sprite_SetVisible(Player_players[id]._sprite_idle, 0);
				Sprite_Reset(Player_players[id]._sprite_idle);
				break;
			case Player_Anim_Run:
				Sprite_SetVisible(Player_players[id]._sprite_run, 0);
				Sprite_Reset(Player_players[id]._sprite_run);
				break;
			case Player_Anim_Jump:
				Sprite_SetVisible(Player_players[id]._sprite_jump, 0);
				Sprite_Reset(Player_players[id]._sprite_jump);
				break;
			}
			switch (state) {
			case Player_Anim_Idle:
				Sprite_SetVisible(Player_players[id]._sprite_idle, 1);
				break;
			case Player_Anim_Run:
				Sprite_SetVisible(Player_players[id]._sprite_run, 1);
				break;
			case Player_Anim_Jump:
				Sprite_SetVisible(Player_players[id]._sprite_jump, 1);
				break;
			}
			Player_players[id]._current_animation = state;
		}
	}
}

void Player_FlipAnimationsH(const int id, const int flip)
{
	if (id < Player_players_size) {
		if (Player_players[id]._animation_flipped != flip) {
			Player_players[id]._animation_flipped = flip;
			Sprite_SetFlip(Player_players[id]._sprite_idle, flip);
			Sprite_SetFlip(Player_players[id]._sprite_run, flip);
			Sprite_SetFlip(Player_players[id]._sprite_jump, flip);
		}
	}
}

void Player_Deccelerate(const float dt)
{
	for (int i = 0; i < Player_players_size; ++i) {
		PhyObjBoundingShape* shape = PhyObj_GetShape(Player_players[i]._box);
		// deccelerate player horizontally if above a certain threshold
		if (shape->_velocity.x > PLAYER_DECCELERATION_THRES) {
			PhyObj_AddVelocity(shape, CP_Vector_Set(-PLAYER_DECCELERATION * dt, 0.0f));
		}
		else if (shape->_velocity.x < -PLAYER_DECCELERATION_THRES) {
			PhyObj_AddVelocity(shape, CP_Vector_Set(PLAYER_DECCELERATION * dt, 0.0f));
		}
		else {
			shape->_velocity.x = 0.0f;
		}
	}
}
