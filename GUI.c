#include "GUI.h"
#include "Sprite.h"
#include <stdio.h>

GUI_Container GUI_containers[GUI_MAX_CONTAINERS];
int GUI_containers_size = 0;

void GUI_AddContainer(const int* id, const CP_Vector pos, const CP_Vector dimension, const char* path)
{
	if (GUI_containers_size < GUI_MAX_BASE) {
		/*GUI_Container new_container = {{0},pos,dimension,Sprite_AddSprite(path)}
		for (int i = 0; i < GUI_MAX_DEPTH; ++i) {

		}
		GUI_containers*/
	}
}

GUI_Container* GUI_GetContainer(const int* id)
{
	//GUI_Container* current_container;
	//// check if valid base
	//if (!id[0]) {
	//	printf("GUI_GetContainer :: Invalid base!");
	//	return NULL;
	//}
	//else {
	//	current_container = &GUI_containers[id[0]];
	//}
	//for (int i = 1; i < GUI_MAX_DEPTH; ++i) {
	//	// end of id
	//	if (id[i] == -1) {
	//		break;
	//	}
	//	// if invalid id
	//	if (id[i] >= current_container->_nest_size) {
	//		printf("GUI_GetContainer :: Invalid id chain, container not found!");
	//		return NULL;
	//	}
	//	// iterate to next container
	//	current_container = current_container->_nest[i];
	//}
	//return current_container;
	return NULL;
}
