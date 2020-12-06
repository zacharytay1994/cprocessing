#include <stdio.h>
#include "TestScene2.h"
#include "CProcessing/inc/cprocessing.h"
#include "Sprite.h"
#include "Particles.h"
#include "Camera.h"
#include "LePlant.h"
#include "Tilemap.h"
#include "GameGUI.h"

#define LEPLANT_PLANT_SCALE 0.5f
#define LEPLANT_POTION_SIZE 150

#define LEPLANT_MAX_BEANS 100
#define LEPLANT_BEAN_SIZE 100.0f

struct LePlant_Bean {
	int _id;
	CP_Vector _position;
	int _active;
	int _fly;
	float _rotation;
};

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

LP_Bean LePlant_beans[LEPLANT_MAX_BEANS] = { 0 };
int LePlant_beans_size = 0;

CP_Image LePlant_bean_image;
float LePlant_bean_fly_speed = 1500.0f;
float LePlant_bean_rotation_speed = 2000.0f;

void LePlant_Init()
{
	//bean_mr = Sprite_AddSpriteF((CP_Vector) { 0.0f, 0.0f }, 300.0f, 300.0f, "./Photos/Plants_PEXEL_02.png", 4, 3, 12, 0.8f, 1);
	bean_mr = Sprite_AddSpriteF((CP_Vector) { 0.0f, 0.0f }, 300.0f, 300.0f, "./Photos/edited_plant.png", 3, 4, 12, 0.8f, 1);
	smokey_mr = Sprite_AddSpriteF((CP_Vector) { 0.0f, 0.0f }, 300.0f, 300.0f, "./Photos/Smoke-02.png", 1, 3, 3, 0.2f, 1);
	potion_mr = Sprite_AddSpriteF((CP_Vector) { 0.0f, 0.0f }, 250.0f, 250.0f, "./Photos/Potions-11-sprite.png", 2, 3, 6, 5.0f, 1);

	LePlant_bean_image = CP_Image_Load("./Photos/Plants-02.png");
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
							0.02f, 225.0, 20, 2, 0);

						plant_pos.y += 5.0f;
						temp2 = Sprite_AddSpriteRepeatAuto(plant_pos, LEPLANT_POTION_SIZE, LEPLANT_POTION_SIZE, potion_mr);
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
			if (LePlant_ValidTile((int)grid.x, (int)grid.y) && GameGUI_GetBean() > 0) {
				temp = Sprite_AddSpriteRepeatAuto(cp_vector, 300.0f * LEPLANT_PLANT_SCALE, 300.0f * LEPLANT_PLANT_SCALE, bean_mr);
				Plant_id[plant_id] = temp;
				plant_id += 1;
				//Sprite_OptOut(temp, 0);
				Sprite_SetRepeat(temp, 0);
				GameGUI_SetBean(GameGUI_GetBean()-1);
			}
		}
		if (bool_check == 0)
		{
			if (LePlant_ValidTile((int)grid.x, (int)grid.y) && GameGUI_GetBean() > 0) {
				temp = Sprite_AddSpriteRepeatAuto(cp_vector, 300.0f * LEPLANT_PLANT_SCALE, 300.0f * LEPLANT_PLANT_SCALE, bean_mr);
				Plant_id[plant_id] = temp;
				plant_id += 1;
				//Sprite_OptOut(temp, 0);
				Sprite_SetRepeat(temp, 0);
				GameGUI_SetBean(GameGUI_GetBean()-1);
			}
		}
	}
	LePlant_UpdateBeans(dt);
}

void LePlant_Render(const float dt)
{
	for (int i = 0; i < potion_id; ++i) {
		Sprite_RenderSprite(dt, Potion_id[i]);
	}
	for (int i = 0; i < plant_id; ++i) {
		Sprite_RenderSprite(dt, Plant_id[i]);
	}
	LePlant_RenderBeans();
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

void LePlant_UpdateBeans(const float dt)
{
	for (int i = 0; i < LePlant_beans_size; ++i) {
		if (LePlant_beans[i]._active && LePlant_beans[i]._fly) {
			CP_Vector vector = CP_Vector_Subtract(Camera_ScreenToWorld(GameGUI_gui_bean_pos.x, GameGUI_gui_bean_pos.y), LePlant_beans[i]._position);
			if (CP_Vector_Length(vector) < 50.0f) {
				LePlant_beans[i]._active = 0;
				GameGUI_SetBean(GameGUI_GetBean() + 1);
			}
			LePlant_beans[i]._position = CP_Vector_Add(LePlant_beans[i]._position, CP_Vector_Scale(CP_Vector_Normalize(vector), LePlant_bean_fly_speed*dt));
			LePlant_beans[i]._rotation += LePlant_bean_rotation_speed * dt;
		}
	}
}

void LePlant_RenderBeans()
{
	for (int i = 0; i < LePlant_beans_size; ++i) {
		if (LePlant_beans[i]._active) {
			CP_Vector trans_pos = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), LePlant_beans[i]._position);
			CP_Image_DrawAdvanced(LePlant_bean_image, trans_pos.x, trans_pos.y, LEPLANT_BEAN_SIZE, LEPLANT_BEAN_SIZE, 255, LePlant_beans[i]._rotation);
		}
	}
}

void LePlant_AddBean(const CP_Vector position)
{
	CP_Vector pos = position;
	pos.y += 50.0f;
	if (LePlant_beans_size < LEPLANT_MAX_BEANS) {
		LePlant_beans[LePlant_beans_size] = (LP_Bean){ LePlant_beans_size, pos, 1, 0, 0.0f };
		++LePlant_beans_size;
	}
	else {
		for (int i = 0; i < LEPLANT_MAX_BEANS; ++i) {
			if (!LePlant_beans[i]._active) {
				LePlant_beans[LePlant_beans_size] = (LP_Bean){ i, pos, 1, 0, 0.0f };
			}
		}
	}
}

void LePlant_RemoveBean(const int id)
{
	if (id < LEPLANT_MAX_BEANS) {
		LePlant_beans[id]._active = 0;
	}
}

void LePlant_CheckBeanWithPlayerPosition(const CP_Vector position, const float w, const float h)
{
	float left_b;
	float right_b;
	float up_b;
	float bottom_b;
	float left_p = position.x - w/2.0f;
	float right_p = position.x + w/2.0f;
	float up_p = position.y - h/2.0f;
	float bottom_p = position.y + h/2.0f;
	for (int i = 0; i < LePlant_beans_size; ++i) {
		if (LePlant_beans[i]._active) {
			// check for bean hit box
			left_b = LePlant_beans[i]._position.x - LEPLANT_BEAN_SIZE * 0.5f;
			right_b = LePlant_beans[i]._position.x + LEPLANT_BEAN_SIZE * 0.5f;
			up_b = LePlant_beans[i]._position.y - LEPLANT_BEAN_SIZE * 0.5f;
			bottom_b = LePlant_beans[i]._position.y + LEPLANT_BEAN_SIZE * 0.5f;
			if (!(left_p > right_b || right_p < left_b || up_p > bottom_b || bottom_p < up_b)) {
				// there is collision
				LePlant_beans[i]._fly = 1;
			}
		}
	}
}