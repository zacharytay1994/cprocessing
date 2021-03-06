//---------------------------------------------------------
// file:	Enemy.c
// author:	Ray
// email:	z.tay@digipen.edu
//
// brief:	Camera functions mainly to move the camera
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "Enemy.h"
#include "LePlant.h"
#include "Particles.h"
#include <stdio.h>


int not_init = 1;
int enemy_one;
int enemy_two;
int enemy_three;
int enemy_four;
int enemy_five;
int enemyhp_Sprite;

CP_Sound Enemy_death_sound;

//float HP_maxSpriteSize;

// Enemy Stuff - 
void Enemy_Initialize()
{
	enemy_one = Sprite_AddSprite(
		(CP_Vector) {-100,-100},
		1.f, 1.f,
		"demo_enemy.png",
		1,1,1,1, 1);

	enemy_two = Sprite_AddSprite(
		(CP_Vector) {-100,-100},
		1.f,1.f,
		"demo_enemy2.png",
		1,1,1,1, 1);

	enemy_three = Sprite_AddSprite(
		(CP_Vector) {-100,-100},
		1.f,1.f,
		"demo_player.png",
		8,1,8,50, 1);

	enemy_four = Sprite_AddSprite(
		(CP_Vector) { -100, -100 }, 
		200.0f, 200.0f, 
		"./Sprites/slime1.png",
		2, 3, 6, 5, 0);

	enemy_five = Sprite_AddSprite(
		(CP_Vector) { -100, -100 }, 
		200.0f, 200.0f, 
		"./Sprites/slime2.png",
		2, 3, 6, 5, 0);

	enemyhp_Sprite = Sprite_AddSprite(
		(CP_Vector) {-100,-100},
		100.f,30.f,
		"half_redBox.png",
		1,1,1,1,1);
	
	for (int i = 0; i < 127; ++i) {
		enemy_list[i]._initialized = 0;
	}
	Enemy_death_sound = CP_Sound_Load("./Assets/Notes/a.wav");
}

