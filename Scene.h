//---------------------------------------------------------
// file:	Scene.h
// author:	Zachary
// email:	z.tay@digipen.edu
//
// brief:	Scene transition using function pointer to handle game state
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#define MAX_SCENES 10

typedef struct Scene_Scene {
	int _id;
	void (*init)();
	void (*update)(const float);
	void (*exit)();
} Scene_Scene;

extern Scene_Scene Scene_scenes[MAX_SCENES];
extern int Scene_scenes_size;

void Scene_Initialize();
void Scene_Update();
void Scene_Exit();

int Scene_AddScene(void (*init)(), void (*update)(const float), void (*exit)());
void Scene_ChangeScene(const int id);
int Scene_GetCurrentID();