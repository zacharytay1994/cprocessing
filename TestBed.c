#include "TestBed.h"

#include "PhyObj.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Sprite.h"

#include <stdio.h>

// just some test stuff
//PhyObjBoundingCircle* circle1;
//PhyObjOBoundingBox* box1;
//PhyObjOBoundingBox* box2;
//CP_Vector box_pos = { 100.0f, 100.0f };
float cam_x = 0.0f;
float cam_y = 0.0f;
//CP_Image test;
//float s0 = 1.0f;
//float t0 = 1.0f;
//float s1 = 1.0f;
//float t1 = 1.0f;

int tilemap;
//int sprite_test;
//CP_Image test_image;

void TestBed_Init()
{
	printf("Switched to testbed");
	PhyObj_Initialize();
	Tilemap_Initialize();
	Sprite_Initialize();

	/*PhyObj_AddCircle((float)(CP_System_GetWindowWidth() / 2), (float)(CP_System_GetWindowHeight() / 2), 0.0f, 30.0f,0.0f);
	PhyObj_AddOBox((float)(CP_System_GetWindowWidth() / 2) + 200.0f, (float)(CP_System_GetWindowHeight() / 2), 0.0f, 30.0f, 30.0f, 0.0f);
	PhyObj_AddOBox((float)(CP_System_GetWindowWidth() / 2), (float)(CP_System_GetWindowHeight() / 2) + 380.0f, 0.0f, 1200.0f, 100.0f, 0.0f);
	PhyObjOBoundingBox* b = PhyObj_AddOBox((float)(CP_System_GetWindowWidth() / 2) - 600.0f, (float)(CP_System_GetWindowHeight() / 2) - 200.0f, 0.0f, 300.0f, 10.0f, 0.0f);
	b->super._rotation = 30.0f;
	sprite_test = Sprite_AddSprite((CP_Vector) { 100.0f, 100.0f }, 100.0f, 100.0f, "demo_player.png", 8, 1, 8, 10);
	Sprite_AddSprite((CP_Vector) { 200.0f, 150.0f }, 50.0f, 50.0f, "demo_player2.png", 8, 2, 16, 20);*/

	// adding a tilemap to the scene
	tilemap = Tilemap_AddTilemap(64, 64, 10, 10);
	//test = CP_Image_Load("demo_player.png");

	//PhyObj_SetAllVisible(1);
}

void TestBed_Update(const float dt)
{
	// UPDATES
	PhyObj_Update(CP_System_GetDt());
	Camera_Update(CP_System_GetDt());

	// RENDERS
	Tilemap_Render(tilemap, Camera_GetCameraTransform());
	Tilemap_Debug_Render(tilemap, Camera_GetCameraTransform()); // renders tilemap grid lines, comment out if not wanted
	PhyObj_Render();
	Sprite_Render(CP_System_GetDt());

	if (CP_Input_MouseClicked()) {
		CP_Vector world = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		CP_Vector tile = Tilemap_WorldToGrid(tilemap, world.x, world.y);
		printf("%.1f,%.1f\n", tile.x, tile.y);
		Tilemap_SetTile(tilemap, (int)tile.x, (int)tile.y, Tilemap_Solid);
	}

	if (CP_Input_KeyReleased(KEY_P)) {
		Tilemap_GeneratePhyObjs(tilemap);
		PhyObj_SetAllVisible(1);
	}
	if (CP_Input_KeyReleased(KEY_O)) {
		CP_Vector world = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		PhyObj_AddOBox(world.x, world.y, 30.0f, 60.0f, 60.0f, 0.6f)->super._visible = 1;
	}
	// INPUT
	//if (CP_Input_MouseClicked()) {
	//	/*CP_Vector world = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
	//	CP_Vector tile = Tilemap_WorldToGrid(tilemap, world.x, world.y);
	//	printf("%.1f,%.1f\n", tile.x, tile.y);
	//	Tilemap_SetTile(tilemap, (int)tile.x, (int)tile.y, Tilemap_Solid);*/
	//	int i = CP_Random_RangeInt(1, 6);
	//	int r = CP_Random_RangeInt(1, 6);
	//	int j = i * r;
	//	if (CP_Input_KeyDown(KEY_B)) {
	//		PhyObj_AddOBox(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 1.0f * j, 10.0f * 3.0f, 10.0f * 6.0f, 0.0f)->super._visible = 1;
	//		//PhyObj_AddOBox(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 30.0f, 30.0f, 30.0f);
	//	}
	//	else if (CP_Input_KeyDown(KEY_N)) {
	//		PhyObj_AddOBox(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 30.0f, 30.0f, 30.0f, 0.0f)->super._visible = 1;
	//	}
	//	else {
	//		PhyObj_AddCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 3 * 10.0f, 3 * 10.0f, 0.0f)->super._visible = 1;
	//	}
	//}
	//if (CP_Input_KeyDown(KEY_S)) {
	//	if (CP_Input_MouseClicked()) {
	//		Tilemap_Save(tilemap, "test.bin");
	//	}
	//}
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
	//// arrow keys
	//if (CP_Input_KeyDown(KEY_RIGHT)) {
	//	Sprite_SetPosition(sprite_test, CP_Vector_Add(Sprite_GetPosition(sprite_test), (CP_Vector) { 1.0f, 0.0f }));
	//}
	//if (CP_Input_KeyDown(KEY_LEFT)) {
	//	Sprite_SetPosition(sprite_test, CP_Vector_Add(Sprite_GetPosition(sprite_test), (CP_Vector) { -1.0f, 0.0f }));
	//}
	//if (CP_Input_KeyDown(KEY_UP)) {
	//	Sprite_SetPosition(sprite_test, CP_Vector_Add(Sprite_GetPosition(sprite_test), (CP_Vector) { 0.0f, -1.0f }));
	//}
	//if (CP_Input_KeyDown(KEY_DOWN)) {
	//	Sprite_SetPosition(sprite_test, CP_Vector_Add(Sprite_GetPosition(sprite_test), (CP_Vector) { 0.0f, 1.0f }));
	//}
}

void TestBed_Exit()
{
	printf("exited to testbed");
	//PhyObj_Free();
	Tilemap_Free();
	Sprite_Free();
}
