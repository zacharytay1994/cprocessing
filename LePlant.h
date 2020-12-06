#pragma once

typedef struct LePlant_Bean LP_Bean;
typedef struct LePlant_Potion LP_Potion;

void LePlant_Init();
void LePlant_Update(const float dt);
void LePlant_Render(const float dt);
void LePlant_Exit();

void LePlant_BindTilemap(const int tilemap);
int  LePlant_ValidTile(const int x, const int y);

void LePlant_UpdateBeans(const float dt);
void LePlant_UpdatePotions(const float dt);
void LePlant_RenderBeans();
void LePlant_AddBean(const CP_Vector position);
void LePlant_RemoveBean(const int id);
void LePlant_CheckBeanWithPlayerPosition(const CP_Vector position, const float w, const float h);
void LePlant_CheckPotionWithPlayerPosition(const CP_Vector position, const float w, const float h);