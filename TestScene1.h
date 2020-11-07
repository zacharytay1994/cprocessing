#pragma once
#include "Button.h"
#include "Sprite.h"
#include "Camera.h"

struct Button btn_popupWind;
struct Button btn_closePopup;

struct Button btn_test1;
struct Button btn_test2;
struct Button btn_test3;

typedef struct Enemy
{
	CP_Vector	position;
	CP_Vector	enem_Size;
	CP_Vector	enem_HitboxScale;	// x = width of hitbox, y = height of hitbox
	//CP_Vector	enem_minHitbox;
	float		speed;
	int			health;
	int			ene_id;
	int			ene_sprite_id;
	int			ene_Type;	// 0 - VitaminC, 1 - NoOxygen, 2 - Late4Class
	const char* sprite_path;
	char		isAlive;
}Enemy;

// General "Makes-life-easy" stuff
float wind_Width;
float wind_Height;

// Enemy Stuff
Enemy enemy_list[127];
void CreateEnemy(int hp, CP_Vector position, CP_Vector size, float speed, 
	int enemy_type);
int Add_Enem_toList(struct Enemy* add_enem);
void UpdateEnemy(const float dt);
// CheckEnemyCollision() takes in other obj hitbox and enemy id to check with. Returns 1 if hit, 0 if not
int CheckEnemyCollision(float maxPos_X, float maxPos_Y, float minPos_X, float minPos_Y, int enemy_id);

// GUI Buttons stuff
int mainGUI_isOpen;	// 0 - isClosed, 1 - isOpened
void TestScene1_BtnInit();
void TestScene1_BtnManager();
void GUIRender();


// Scene Stuff
void TestScene1_Init();
void TestScene1_Update(const float dt);
void TestScene1_Exit();