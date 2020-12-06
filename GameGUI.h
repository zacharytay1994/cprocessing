#pragma once

#include "CProcessing/inc/cprocessing.h"

void GameGUI_Init();
void GameGUI_Render(const float dt);
void GameGUI_Exit();

void GameGUI_DrawText(CP_Color colour, const char* text, const float x, const float y, const float size, const CP_TEXT_ALIGN_HORIZONTAL h, const CP_TEXT_ALIGN_VERTICAL v);
void GameGUI_SetHealthScale(const float scale);

void GameGUI_RenderHitOverlay(const float dt);
void GameGUI_SetRedHitRatio(const float value);