//---------------------------------------------------------
// file:	Sprite.h
// author:	Zachary
// email:	z.tay@digipen.edu
//
// brief:	Sprite class for animation and control
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include "CProcessing/inc/cprocessing.h"

#define MAX_SPRITES 500
#define SPRITES_INITIAL_SIZE 200
#define MAX_IMAGE_RESOURCE 100
#define MAX_SPRITE_FRAMES 32
#define SPRITE_MAX_PATH_LENGTH 50

typedef struct Sprite {
	CP_Vector	_position;
	float		_rotation;				// in degrees
	float		_scale;					// scales both width and height
	float		_scale_x;				// scales width
	float		_scale_y;				// scales height
	float		_alpha;
	int			_flip;					// flips the sprite horizontally
	float		_width;
	float		_height;
	int			_columns;
	int			_rows;
	float		_subwidth;
	float		_subheight;
	int			_current_frame;
	int			_number_of_frames;
	float		_frame_second;			// 1/fps, Sprite_SetFPS() assigns 1/ the input fps to this value
	float		_frame_count;
	int			_image_resource;		// the sub images generated from the sprite sheet
	int			_visible;
	int			_repeat;
	int			_optOut;
} Sprite;

typedef struct Sprite_InitData {
	CP_Vector	_position;
	float		_width;
	float		_height;
	char*		_path;
	int			_col;
	int			_row;
	int			_frames;
	int			_fps;
	int			_optOut;
} Sprite_InitData;

extern Sprite* sprites;
extern int sprites_size;

extern CP_Image images[MAX_IMAGE_RESOURCE][MAX_SPRITE_FRAMES]; // shared image resource between sprites
extern int images_size;

/*____________________________________________________________________________________________________________________________________*/
// MISCELLANEOUS
/*____________________________________________________________________________________________________________________________________*/
void	Sprite_Initialize();
int		Sprite_AddSprite(const CP_Vector position, const float width, const float height, const char* path, const int col, const int row, const int frame, const int fps, const int optOut);
int		Sprite_AddSpriteF(const CP_Vector position, const float width, const float height, const char* path, const int col, const int row, const int frame, const float fps, const int optOut);
int		Sprite_AddSpriteRepeatAuto(const CP_Vector position, const float width, const float height, const int image);
int		Sprite_AddSpriteRepeatManual(const CP_Vector position, const float width, const float height, const int image, const int col, const int row, const int frame, const int fps, const int optOut);
int		Sprite_AddSpriteInitData(const Sprite_InitData data);
void	Sprite_RenderSprite(const float dt, const int id);
void	Sprite_Render(const float dt);
CP_Image Sprite_GenerateSubImage(const float u0, const float v0, const float u1, const float v1, const CP_Image img); // generate subimages to allow individual processing of frames
void	Sprite_Free();

/*____________________________________________________________________________________________________________________________________*/
// "CLASS" FUNCTIONS
/*____________________________________________________________________________________________________________________________________*/
void Sprite_SetPosition(const int id, const CP_Vector pos);
void Sprite_SetRotation(const int id, const float rot);
void Sprite_SetScale(const int id, const float scale);
void Sprite_SetScaleX(const int id, const float scalex);
void Sprite_SetScaleY(const int id, const float scaley);
void Sprite_SetAlpha(const int id, const float alpha);
void Sprite_SetFlip(const int id, const int flip);
void Sprite_SetWidth(const int id, const float width);
void Sprite_SetHeight(const int id, const float height);
void Sprite_SetFPS(const int id, const int fps);
void Sprite_SetVisible(const int id, const int visible);
void Sprite_SetRepeat(const int id, const int repeat);

void Sprite_OptOut(const int id , int bool_01);
void Sprite_Reset(const int id); // resets all counter based variables of sprite, i.e. as if just initialized

CP_Vector	Sprite_GetPosition(const int id);
float		Sprite_GetRotation(const int id);
float		Sprite_GetScale(const int id);
float		Sprite_GetScaleX(const int id);
float		Sprite_GetScaleY(const int id);
float		Sprite_GetAlpha(const int id);
int			Sprite_GetFlip(const int id);
float		Sprite_GetWidth(const int id);
float		Sprite_GetHeight(const int id);
int			Sprite_GetFPS(const int id);
int			Sprite_GetVisible(const int id);
int			Sprite_GetImageResource(const int id);
Sprite*		Sprite_GetSprite(const int id);
