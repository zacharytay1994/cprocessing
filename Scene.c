#include "Scene.h"
#include "PhyObj.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "Camera.h"
#include "CProcessing/inc/cprocessing.h"
#include "Player.h"
#include "GUI.h"
#include <stdio.h>

Scene_Scene Scene_scenes[MAX_SCENES] = { 0 };
int Scene_scenes_size = 0;

int Scene_has_scene = 0;
Scene_Scene current_scene;

void Scene_Initialize()
{
	// SCENE INITIALIZE
	PhyObj_Initialize();
	Sprite_Initialize();
}

void Scene_Update()
{
	float dt = CP_System_GetDt();
	// SCENE UPDATES
	PhyObj_Update(dt);
	Camera_Update(dt);
	GUI_Update(dt);

	if (Scene_has_scene) {
		current_scene.update(CP_System_GetDt());
	}

	// SCENE RENDERS
	//PhyObj_Render();
	Sprite_Render(CP_System_GetDt());
}

void Scene_Exit()
{
	// SCENE EXITS
	PhyObj_Free();
	Sprite_Free();
}

int Scene_AddScene(void(*init)(), void(*update)(const float), void(*exit)())
{
	if (Scene_scenes_size < MAX_SCENES) {
		Scene_scenes[Scene_scenes_size] = (Scene_Scene){ Scene_scenes_size, init, update, exit };
		Scene_scenes_size++;
		return Scene_scenes_size - 1;
	}
	else {
		printf("Scenes container size exceeded!");
		return -1;
	}
}

void Scene_ChangeScene(const int id)
{
	if (id < Scene_scenes_size) {
		if (Scene_has_scene) {
			current_scene.exit();
		}
		current_scene = Scene_scenes[id];
		current_scene.init();
		Scene_has_scene = 1;
	}
	else {
		printf("No scene with id exists");
	}
}

int Scene_GetCurrentID()
{
	return current_scene._id;
}