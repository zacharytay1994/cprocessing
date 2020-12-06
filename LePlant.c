#include <stdio.h>
#include "TestScene2.h"
#include "CProcessing/inc/cprocessing.h"
#include "Sprite.h"
#include "Particles.h"
#include "Camera.h"
#include "LePlant.h"
#include "Tilemap.h"

#define LEPLANT_PLANT_SCALE 0.5f

int Plant_id[100];
int Potion_id[100];
int plant_id = 0, potion_id = 0;
int bean_mr, smokey_mr, potion_mr;
int temp, temp2;
int bool_check = 1;
int smoke_check = 1;

int LePlant_tilemap;
CP_Vector LePlant_valid_ground_tiles[50];
int LePlant_valid_ground_tiles_size = 0;

void LePlant_Init()
{
	//bean_mr = Sprite_AddSpriteF((CP_Vector) { 0.0f, 0.0f }, 300.0f, 300.0f, "./Photos/Plants_PEXEL_02.png", 4, 3, 12, 0.8f, 1);
	bean_mr = Sprite_AddSpriteF((CP_Vector) { 0.0f, 0.0f }, 300.0f, 300.0f, "./Photos/edited_plant.png", 3, 4, 12, 0.8f, 1);
	smokey_mr = Sprite_AddSpriteF((CP_Vector) { 0.0f, 0.0f }, 300.0f, 300.0f, "./Photos/Smoke-02.png", 1, 3, 3, 0.2f, 1);
	potion_mr = Sprite_AddSpriteF((CP_Vector) { 0.0f, 0.0f }, 250.0f, 250.0f, "./Photos/Potions-11.png", 1, 1, 1, 0.2f, 1);

	//Particle_Initialize();
}

void LePlant_Update(const float dt)
{
	//Particle_Update(dt);
	float MouseX = CP_Input_GetMouseX();
	float MouseY = CP_Input_GetMouseY();
	CP_Vector mouse = Camera_ScreenToWorld(MouseX, MouseY);
	CP_Vector grid = Tilemap_WorldToGrid(LePlant_tilemap, mouse.x, mouse.y);

	//CP_Vector cp_vector = CP_Vector_Set(CP_Input_GetMouseX(), (CP_Input_GetMouseY() - 70.0f));
	//CP_Vector cp_vector = Camera_ScreenToWorld((float)CP_Input_GetMouseX(), (float)CP_Input_GetMouseY() - 70.0f * LEPLANT_PLANT_SCALE);
	CP_Vector cp_vector = (CP_Vector){ tilemaps[LePlant_tilemap]._tile_width * grid.x + (float)tilemaps[LePlant_tilemap]._tile_width/2.0f,
										tilemaps[LePlant_tilemap]._tile_height * grid.y };
	CP_Vector cp_vector_reset = CP_Vector_Set(0.0f, 0.0f);

	if (CP_Input_MouseReleased(1))
	{
		bool_check = 0;
		if (plant_id > 0)
		{
			for (int i = 0; i < plant_id; ++i)
			{
				Sprite* sprite = Sprite_GetSprite(Plant_id[i]);
				CP_Vector plant_pos = Sprite_GetPosition(Plant_id[i]);
				float left = plant_pos.x - 45.0f;
				float right = plant_pos.x + 45.0f;
				float up = plant_pos.y - 100.0f * LEPLANT_PLANT_SCALE;
				float down = plant_pos.y + 150.0f * LEPLANT_PLANT_SCALE;
				if (!(mouse.x < left || mouse.x > right || mouse.y < up || mouse.y > down))
				{
					bool_check = 1;

					if (sprite->_current_frame == 12)
					{

						Sprite_SetAlpha(Plant_id[i], 0.0f);
						Particle_EmitOut(PT_Bean, plant_pos, 100.0f, 200.0f, -10.0f,
							-10.0f, 20.0f, -20.0f, 3.0f,
							1.0f, 0.0f, 0.0f, 0.04f,
							0.02f, 225.0, 20, 2);

						temp2 = Sprite_AddSpriteRepeatAuto(plant_pos, 250.0f, 250.0f, potion_mr);
						Potion_id[potion_id] = temp2;
						potion_id += 1;
						//Sprite_OptOut(temp2, 0);

						Sprite_SetPosition(Plant_id[i], cp_vector_reset);

					}
				}
			}
		}
		else
		{
			if (LePlant_ValidTile((int)grid.x, (int)grid.y)) {
				temp = Sprite_AddSpriteRepeatAuto(cp_vector, 300.0f * LEPLANT_PLANT_SCALE, 300.0f * LEPLANT_PLANT_SCALE, bean_mr);
				Plant_id[plant_id] = temp;
				plant_id += 1;
				//Sprite_OptOut(temp, 0);
				Sprite_SetRepeat(temp, 0);
			}
		}
		if (bool_check == 0)
		{
			if (LePlant_ValidTile((int)grid.x, (int)grid.y)) {
				temp = Sprite_AddSpriteRepeatAuto(cp_vector, 300.0f * LEPLANT_PLANT_SCALE, 300.0f * LEPLANT_PLANT_SCALE, bean_mr);
				Plant_id[plant_id] = temp;
				plant_id += 1;
				//Sprite_OptOut(temp, 0);
				Sprite_SetRepeat(temp, 0);
			}
		}
	}
}

void LePlant_Render(const float dt)
{
	for (int i = 0; i < potion_id; ++i) {
		Sprite_RenderSprite(dt, Potion_id[i]);
	}
	for (int i = 0; i < plant_id; ++i) {
		Sprite_RenderSprite(dt, Plant_id[i]);
	}
}

void LePlant_Exit()
{
}

void LePlant_BindTilemap(const int tilemap)
{
	LePlant_tilemap = tilemap;
	LePlant_valid_ground_tiles_size = Tilemap_GetValidGroundTiles(tilemap, LePlant_valid_ground_tiles);
}

int LePlant_ValidTile(const int x, const int y)
{
	for (int i = 0; i < LePlant_valid_ground_tiles_size; ++i) {
		if ((int)LePlant_valid_ground_tiles[i].x == x && (int)LePlant_valid_ground_tiles[i].y == y) {
			return 1;
		}
	}
	return 0;
}
