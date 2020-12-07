//---------------------------------------------------------
// file:	GameGUI.h
// author:	Zachary Tay
// email:	z.tay@digipen.edu
//
// brief:	Basic GUI functionality
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include "CProcessing/inc/cprocessing.h"

extern CP_Vector GameGUI_gui_bean_pos;
extern CP_Vector GameGUI_gui_potion_pos;

void GameGUI_Init();
void GameGUI_Render(const float dt);
void GameGUI_Exit();

void GameGUI_DrawText(CP_Color colour, const char* text, const float x, const float y, const float size, const CP_TEXT_ALIGN_HORIZONTAL h, const CP_TEXT_ALIGN_VERTICAL v);
void GameGUI_SetHealthScale(const float scale);

void GameGUI_RenderHitOverlay(const float dt);
void GameGUI_SetRedHitRatio(const float value); 
void GameGUI_RenderHealOverlay(const float dt);
void GameGUI_SetRedHealRatio(const float value);

int GameGUI_GetBean();
int GameGUI_GetPotion();
void GameGUI_SetBean(const int i);
void GameGUI_SetPotion(const int i);