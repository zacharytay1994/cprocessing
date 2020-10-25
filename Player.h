#pragma once

#include "CProcessing/inc/cprocessing.h"
#include "PhyObj.h"
#include "Sprite.h"

#define PLAYER_MASS 30.0f
#define PLAYER_WIDTH 60.0f
#define PLAYER_HEIGHT 60.0f
#define PLAYER_FRICTION 0.03f

#define PLAYER_MAX_HVELOCITY 100.0f // maximum velocity after which the player will no longer be able to exert valocity onto itself
#define PLAYER_MAX_VVELOCITY 300.0f

typedef struct PlayerData {
	CP_Vector _position;
	int _box;
	int _sprite;
	int _grounded;
} PlayerData;

extern PlayerData player;

void Player_Initialize();
void Player_Update(const float dt);
void Player_Render();
void Player_Input(const float dt);
void Player_SyncPositionWithBoundingShape();