//---------------------------------------------------------
// file:	GUI.h
// author:	Zachary Tay
// email:	z.tay@digipen.edu
//
// brief:	Nested containers for easy menu creation
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include "CProcessing/inc/cprocessing.h"
#include "Sprite.h"

#define GUI_MAX_NEST 10
#define GUI_MAX_DEPTH 10
#define GUI_MAX_BASE 100
#define GUI_MAX_CONTAINERS 100

typedef struct GUI_Container {
	int				_id;
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
void GUI_UpdateContainers(const float dt);
void GUI_UpdateNest(const float dt, const int index);

// id is a stack
int GUI_AddRootContainer(const CP_Vector pos, const CP_Vector dimension, const Sprite_InitData spriteData);
int GUI_AddContainer(const int parentId, const CP_Vector pos, const CP_Vector dimension, const Sprite_InitData path);
GUI_Container* GUI_GetContainer(const int* id);