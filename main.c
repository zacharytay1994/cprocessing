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

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	CP_System_SetWindowSize(1600, 900);
	CP_Settings_AntiAlias(0);

	PhyObj_Initialize();
	Tilemap_Initialize();
	Sprite_Initialize();

	
	sprite_test = Sprite_AddSprite((CP_Vector){ 100.0f, 100.0f }, 100.0f, 100.0f, "demo_player.png", 8, 1, 8, 10);
	Sprite_AddSprite((CP_Vector) { 200.0f, 150.0f }, 50.0f, 50.0f, "demo_player2.png", 8, 2, 16, 20);
	//test_image = CP_Image_Load("demo_test.png");
	//Sprite_AddSpriteFrame(sprite_test, 1, Sprite_GenerateSubImages(0.0f,0.0f,0.75f,0.75f,0,test_image));
	//Sprite_AddSpriteFrame(sprite_test, 2, Sprite_GenerateSubImages(0.25f, 0.25f, 0.75f, 0.75f, 0, test_image));
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
	PhyObj_Render();
	Tilemap_Debug_Render(tilemap, Camera_GetCameraTransform());
	Tilemap_RenderGrid(tilemap, Camera_GetCameraTransform());
	Sprite_Render(CP_System_GetDt());
	if (CP_Input_KeyDown(KEY_RIGHT)) {
		s0+= CP_System_GetDt();
	}
	if (CP_Input_KeyDown(KEY_LEFT)) {
		t0+= CP_System_GetDt();
	}
	if (CP_Input_KeyDown(KEY_UP)) {
		s1+= CP_System_GetDt();
	}
	if (CP_Input_KeyDown(KEY_DOWN)) {
		t1+= CP_System_GetDt();
	}
	//CP_Image_DrawSubImage(test, 100.0f, 100.0f, 100.0f, 100.0f, s0, t0, s1, t1, 255);

	// INPUT
	if (CP_Input_MouseClicked()) {
		CP_Vector world = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		CP_Vector tile = Tilemap_WorldToGrid(tilemap, world.x, world.y);
		printf("%.1f,%.1f\n", tile.x, tile.y);
		Tilemap_SetTile(tilemap, (int)tile.x, (int)tile.y, Tilemap_Solid);
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
	Camera_SetCameraX(cam_x);
	Camera_SetCameraY(cam_y);
	/*CP_Vector camera_transformed_box_pos = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), box_pos);
	CP_Graphics_DrawRect(camera_transformed_box_pos.x, camera_transformed_box_pos.y, 30.0f, 30.0f);*/
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
	PhyObj_Free();
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
