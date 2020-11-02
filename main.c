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
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "cprocessing.h"
#include "PhyObj.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Sprite.h"

#include <stdio.h>
#include <stdlib.h>

// just some test stuff
PhyObjBoundingCircle* circle1;
PhyObjOBoundingBox* box1;
PhyObjOBoundingBox* box2;
CP_Vector box_pos = { 100.0f, 100.0f };
float cam_x = 0.0f;
float cam_y = 0.0f;
CP_Image test;
float s0 = 1.0f;
float t0 = 1.0f;
float s1 = 1.0f;
float t1 = 1.0f;

int tilemap;
int sprite_test;
CP_Image test_image;

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

	PhyObj_Initialize();
	Tilemap_Initialize();
	Sprite_Initialize();

	PhyObj_AddCircle((float)(CP_System_GetWindowWidth() / 2), (float)(CP_System_GetWindowHeight() / 2), 0.0f, 30.0f);
	PhyObj_AddOBox((float)(CP_System_GetWindowWidth() / 2) + 200.0f, (float)(CP_System_GetWindowHeight() / 2), 0.0f, 30.0f, 30.0f);
	PhyObj_AddOBox((float)(CP_System_GetWindowWidth() / 2), (float)(CP_System_GetWindowHeight() / 2) + 380.0f, 0.0f, 1200.0f, 100.0f);
	PhyObjOBoundingBox* b = PhyObj_AddOBox((float)(CP_System_GetWindowWidth() / 2)-600.0f, (float)(CP_System_GetWindowHeight() / 2)-200.0f, 0.0f, 300.0f, 10.0f);
	b->super._rotation = 30.0f;
	sprite_test = Sprite_AddSprite((CP_Vector){ 100.0f, 100.0f }, 100.0f, 100.0f, "demo_player.png", 8, 1, 8, 10);
	Sprite_AddSprite((CP_Vector) { 200.0f, 150.0f }, 50.0f, 50.0f, "demo_player2.png", 8, 2, 16, 20);

	//Button Shortcut init
	MainBtnArr[0].b_widthPos = (float)(CP_System_GetWindowWidth() / 1.1f);
	MainBtnArr[0].b_heightPos = (float)(CP_System_GetWindowHeight() / 9);
	MainBtnArr[0].b_widthSize = (float)(CP_System_GetWindowWidth() / 9);
	MainBtnArr[0].b_heightSize = (float)(CP_System_GetWindowHeight() / 11);
	Sprite_AddSprite((CP_Vector) { MainBtnArr[0].b_widthPos, MainBtnArr[0].b_heightPos},
		MainBtnArr[0].b_widthSize, MainBtnArr[0].b_heightSize, "Assets/demo_mShortcut.png", 1, 1, 1, 1);

	MainBtnArr[1].b_widthPos = (float)(CP_System_GetWindowWidth() / 1.25f);
	MainBtnArr[1].b_heightPos = (float)(CP_System_GetWindowHeight() / 2);
	MainBtnArr[1].b_widthSize = (float)(CP_System_GetWindowWidth() / 9);
	MainBtnArr[1].b_heightSize = (float)(CP_System_GetWindowHeight() / 1.2);

	// adding a tilemap to the scene
	tilemap = Tilemap_AddTilemap(32, 32, 100, 100);
	test = CP_Image_Load("demo_player.png");
	// system settings
	CP_System_ShowConsole();
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{
	// check input, update simulation, render etc.
	CP_Settings_Background(CP_Color_Create(100, 100, 100, 255));
	// process physics system
	
	// UPDATES
	PhyObj_Update(CP_System_GetDt());
	Camera_Update(CP_System_GetDt());
	
	// RENDERS
	Tilemap_Render(tilemap, Camera_GetCameraTransform());
	Tilemap_Debug_Render(tilemap, Camera_GetCameraTransform()); // renders tilemap grid lines, comment out if not wanted
	PhyObj_Render();
	Sprite_Render(CP_System_GetDt());

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

	// INPUT
	if (CP_Input_MouseClicked()) {
		/*CP_Vector world = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		CP_Vector tile = Tilemap_WorldToGrid(tilemap, world.x, world.y);
		printf("%.1f,%.1f\n", tile.x, tile.y);
		Tilemap_SetTile(tilemap, (int)tile.x, (int)tile.y, Tilemap_Solid);*/
		int i = CP_Random_RangeInt(1, 6);
		int r = CP_Random_RangeInt(1, 6);
		int j = i * r;
		if (CP_Input_KeyDown(KEY_B)) {
			PhyObj_AddOBox(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 1.0f * j, 10.0f * 3.0f, 10.0f * 6.0f);
			//PhyObj_AddOBox(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 30.0f, 30.0f, 30.0f);
		}
		else if (CP_Input_KeyDown(KEY_N)) {
			PhyObj_AddOBox(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 30.0f, 30.0f, 30.0f);
		}
		else {
			if (!(CP_Input_GetMouseX() > MainBtnArr[0].b_widthPos + MainBtnArr[0].b_widthSize/2 ||
				CP_Input_GetMouseX() < MainBtnArr[0].b_widthPos - MainBtnArr[0].b_widthSize /2 ||
				CP_Input_GetMouseY() > MainBtnArr[0].b_heightPos + MainBtnArr[0].b_heightSize /2 ||
				CP_Input_GetMouseY() < MainBtnArr[0].b_heightPos - MainBtnArr[0].b_heightSize /2))
			{
				Sprite_AddSprite((CP_Vector) {
					MainBtnArr[1].b_widthPos, MainBtnArr[1].b_heightPos},
					MainBtnArr[1].b_widthSize, MainBtnArr[1].b_heightSize, "dirt_block.png", 1, 1, 1, 1);
			}
			else
				PhyObj_AddCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 3 * 10.0f, 3 * 10.0f);
		}
	}
	if (CP_Input_KeyDown(KEY_S)) {
		if (CP_Input_MouseClicked()) {
			Tilemap_Save(tilemap, "test.bin");
		}
 	}
	// moving the camera
	if (CP_Input_KeyDown(KEY_W)) {
		cam_y -= 100.0f * CP_System_GetDt();
	}
	if (CP_Input_KeyDown(KEY_S)) {
		cam_y += 100.0f * CP_System_GetDt();
	}
	if (CP_Input_KeyDown(KEY_A)) {
		cam_x -= 100.0f * CP_System_GetDt();
	}
	if (CP_Input_KeyDown(KEY_D)) {
		cam_x += 100.0f * CP_System_GetDt();
	}
	Camera_SetCameraX(cam_x);
	Camera_SetCameraY(cam_y);
	// arrow keys
	if (CP_Input_KeyDown(KEY_RIGHT)) {
		Sprite_SetPosition(sprite_test, CP_Vector_Add(Sprite_GetPosition(sprite_test), (CP_Vector){1.0f,0.0f}));
	}
	if (CP_Input_KeyDown(KEY_LEFT)) {
		Sprite_SetPosition(sprite_test, CP_Vector_Add(Sprite_GetPosition(sprite_test), (CP_Vector) { -1.0f, 0.0f }));
	}
	if (CP_Input_KeyDown(KEY_UP)) {
		Sprite_SetPosition(sprite_test, CP_Vector_Add(Sprite_GetPosition(sprite_test), (CP_Vector) { 0.0f, -1.0f }));
	}
	if (CP_Input_KeyDown(KEY_DOWN)) {
		Sprite_SetPosition(sprite_test, CP_Vector_Add(Sprite_GetPosition(sprite_test), (CP_Vector) { 0.0f, 1.0f }));
	}
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
	PhyObj_Free();
	Tilemap_Free();
	Sprite_Free();
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
