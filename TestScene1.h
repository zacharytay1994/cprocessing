#pragma once
#include "Button.h"
#include "Sprite.h"

struct Button btn_popupWind;
struct Button btn_closePopup;

struct Button btn_test1;
struct Button btn_test2;
struct Button btn_test3;

typedef struct Enemy
{
	CP_Vector	position;
	CP_Vector	enem_Size;
	float		speed;
	int			health;
	int			ene_id;
	int			ene_sprite_id;
	int			ene_Type;	// 0 - VitaminC, 1 - NoOxygen, 2 - Late4Class
	const char* sprite_path;
	char		isAlive;
}Enemy;

// Enemy Stuff
Enemy enemy_list[127];

void CreateEnemy(int hp, CP_Vector position, CP_Vector size, float speed, 
	int enemy_type);
int Add_Enem_toList(struct Enemy* add_enem);
void UpdateEnemy(const float dt);

// GUI Buttons stuff
int mainGUI_isOpen;	// 0 - isClosed, 1 - isOpened
void TestScene1_BtnInit();
void TestScene1_BtnManager();
void GUIRender();


// Scene Stuff
void TestScene1_Init();
void TestScene1_Update(const float dt);
void TestScene1_Exit();