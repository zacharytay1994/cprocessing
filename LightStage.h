#pragma once

#include "CProcessing/inc/cprocessing.h"

typedef struct LightStage_Light LightStage_Light;

void LightStage_Initialize();
void LightStage_Update(const float dt);
void LightStage_Render();
void LightStage_Exit();

int  LightStage_AddLight(const CP_Vector position, const float size, const float fadeUp, const float fadeDown, const float initialAlpha, const float midAlpha);
void LightStage_UpdateAndRenderLights(const float dt);

void LightStage_SetAmbient(const int ambient);

void LightStage_SetPosition(const int id, const CP_Vector position);
void LightStage_DeactivateLight(const int id);

void LightStage_ApplyNormalMap(	void* data, void* normal_data, void* og_data, 
								CP_Vector topLeftPosition, const float width, const float height,
								const float pixelWidth, const float pixelHeight,
								CP_Vector* lightPositions, int numberOfLights);

CP_Vector* LightStage_GetLightPositionsArray();
int LightStage_GetLightPositionsSize();