#pragma once

#include "CProcessing/inc/cprocessing.h"

#define MAX_SPRITES 100

typedef struct Sprite {
	CP_Vector	_position;
	float		_rotation;
	int			_alpha;
	float		_width;
	float		_height;
	int			_columns;
	int			_rows;
	float		_subwidth;
	float		_subheight;
	int			_current_frame;
	int			_number_of_frames;
	float		_frame_second;
	float		_frame_count;
	CP_Image	_sprite_sheet;
	CP_Image*	_sprites;
} Sprite;

extern Sprite sprites[MAX_SPRITES];
extern int sprites_size;

void Sprite_Initialize();
int Sprite_AddSprite(const CP_Vector position, const float width, const float height, const char* path, const int col, const int row, const int frame, const int fps);
void Sprite_AddSpriteFrame(const int id, const int frame, const CP_Image img);
void Sprite_RenderSpriteSub(const float dt, const int id);
void Sprite_RenderSpriteWhole(const float dt, const int id);
void Sprite_Render(const float dt);
CP_Image Sprite_GenerateSubImage(const float u0, const float v0, const float u1, const float v1, const CP_Image img);