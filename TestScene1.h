#pragma once
#include "Button.h"
#include "Sprite.h"
#include "Camera.h"
#include "Enemy.h"
#include "Inventory.h"

int btn_popupWind;
int btn_closePopup;

int btn_test1;
int btn_test2;
int btn_test3;



// General "Makes-life-easy" stuff
float wind_Width;
float wind_Height;
float house_posX;
float house_posY;
float house_SizeX;
float house_SizeY;
int tempHouseSprite_id;
int tempHouseHP_spriteId;
float houseHP;
void KeyInputAssign();	//Checks for keyboard input 


// GUI Buttons stuff
int mainGUI_isOpen;	// 0 - isClosed, 1 - isOpened
void TestScene1_BtnInit();
void TestScene1_BtnManager();
void GUIRender();

// Scene Stuff
void TestScene1_Init();
void TestScene1_Update(const float dt);
void TestScene1_Exit();