void CreateEnemy(float hp, CP_Vector position, CP_Vector size, float speed, int enemy_type)
{
	struct Enemy new_enemy;

	// Enemy Type Dependant variables
	//char* path;
	int path_id;
	int enem_sprite_col = 1;
	int enem_sprite_row = 1;
	int enem_sprite_frames = 1;
	int enem_sprite_animate_speed = 1;


	// Basic Enemy Variables
	new_enemy.health = hp;
	new_enemy.maxHealth = new_enemy.health;
	new_enemy.position = position;
	new_enemy.speed = speed;
	new_enemy.enem_Size = size;
	new_enemy.ene_Type = enemy_type;
	new_enemy.money_amount = 0;

	switch (enemy_type)
	{
	case 0:	// VitaminC
	{
		path_id = enemy_one;
		new_enemy.ene_dmg = 1;
		
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,50.0f };
		new_enemy.HPsprite_position = (CP_Vector){ new_enemy.position.x, new_enemy.position.y - 30.f };
		break;
	}
	case 1:	// NoOxygen
	{
		path_id = enemy_two;
		new_enemy.ene_dmg = 2;
		new_enemy.enem_Size.x = size.x * 2.5f;
		new_enemy.enem_Size.y = size.y * 2.5f;
		new_enemy.HPsprite_position = (CP_Vector){ new_enemy.position.x, new_enemy.position.y - 50.f };
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,50.0f };

		break;
	}
	case 2:	// Late4Class
	{
		path_id = enemy_three;
		new_enemy.ene_dmg = 3;
		new_enemy.enem_Size.x = size.x * 1.2f;
		new_enemy.enem_Size.y = size.y * 1.2f;
		enem_sprite_col = 8;
		enem_sprite_row = 1;
		enem_sprite_frames = 8;
		enem_sprite_animate_speed = 50;
		new_enemy.HPsprite_position = (CP_Vector){ new_enemy.position.x, new_enemy.position.y - 50.f };
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,50.0f };

		break;
	}
	case 3:	// toothpaste
	{
		path_id = enemy_four;
		new_enemy.ene_dmg = 1;
		new_enemy.enem_Size.x = -size.x;
		new_enemy.enem_Size.y = size.y;
		new_enemy.money_amount = 10;
		enem_sprite_col = 2;
		enem_sprite_row = 3;
		enem_sprite_frames = 6;
		enem_sprite_animate_speed = 5;
		new_enemy.HPsprite_position = (CP_Vector){ new_enemy.position.x, new_enemy.position.y - 50.f };
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,50.0f };

		break;
	}

	case 4:	// strawberry jam
	{
		path_id = enemy_five;
		new_enemy.ene_dmg = 2;
		new_enemy.enem_Size.x = -size.x;
		new_enemy.enem_Size.y = size.y;
		new_enemy.money_amount = 50;
		enem_sprite_col = 2;
		enem_sprite_row = 3;
		enem_sprite_frames = 6;
		enem_sprite_animate_speed = 3;
		new_enemy.HPsprite_position = (CP_Vector){ new_enemy.position.x, new_enemy.position.y - 50.f };
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };

		break;
	}

	case 5:	// COLD-GATE
	{
		path_id = enemy_four;
		new_enemy.ene_dmg = 3;
		new_enemy.enem_Size.x = -size.x * 7;
		new_enemy.enem_Size.y = size.y * 7;
		new_enemy.position.y -= 400;
		new_enemy.money_amount = 1000;
		enem_sprite_col = 2;
		enem_sprite_row = 3;
		enem_sprite_frames = 2;
		enem_sprite_animate_speed = 2;
		new_enemy.HPsprite_position = (CP_Vector){ new_enemy.position.x, new_enemy.position.y - 50.f };
		new_enemy.enem_HitboxScale = (CP_Vector){ 100,5000 };

		break;
	}
	default:	// jus some random fall thru in case something happens
	{
		path_id = enemy_one;
		enem_sprite_col = 1;
		enem_sprite_row = 1;
		enem_sprite_frames = 1;
		enem_sprite_animate_speed = 1;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,50.0f };
		new_enemy.HPsprite_position = (CP_Vector){ new_enemy.position.x, new_enemy.position.y - 30.f };
		break;
	}
	}


	// Assign Enemy Sprite
	new_enemy.ene_sprite_id = Sprite_AddSpriteRepeatManual(
		new_enemy.position,
		-(new_enemy.enem_Size.x),
		new_enemy.enem_Size.y,
		path_id,
		enem_sprite_col,
		enem_sprite_row,
		enem_sprite_frames,
		enem_sprite_animate_speed, 1);

	new_enemy.enemyHP_spriteID = Sprite_AddSpriteRepeatManual(
		new_enemy.HPsprite_position,
		(new_enemy.health / new_enemy.maxHealth) * 200.f, 10.f,
		enemyhp_Sprite,
		1, 1, 1, 1, 1);
	

	new_enemy.isAlive = 1;	//1 - alive, 0 - dead
	//printf("NEWeneScaleX: %2f, NEWeneScaleY: %2f\n", (new_enemy.enem_HitboxScale.x), (new_enemy.enem_HitboxScale.y));
	// Add enemy to list
	new_enemy._initialized = 1;
	new_enemy.ene_id = Add_Enem_toList(&new_enemy);
	//Debug
	//printf("Enemy created at x: %f, y: %f\n", new_enemy.position.x, new_enemy.position.y);
}

int Add_Enem_toList(struct Enemy* add_enem)
{
	for (int i = 0; i < ENEMY_MAX_ENEMIES; i++)
	{
		if (enemy_list[i].isAlive == 0)
		{
			enemy_list[i] = *add_enem;
			enemy_list[i].ene_id = i;
			// Debug
			//printf("Enemy added\n");
			return i;
		}
	}
	return 0;
}

