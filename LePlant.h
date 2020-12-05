#pragma once

void LePlant_Init();
void LePlant_Update(const float dt);
void LePlant_Render(const float dt);
void LePlant_Exit();

void LePlant_BindTilemap(const int tilemap);
int  LePlant_ValidTile(const int x, const int y);