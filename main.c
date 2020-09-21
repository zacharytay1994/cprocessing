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
#include <stdio.h>

PhyObjBoundingCircle* circle1;
PhyObjOBoundingBox* box1;

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	PhyObj_Initialize();
	// initialize variables and CProcessing settings for this gamestate
	/*circle1 = PhyObj_AddCircle(100.0f, 100.0f, 30.0f, 30.0f);
	circle2 = PhyObj_AddCircle(250.0f, 100.0f, 20.0f, 20.0f);
	PhyObj_AddCircle(100.0f, 250.0f, 20.0f, 20.0f);
	PhyObj_AddCircle(250.0f, 250.0f, 30.0f, 30.0f);*/
	//box1 = PhyObj_AddOBox(100.0f, 100.0f, 30.0f, 30.0f, 30.0f);
	//box1->super._angular_velocity = 1.0f;
	//PhyObj_AddCircle(100.0f, 100.0f, 30.0f, 30.0f);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{
	PhyObj_GlobalAcceleration();
	PhyObj_DrawCircles();
	PhyObj_DrawOBoxes();
	// check input, update simulation, render etc.
	CP_Settings_Background(CP_Color_Create(100, 100, 100, 255));
	if (CP_Input_KeyDown(KEY_P)) {
		//PhyObj_ApplyImpulse(circle1, (CP_Vector) { 100.0f, 0.0f });
	}
	if (CP_Input_KeyDown(KEY_O)) {
		//PhyObj_ApplyImpulse(circle2, (CP_Vector) { 1.0f, 0.0f });
	}
	if (CP_Input_KeyDown(KEY_L)) {
		float random_mass = (float)CP_Random_RangeInt(20, 40);
		PhyObj_AddOBox(CP_Input_GetMouseX(), CP_Input_GetMouseY(), random_mass, random_mass, random_mass);
	}
	if (CP_Input_MouseClicked()) {
		float random_mass = (float)CP_Random_RangeInt(5, 30);
		PhyObj_AddCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), random_mass, random_mass);
		//PhyObj_AddOBox(CP_Input_GetMouseX(), CP_Input_GetMouseY(), random_mass, random_mass, random_mass);
	}
	PhyObj_UpdatePosition(CP_System_GetDt());
	PhyObj_UpdateRotation(CP_System_GetDt());
	//CP_Vector mouse = (CP_Vector){ CP_Input_GetMouseX(),CP_Input_GetMouseY() };
	//CP_Vector result = PhyObj_NearestPointOnOBBToPoint(mouse, box1);
	//CP_Graphics_DrawCircle(result.x, result.y, 2);
	//PhyObj_SetPosition(circle1, (CP_Vector) {CP_Input_GetMouseX(),CP_Input_GetMouseY()});
	PhyObj_CheckForCollisions();
	PhyObj_IterativeSolveManifolds(1);
	//background(color(20, 200, 255, 255));
	/*CP_Graphics_DrawRect(100.0f, 100.0f, 50.0f, 50.0f);
	CP_Graphics_DrawRect(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 25.0f, 25.0f);*/
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
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
