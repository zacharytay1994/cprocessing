#pragma once
#include "Sprite.h"
#include <stdio.h>


typedef struct Enemy
{
	CP_Vector	position;
	CP_Vector	HPsprite_position;
	CP_Vector	enem_Size;
	CP_Vector	enem_HitboxScale;	// x = width of hitbox, y = height of hitbox
	//CP_Vector	enem_minHitbox;
	float		speed;
	float		health;
	int			ene_id;
	int			ene_sprite_id;
	int			enemyHP_spriteID;
	int			ene_Type;	// 0 - VitaminC, 1 - NoOxygen, 2 - Late4Class
	int			ene_dmg;
	const char* sprite_path;
	char		isAlive;
}Enemy;

// General "Makes-life-easy" stuff part 2 lol
float wind_Width;
float wind_Height;

// Enemy Stuff
Enemy enemy_list[127];

void CreateEnemy(float hp, CP_Vector position, CP_Vector size, float speed, int enemy_type);
int Add_Enem_toList(struct Enemy* add_enem);
void UpdateEnemy(const float dt);
// CheckEnemyCollision() takes in other obj hitbox and enemy id to check with. Returns 1 if hit, 0 if not
int CheckEnemyCollision(float maxPos_X, float maxPos_Y, float minPos_X, float minPos_Y, int enemy_id);
int CheckEnemyAlive(int id);
void SetEnemySpeed(int id, float newSpeed);
void SetEnemyHP(int id, float newHP);
float GetEnemyHP(int id);
int GetEnemyDMG(int id);
void SetEnemyDie(int id);

void Enemy_Initialize();

