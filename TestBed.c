#include "TestBed.h"

#include "PhyObj.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"
#include "GUI.h"

#include <stdio.h>

float cam_x = 0.0f;
float cam_y = 0.0f;

int tilemap;

void TestBed_Init()
{
	printf("Switched to testbed.\n");
	/*PhyObj_Initialize();
	Sprite_Initialize();*/

	// Setting up tilemaps -
	Tilemap_Initialize();
	tilemap = Tilemap_TxtLoad256("prototype_map.txt");
	Tilemap_GeneratePhyObjs(tilemap);
	Player_Initialize();

	Camera_BindToPosition(Player_GetPosition_P(0)); // magic number 0 here means player 1, index 0 in players array

	/*Sprite_InitData s_data = { (CP_Vector) { 100.0f,100.0f },100.0f,100.0f,"dirt_block.png",1,1,1,1,1 };
	int parent = GUI_AddRootContainer((CP_Vector) { 100.0f, 100.0f }, CP_Vector_Set(30.0f, 30.0f), s_data);
	s_data._position.x = 100.0f;
	s_data._path = "ground_block.png";
	s_data._width = 70.0f;
	s_data._height = 70.0f;
	parent = GUI_AddContainer(parent, (CP_Vector) { 200.0f, 200.0f }, CP_Vector_Set(30.0f, 30.0f), s_data);
	s_data._position.x = 100.0f;
	s_data._path = "ground_block.png";
	s_data._width = 30.0f;
	s_data._height = 30.0f;
	GUI_AddContainer(parent, (CP_Vector) { 200.0f, 200.0f }, CP_Vector_Set(30.0f, 30.0f), s_data);*/
}

void TestBed_Update(const float dt)
{
	//// UPDATES
	/*PhyObj_Update(dt);
	Camera_Update(dt);*/
	Player_Update(dt);
	//GUI_Update(dt);

	//// RENDERS
	Tilemap_Render(tilemap, Camera_GetCameraTransform());
	Tilemap_Debug_Render(tilemap, Camera_GetCameraTransform());
	/*PhyObj_Render();
	Sprite_Render(CP_System_GetDt());*/
	Player_Render();
	
}

void TestBed_Exit()
{
	printf("Exited TestBed.");
	//PhyObj_Free();
	Tilemap_Free();
	//Sprite_Free();
}
