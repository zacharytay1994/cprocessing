#include "TestBed.h"

#include "PhyObj.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"

#include <stdio.h>

float cam_x = 0.0f;
float cam_y = 0.0f;

int tilemap;

void TestBed_Init()
{
	printf("Switched to testbed.\n");
	PhyObj_Initialize();
	Sprite_Initialize();
	Tilemap_Initialize();

	// Setting up tilemaps -
	tilemap = Tilemap_TxtLoad256("prototype_map.txt");
	Tilemap_GeneratePhyObjs(tilemap);
	Player_Initialize();
}

void TestBed_Update(const float dt)
{
	// UPDATES
	PhyObj_Update(dt);
	Camera_Update(dt);
	Player_Update(dt);

	// RENDERS
	Tilemap_Render(tilemap, Camera_GetCameraTransform());
	PhyObj_Render();
	Sprite_Render(CP_System_GetDt());
	Player_Render();

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
}

void TestBed_Exit()
{
	printf("Exited TestBed.");
	PhyObj_Free();
	Tilemap_Free();
	Sprite_Free();
}
