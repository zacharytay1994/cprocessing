#include "Player.h"

#include <stdio.h>


PlayerData player1;

void Player_Initialize()
{
	// initialize player
	CP_Vector position = CP_Vector_Set(100.0f, 100.0f);
	player1 = (PlayerData) { 
		(CP_Vector) { position.x,position.y },
		PhyObj_AddAABox( position.x, position.y, PLAYER_MASS, PLAYER_WIDTH/2.0f, PLAYER_HEIGHT/2.0f, PLAYER_FRICTION )->super._id,
		Sprite_AddSprite( position, PLAYER_WIDTH/2.0f, PLAYER_HEIGHT/2.0f, "dirt_block.png", 1, 1, 1, 1),
		0
	};
}

void Player_Update(const float dt)
{
	player1._grounded = 0;
	Player_SyncPositionWithBoundingShape();

	// temp test
	PhyObjBoundingShape* shape = PhyObj_GetShape(player1._box);
	for (int i = 0; i < shape->_num_contacts; ++i) {
		CP_Vector contact_vector = CP_Vector_Normalize(CP_Vector_Subtract(shape->_contact_data[i], shape->_position));
		// dot with upward direction to see if contact is from the bottom
		float v_val = CP_Vector_DotProduct(contact_vector, CP_Vector_Set(0.0f, 1.0f));
		//float h_val = CP_Vector_DotProduct(contact_vector, CP_Vector_Set(1.0f, 0.0f));
		//int wall_connection = h_val < -0.8f || h_val > 0.8f;
		//int floor_connection = v_val > 0.68f;
		if (CP_Input_KeyDown(KEY_I)) {
			printf("%.2f\n", v_val);
		}
		if (shape->_grounded) {
			player1._grounded = 1;
		}
	}
	Player_Input(dt);
}

void Player_Render()
{
}

void Player_Input(const float dt)
{
	PhyObjBoundingShape* shape = PhyObj_GetShape(player1._box);
	// detect key press
	if (CP_Input_KeyDown(KEY_RIGHT)) {
		// check if horizontal velocity has not exceeded velocity limit
		if (shape->_velocity.x < PLAYER_MAX_HVELOCITY) {
			PhyObj_AddVelocity(shape, CP_Vector_Set(1000.0f * dt, 0.0f));
		}
	}
	if (CP_Input_KeyDown(KEY_LEFT)) {
		if (shape->_velocity.x > -PLAYER_MAX_HVELOCITY) {
			PhyObj_AddVelocity(shape, CP_Vector_Set(-1000.0f * dt, 0.0f));
		}
	}
	if (CP_Input_KeyDown(KEY_UP)) {
		if (player1._grounded && shape->_velocity.y > -PLAYER_MAX_VVELOCITY) {
			PhyObj_ApplyImpulse(shape, CP_Vector_Set(0.0f, -10000.0f));
		}
	}
}

void Player_SyncPositionWithBoundingShape()
{
	// sync sprite position with bounding box
	Sprite_SetPosition(player1._sprite, PhyObj_GetShape(player1._box)->_position);
}
