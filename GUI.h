#pragma once

#include "CProcessing/inc/cprocessing.h"
#include "Sprite.h"

#define GUI_MAX_NEST 10
#define GUI_MAX_DEPTH 10
#define GUI_MAX_BASE 100
#define GUI_MAX_CONTAINERS 100

typedef struct GUI_Container {
	int				_id[GUI_MAX_DEPTH];
	CP_Vector		_position;
	CP_Vector		_dimensions;
	int				_sprite;
	int				_nest[GUI_MAX_NEST];
	int				_nest_size;
	int				_visible;
} GUI_Container;

void GUI_Initialize();
void GUI_Update(const float dt);
void GUI_Exit();
void GUI_RenderContainers();

// id is a stack
void GUI_AddContainer(const int* id, const CP_Vector pos, const CP_Vector dimension, const char* path);
GUI_Container* GUI_GetContainer(const int* id);