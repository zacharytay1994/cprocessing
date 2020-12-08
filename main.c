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
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "cprocessing.h"

#include "Scene.h"
#include "TestScene1.h"
#include "TestScene2.h"
#include "TestScene3.h"
#include "TestBed.h"
#include "MainMenu.h"
#include "SplashScreen.h"
#include "Credits.h"
#include "HowToPlay.h"

#include <stdio.h>
#include <stdlib.h>

int splash_screen;
int test_scene_1;
int test_scene_2;
int test_bed;
int main_menu;
int test_scene_3;
int credits_scene;
int howtoplay_scene;

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	CP_System_SetWindowSize(1600, 900);
	CP_Settings_AntiAlias(0);
	
	// system settings
	
	Scene_Initialize();
	Button_Class_Init();
	test_scene_1 = Scene_AddScene(TestScene1_Init,TestScene1_Update,TestScene1_Exit);
	test_scene_2 = Scene_AddScene(TestScene2_Init, TestScene2_Update, TestScene2_Exit);
	test_bed = Scene_AddScene(TestBed_Init, TestBed_Update, TestBed_Exit);
	main_menu = Scene_AddScene(MainMenu_Initialize, MainMenu_Update, MainMenu_Exit);
	splash_screen = Scene_AddScene(SplashScreen_Init, SplashScreen_Update, SplashScreen_Exit);
	test_scene_3 = Scene_AddScene(TestScene3_Init, TestScene3_Update, TestScene3_Exit);
	credits_scene = Scene_AddScene(Credits_Init, Credits_Update, Credits_Exit);
	howtoplay_scene = Scene_AddScene(HowToPlay_Init, HowToPlay_Update, HowToPlay_Exit);
	Scene_ChangeScene(howtoplay_scene);// splash_screen);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{
	// check input, update simulation, render etc.
	CP_Settings_Background(CP_Color_Create(100, 100, 100, 255));
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