void UpdateEnemy(const float dt)
{
	for (int i = 0; i < ENEMY_MAX_ENEMIES; ++i)
	{
		if (enemy_list[i]._initialized) {
			if (enemy_list[i].isAlive == 0)
			{
				// if Enemy is "dead", set sprite visible off
				Sprite_SetVisible(enemy_list[i].ene_sprite_id, 0);
				Sprite_SetVisible(enemy_list[i].enemyHP_spriteID, 0);
				continue;
			}
			else
			{
				Sprite_RenderSprite(dt, enemy_list[i].ene_sprite_id);
				Sprite_RenderSprite(dt, enemy_list[i].enemyHP_spriteID);
				//printf("Enemy id: %d, pos: %f, %f\n", i, enemy_list[i].position.x, enemy_list[i].position.y);
				//Update Movement/sprite movement 
				enemy_list[i].position.x -= dt * (enemy_list[i].speed);
				Sprite_SetPosition(enemy_list[i].ene_sprite_id, enemy_list[i].position);
				Sprite_SetPosition(enemy_list[i].enemyHP_spriteID, (CP_Vector) { enemy_list[i].position.x - 50.f, enemy_list[i].HPsprite_position.y });

				//Only render alive enemies
				Sprite_RenderSprite(dt, enemy_list[i].ene_sprite_id);
				Sprite_RenderSprite(dt, enemy_list[i].enemyHP_spriteID);

				//if out of map (left boundaries only, not like enemy gonna move right...right?)
				if (CheckEnemyCollision(0.f, wind_Height, -10.f, 0.f, i) == 1)
				{
					enemy_list[i].isAlive = 0;
				}

			}
		}
	}
}

int CheckEnemyCollision(float maxPos_X, float maxPos_Y, float minPos_X, float minPos_Y, int enemy_id)
{
	// Get enemy max hitbox boundery
	float maxhitBox_posX = enemy_list[enemy_id].position.x + (enemy_list[enemy_id].enem_HitboxScale.x / 2.f);
	float minhitBox_posX = enemy_list[enemy_id].position.x - (enemy_list[enemy_id].enem_HitboxScale.x / 2.f);
	// Get enemy min hitbox boundary
	float maxhitBox_posY = enemy_list[enemy_id].position.y + (enemy_list[enemy_id].enem_HitboxScale.y / 2.f);
	float minhitBox_posY = enemy_list[enemy_id].position.y - (enemy_list[enemy_id].enem_HitboxScale.y / 2.f);

	// Compares objbox with enemy hitbox
	if (!(maxPos_X < minhitBox_posX || maxPos_Y < minhitBox_posY || minPos_X > maxhitBox_posX || minPos_Y > maxhitBox_posY)) {
		return 1;
	}

	return 0;
	/*if (maxPos_X >= minhitBox_posX &&
		maxPos_Y >= minhitBox_posY)
	{
		return 1;
	}
	else if (minPos_X <= maxhitBox_posX &&
		minPos_Y >= maxhitBox_posY)
	{
		return 1;
	}
	else
	{
		return 0;
	}*/
}

int CheckEnemyAlive(int id)
{
	if (enemy_list[id].isAlive == 1)
		return 1;
	else
		return 0;
}

void SetEnemySpeed(int id, float newSpeed)
{
	enemy_list[id].speed = newSpeed;
}

void SetEnemyHP(int id, float newHP)
{
	enemy_list[id].health = newHP;
	Sprite_SetWidth(enemy_list[id].enemyHP_spriteID, (enemy_list[id].health / enemy_list[id].maxHealth) * 200.f);

	if (enemy_list[id].health <= 0)
	{
		SetEnemyDie(enemy_list[id].ene_id);
		LePlant_AddBean(enemy_list[id].position);
		Particle_EmitOut(PT_Droplets, enemy_list[id].position, 200.0f, 300.0f,
			-60.0f, -60.0f, 250.0f, -250.0f,
			1.8f, 1.3f, -50.0f, -80.0f, 0.04f,
			0.02f, 120.0f, 10, 15, 1);
	}
}

float GetEnemyHP(int id)
{
	float currHP;
	currHP = enemy_list[id].health;
	return currHP;
}

int GetEnemyDMG(int id)
{
	int dmg = enemy_list[id].ene_dmg;
	return dmg;
}

int GetEnemyMoney(int id)
{
	int cashCough = enemy_list[id].money_amount;
	return cashCough;
}

void SetEnemyDie(int id)
{
	enemy_list[id].isAlive = 0;
	CP_Sound_Play(Enemy_death_sound);
}

void EnemyTakeDamage(int id, int dmg)
{
	SetEnemyHP(id, GetEnemyHP(id) - dmg);
}

// End of Enemy Stuff