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
#include "Demo.h"

#include <stdio.h>
#include <stdlib.h>

PhyObjBoundingCircle* circle1;
PhyObjOBoundingBox* box1;
PhyObjOBoundingBox* box2;
CP_Vector box_pos = { 100.0f, 100.0f };
float cam_x = 0.0f;
float cam_y = 0.0f;

int tilemap;

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	CP_System_SetWindowSize(1600, 900);
	CP_Settings_AntiAlias(0);
	PhyObj_Initialize();
	/*Demo_Initialize();
	Demo_AddEnemy(0.0f);
	Demo_AddEnemy((float)CP_System_GetWindowWidth());*/
	
	Tilemap_Initialize();
	tilemap = Tilemap_AddTilemap(32, 32, 32, 32);
	Tilemap_SetTile(tilemap, 0, 0, Tilemap_Solid);
	Tilemap_SetTile(tilemap, 0, 1, Tilemap_Solid);
	Tilemap_SetTile(tilemap, 1, 0, Tilemap_Solid);
	Tilemap_SetTile(tilemap, 1, 1, Tilemap_Ground);
	CP_System_ShowConsole();
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{
	// check input, update simulation, render etc.
	CP_Settings_Background(CP_Color_Create(100, 100, 100, 255));
	// process physics system
	PhyObj_Render();
	PhyObj_Update(CP_System_GetDt());
	
	Camera_Update(CP_System_GetDt());
	Tilemap_Debug_Render(tilemap, Camera_GetCameraTransform());
	if (CP_Input_MouseClicked()) {
		CP_Vector world = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		CP_Vector tile = Tilemap_WorldToGrid(tilemap, world.x, world.y);
		printf("%.1f,%.1f\n", tile.x, tile.y);
	}
	Tilemap_RenderTileGrid(tilemap, Camera_GetCameraTransform());
	/*Demo_Render();
	Demo_Update(CP_System_GetDt());
	Demo_EnemiesUpdate(CP_System_GetDt());

	if (CP_Input_KeyReleased(KEY_S)) {
		Demo_SpawnWave(2.0f, 4);
	}
	if (CP_Input_MouseClicked()) {
		Demo_AddDamageNumbers(100.0f, 100.0f);
	}*/

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
	CP_Vector camera_transformed_box_pos = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), box_pos);
	CP_Graphics_DrawRect(camera_transformed_box_pos.x, camera_transformed_box_pos.y, 30.0f, 30.0f);
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
	PhyObj_Free();
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
