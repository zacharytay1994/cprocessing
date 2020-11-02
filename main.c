//---------------------------------------------------------
// file:	main.c
// author:	[NAME]
// email:	[DIGIPEN EMAIL ADDRESS]
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright � 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "cprocessing.h"

#include "Scene.h"
#include "TestScene1.h"
#include "TestScene2.h"
#include "TestBed.h"
#include "MainMenu.h"

#include <stdio.h>
#include <stdlib.h>


int test_scene_1;
int test_scene_2;
int test_bed;
int main_menu;

//Button Shortcut Test
struct tempBtn {
	float b_widthPos;
	float b_heightPos;
	float b_widthSize;
	float b_heightSize;
}tempBtn;

struct tempBtn MainBtnArr[2];
/*
* 1 - Main popup Btn
* 2 - Close main popup
*/

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	CP_System_SetWindowSize(1600, 900);
	CP_Settings_AntiAlias(0);



	//Button Shortcut init
	/*MainBtnArr[0].b_widthPos = (float)(CP_System_GetWindowWidth() / 1.1f);
	MainBtnArr[0].b_heightPos = (float)(CP_System_GetWindowHeight() / 9);
	MainBtnArr[0].b_widthSize = (float)(CP_System_GetWindowWidth() / 9);
	MainBtnArr[0].b_heightSize = (float)(CP_System_GetWindowHeight() / 11);
	Sprite_AddSprite((CP_Vector) { MainBtnArr[0].b_widthPos, MainBtnArr[0].b_heightPos},
		MainBtnArr[0].b_widthSize, MainBtnArr[0].b_heightSize, "Assets/demo_mShortcut.png", 1, 1, 1, 1);

	MainBtnArr[1].b_widthPos = (float)(CP_System_GetWindowWidth() / 1.25f);
	MainBtnArr[1].b_heightPos = (float)(CP_System_GetWindowHeight() / 2);
	MainBtnArr[1].b_widthSize = (float)(CP_System_GetWindowWidth() / 9);
	MainBtnArr[1].b_heightSize = (float)(CP_System_GetWindowHeight() / 1.2);*/

	
	// system settings
	CP_System_ShowConsole();
	
	Scene_Initialize();
	test_scene_1 = Scene_AddScene(TestScene1_Init,TestScene1_Update,TestScene1_Exit);
	test_scene_2 = Scene_AddScene(TestScene2_Init, TestScene2_Update, TestScene2_Exit);
	test_bed = Scene_AddScene(TestBed_Init, TestBed_Update, TestBed_Exit);
	main_menu = Scene_AddScene(MainMenu_Initialize, MainMenu_Update, MainMenu_Exit);
	Scene_ChangeScene(main_menu);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{

	// check input, update simulation, render etc.
	CP_Settings_Background(CP_Color_Create(100, 100, 100, 255));


	//DEBUG MOUSE POSITION DISPLAY
	char mouseX[100];
	char mouseY[100];
	//char tempDebugX[100];
	//char tempDebugY[100];
	CP_Settings_TextSize(30.f);
	sprintf_s(mouseX, 100, "Mouse X Pos: %.2f", CP_Input_GetMouseX());
	CP_Font_DrawText(mouseX, (float)(CP_System_GetWindowWidth() / 1.5), (float)(CP_System_GetWindowHeight() / 5));
	sprintf_s(mouseY, 100, "Mouse Y Pos: %.2f", CP_Input_GetMouseY());
	CP_Font_DrawText(mouseY, (float)(CP_System_GetWindowWidth() / 1.5), (float)(CP_System_GetWindowHeight() / 6));
	//sprintf_s(tempDebugX, 100, "btnWPos : %.2f", bm_widthPos);
	//CP_Font_DrawText(tempDebugX, (float)(CP_System_GetWindowWidth() / 1.5), (float)(CP_System_GetWindowHeight() / 7));
	//sprintf_s(tempDebugY, 100, "btnWSize : %.2f", bm_widthSize);
	//CP_Font_DrawText(tempDebugY, (float)(CP_System_GetWindowWidth() / 1.5), (float)(CP_System_GetWindowHeight() / 8));

	

	Scene_Update();


}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
	Scene_Exit();
}

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
