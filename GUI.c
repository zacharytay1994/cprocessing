#include "GUI.h"
#include "Sprite.h"
#include <stdio.h>

GUI_Container GUI_containers[GUI_MAX_CONTAINERS];
int GUI_containers_size = 0;
GUI_Container* GUI_root_containers[GUI_MAX_CONTAINERS];
int GUI_root_containers_size = 0;

void GUI_Update(const float dt)
{
	GUI_UpdateContainers(dt);
}

// recursively render root and all child buttons
void GUI_UpdateContainers(const float dt)
{
	for (int i = 0; i < GUI_root_containers_size; ++i) {
		Sprite_RenderSprite(dt, GUI_root_containers[i]->_sprite);
		for (int j = 0; j < GUI_root_containers[i]->_nest_size; ++j) {
			GUI_UpdateNest(dt,GUI_root_containers[i]->_nest[j]);
		}
	}
}

// recursively render all child buttons
void GUI_UpdateNest(const float dt, const int index)
{
	Sprite_RenderSprite(dt, GUI_containers[index]._sprite);
	for (int j = 0; j < GUI_containers[index]._nest_size; ++j) {
		GUI_UpdateNest(dt, GUI_containers[index]._nest[j]);
	}
}

int GUI_AddRootContainer(const CP_Vector pos, const CP_Vector dimension, const Sprite_InitData spriteData)
{
	if (GUI_root_containers_size < GUI_MAX_CONTAINERS && GUI_containers_size < GUI_MAX_CONTAINERS) {
		GUI_containers[GUI_containers_size] = (GUI_Container){ GUI_containers_size,pos,dimension,Sprite_AddSpriteInitData(spriteData),{0},0,1 };
		GUI_root_containers[GUI_root_containers_size++] = &(GUI_containers[GUI_containers_size]);
		++GUI_containers_size;
		return GUI_containers_size - 1;
	}
	else {
		printf("GUI_AddRootContainer :: Container size exceeded!");
	}
	return -1;
}

int GUI_AddContainer(const int parentId, const CP_Vector pos, const CP_Vector dimension, const Sprite_InitData spriteData)
{
	if (parentId < GUI_containers_size) {
		if (GUI_containers_size < GUI_MAX_CONTAINERS) {
			GUI_containers[GUI_containers_size] = (GUI_Container){ GUI_containers_size,pos,dimension,Sprite_AddSpriteInitData(spriteData),{0},0,1 };
			if (GUI_containers[parentId]._nest_size < GUI_MAX_DEPTH) {
				GUI_containers[parentId]._nest[GUI_containers[parentId]._nest_size++] = GUI_containers_size;
				++GUI_containers_size;
				return GUI_containers_size - 1;
			}
			else {
				printf("GUI_AddContainer :: Button %d nest size exceeded!", parentId);
			}
		}
		else {
			printf("GUI_AddContainer :: Container size exceeded!");
		}
	}
	else {
		printf("GUI_AddContainer :: Invalid parentID.");
	}
	return -1;
}

GUI_Container* GUI_GetContainer(const int* id)
{
	GUI_Container* current_container;
	// check if valid base
	if (!id[0]) {
		printf("GUI_GetContainer :: Invalid base!");
		return NULL;
	}
	else {
		current_container = GUI_root_containers[id[0]];
	}
	for (int i = 1; i < GUI_MAX_DEPTH; ++i) {
		// end of id
		if (id[i] == -1) {
			break;
		}
		// if invalid id
		if (id[i] >= current_container->_nest_size) {
			printf("GUI_GetContainer :: Invalid id chain, container not found!");
			return NULL;
		}
		// iterate to next container
		current_container = &GUI_containers[current_container->_nest[i]];
	}
	return current_container;
}
