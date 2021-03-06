//---------------------------------------------------------
// file:	Sprite.c
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
#include "Sprite.h"
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>

Sprite* sprites;
int sprites_size = 0;
int sprites_max_size;
CP_Image images[MAX_IMAGE_RESOURCE][MAX_SPRITE_FRAMES] = { 0 };
int images_size = 0;

CP_Image images_v2[MAX_IMAGE_RESOURCE] = { 0 };
int Sprite_images_v2_size = 0;
//char images_path[MAX_IMAGE_RESOURCE][SPRITE_MAX_PATH_LENGTH];
//int Sprite_path_size = 0;

void Sprite_Initialize()
{
	sprites = (Sprite*)malloc(SPRITES_INITIAL_SIZE * sizeof(Sprite));
	sprites_max_size = SPRITES_INITIAL_SIZE;
}

int Sprite_AddSprite(const CP_Vector position, const float width, const float height, const char* path, const int col, const int row, const int frame, const int fps, const int optOut)
{
	return Sprite_AddSpriteF(position,width,height,path,col,row,frame,(float)fps,optOut);
}

int Sprite_AddSpriteF(const CP_Vector position, const float width, const float height, const char* path, const int col, const int row, const int frame, const float fps, const int optOut)
{
	if (images_size >= MAX_IMAGE_RESOURCE) {
		printf("Sprite::MAX_IMAGE_RESOURCE exceeded!");
		return -1;
	}
	/*if (Sprite_images_v2_size >= MAX_IMAGE_RESOURCE) {
		printf("Sprite::MAX_IMAGE_RESOURCE exceeded!");
		return -1;
	}*/
	//images_v2[Sprite_images_v2_size++] = CP_Image_Load(path);
	//CP_Image* image_array = malloc(frame * sizeof(CP_Image));
	CP_Image sheet = CP_Image_Load(path);
	// generate sub images - maybe temp solution
	int frame_count = 0;
	float percent_width = 1.0f / (float)col;
	float percent_height = 1.0f / (float)row;
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			if (frame_count < frame) {
				float u0 = percent_width * (float)x;
				float v0 = percent_height * (float)y;
				float u1 = percent_width * (float)x + percent_width;
				float v1 = percent_height * (float)y + percent_height;
				u0 = u0 < 0.0f ? 0.0f : u0;
				u0 = u0 > 1.0f ? 1.0f : u0;
				v0 = v0 < 0.0f ? 0.0f : v0;
				v0 = v0 > 1.0f ? 1.0f : v0;
				u1 = u1 < 0.0f ? 0.0f : u1;
				u1 = u1 > 1.0f ? 1.0f : u1;
				v1 = v1 < 0.0f ? 0.0f : v1;
				v1 = v1 > 1.0f ? 1.0f : v1;
				images[images_size][frame_count] = Sprite_GenerateSubImage(u0, v0, u1, v1, sheet);
				frame_count++;
			}
			else { // end of frames, end generating subimages
				break;
			}
		}
	}
	if (sprites_size >= sprites_max_size) {
		Sprite* check = (Sprite*)realloc(sprites, sizeof(Sprite) * sprites_max_size * 2);
		sprites_max_size *= 2;
		if (check != NULL) {
			sprites = check;
		}
		else {
			printf("Sprite_AddSprite :: realloc returned null");
		}
	}
	sprites[sprites_size++] = (Sprite){ position,0.0f,1.0f,1.0f,1.0f,255,0,width,height,col,row,1.0f / (float)col,1.0f / (float)row,1,frame,1.0f / (float)fps,0.0f,images_size,1,1,optOut };
	//sprites[sprites_size++] = (Sprite){ position,0.0f,1.0f,1.0f,1.0f,255,0,width,height,col,row,1.0f / (float)col,1.0f / (float)row,1,frame,1.0f / (float)fps,0.0f,Sprite_images_v2_size-1,1,1,optOut };
	++images_size;
	return sprites_size - 1;
}

int Sprite_AddSpriteRepeatAuto(const CP_Vector position, const float width, const float height, const int sprite)
{
	if (sprite >= sprites_size) {
		printf("Sprite::sprites_size exceeded!");
		return -1;
	}
	if (images_size >= MAX_IMAGE_RESOURCE) {
		printf("Sprite::MAX_IMAGE_RESORUCE exceeded!");
		return -1;
	}
	if (sprites_size >= sprites_max_size) {
		Sprite* check = (Sprite*)realloc(sprites, sizeof(Sprite) * sprites_max_size * 2);
		sprites_max_size *= 2;
		if (check != NULL) {
			sprites = check;
		}
		else {
			printf("Sprite_AddSprite :: realloc returned null");
		}
	}
	sprites[sprites_size++] = (Sprite){ position,0.0f,1.0f,1.0f,1.0f,255,0,width,height,sprites[sprite]._columns,sprites[sprite]._rows,sprites[sprite]._subwidth,sprites[sprite]._subheight,1,sprites[sprite]._number_of_frames,sprites[sprite]._frame_second,0.0f,sprites[sprite]._image_resource,1,1,sprites[sprite]._optOut };
	return sprites_size - 1;
}

int Sprite_AddSpriteRepeatManual(const CP_Vector position, const float width, const float height, const int sprite, const int col, const int row, const int frame, const int fps, const int optOut)
{
	if (images_size >= MAX_IMAGE_RESOURCE) {
		printf("Sprite::MAX_IMAGE_RESORUCE exceeded!");
		return -1;
	}
	if (sprites_size >= sprites_max_size) {
		Sprite* check = (Sprite*)realloc(sprites, sizeof(Sprite) * sprites_max_size * 2);
		sprites_max_size *= 2;
		if (check != NULL) {
			sprites = check;
		}
		else {
			printf("Sprite_AddSprite :: realloc returned null");
		}
	}
	sprites[sprites_size++] = (Sprite){ position,0.0f,1.0f,1.0f,1.0f,255,0,width,height,col,row,1.0f / (float)col,1.0f / (float)row,1,frame,1.0f / (float)fps,0.0f,sprites[sprite]._image_resource,1,1,optOut };
	return sprites_size - 1;
}

int Sprite_AddSpriteInitData(const Sprite_InitData data)
{
	return Sprite_AddSprite(data._position, data._width, data._height, data._path, data._col, data._row, data._frames, data._fps, data._optOut);
}

void Sprite_RenderSprite(const float dt, const int id)
{
	float y, x;
	// if frame up
	if ((sprites[id]._frame_count += dt) > sprites[id]._frame_second) {
		// else switch frame
		sprites[id]._frame_count = 0.0f;
		if (sprites[id]._repeat) {
			sprites[id]._current_frame = sprites[id]._current_frame < sprites[id]._number_of_frames ? ++sprites[id]._current_frame : 1;
		}
		else {
			if (sprites[id]._current_frame < sprites[id]._number_of_frames) {
				++sprites[id]._current_frame;
			}
		}
	}
	y = (float)((sprites[id]._current_frame - 1) / sprites[id]._columns) * sprites[id]._subheight;
	x = (float)((sprites[id]._current_frame - 1) % sprites[id]._rows) * sprites[id]._subwidth;

	// its ok hacked a solution
	float width = sprites[id]._flip ? -sprites[id]._width : sprites[id]._width;
	width *= sprites[id]._scale * sprites[id]._scale_x;
	float height = sprites[id]._height * sprites[id]._scale * sprites[id]._scale_y;
	CP_Vector cam_translated_pos = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), sprites[id]._position);
	
	//CP_Image_Draw(images[sprites[id]._image_resource][0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), 100.0f, 100.0f, 255);
	////CP_Image_DrawSubImage()
	//printf("id:%d\n", sprites[id]._image_resource);
	//printf("sprites_size%d", sprites_size);

	CP_Image_DrawAdvanced(images[sprites[id]._image_resource][sprites[id]._current_frame-1], cam_translated_pos.x, cam_translated_pos.y,
		width, height, (int)sprites[id]._alpha, sprites[id]._rotation);
	/*CP_Image_DrawAdvanced(images_v2[sprites[id]._image_resource], cam_translated_pos.x, cam_translated_pos.y,
		width, height, sprites[id]._alpha, sprites[id]._rotation);*/
}

void Sprite_Render(const float dt)
{
	for (int i = 0; i < sprites_size; i++) {
		if (sprites[i]._visible && !sprites[i]._optOut) {
			Sprite_RenderSprite(dt, i);
		}
	}
}

CP_Image Sprite_GenerateSubImage(const float u0, const float v0, const float u1, const float v1, const CP_Image img)
{
	int pixel_width = CP_Image_GetWidth(img);
	int pixel_height = CP_Image_GetHeight(img);
	int size = CP_Image_GetPixelBufferSize(img);
	unsigned char* buffer = malloc(size);
	CP_Image_GetPixelData(img, buffer);
	int x_start = (int)(u0 * pixel_width);
	int x_end = (int)(u1 * pixel_width);
	int y_start = (int)(v0 * pixel_height);
	int y_end = (int)(v1 * pixel_height);
	int sub_width = (x_end - x_start);
	int sub_height = (y_end - y_start);
	unsigned char* sub_buffer = malloc(size);
	if (!buffer || !sub_buffer) {
		printf("Buffers not allocated.");
	}
	// create the sub image buffer
	for (int y = 0; y < sub_height; y++) {
		for (int x = 0; x < sub_width; x++) {
			int buffer_x = (x_start + x) * 4;
			int buffer_y = (y_start + y);
			if (buffer && sub_buffer) {
				sub_buffer[(y * sub_width + x) * 4] = buffer[buffer_y * (pixel_width * 4) + buffer_x];			// r
				sub_buffer[(y * sub_width + x) * 4 + 1] = buffer[buffer_y * (pixel_width * 4) + buffer_x + 1];	// g
				sub_buffer[(y * sub_width + x) * 4 + 2] = buffer[buffer_y * (pixel_width * 4) + buffer_x + 2];	// b
				sub_buffer[(y * sub_width + x) * 4 + 3] = buffer[buffer_y * (pixel_width * 4) + buffer_x + 3];	// a
			}
		}
	}
	CP_Image image = CP_Image_CreateFromData(sub_width, sub_height, sub_buffer);
	free(buffer);
	free(sub_buffer);
	return image;
}

void Sprite_Free()
{
	// free up resources
	free(sprites);
}

void Sprite_SetFlip(const int id, const int flip)															
{
	if (id < sprites_size) {
		sprites[id]._flip = flip;
	}
}

void Sprite_SetWidth(const int id, const float width)
{
	if (id < sprites_size) {
		sprites[id]._width = width;
	}
}

void Sprite_SetHeight(const int id, const float height)
{
	if (id < sprites_size) {
		sprites[id]._height = height;
	}
}

void Sprite_SetFPS(const int id, const int fps)
{
	if (id < sprites_size) {
		sprites[id]._frame_second = 1.0f / (float)fps;
	}
}

void Sprite_SetVisible(const int id, const int visible)
{
	if (id < sprites_size) {
		sprites[id]._visible = visible;
	}
}

void Sprite_SetRepeat(const int id, const int repeat)
{
	if (id < sprites_size) {
		sprites[id]._repeat = repeat;
	}
}

void Sprite_OptOut(const int id, int bool_01)
{
	if (id < sprites_size || id > 0 )
	{
		sprites[id]._optOut = bool_01;
	}
}



void Sprite_Reset(const int id)
{
	if (id < sprites_size) {
		sprites[id]._current_frame = 1;
		sprites[id]._frame_count = 0.0f;
	}
}

CP_Vector Sprite_GetPosition(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._position;
	}
	return CP_Vector_Set(-1.0f, -1.0f);
}

float Sprite_GetRotation(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._rotation;
	}
	return -1.0f;
}

float Sprite_GetScale(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._scale;
	}
	return -1.0f;
}

float Sprite_GetScaleX(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._scale_x;
	}
	return -1.0f;
}

float Sprite_GetScaleY(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._scale_y;
	}
	return -1.0f;
}

float Sprite_GetAlpha(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._alpha;
	}
	return -1;
}

int Sprite_GetFlip(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._flip;
	}
	return -1;
}

float Sprite_GetWidth(const int id)
{
	if (id < sprites_size) {
		sprites[id]._width;
	}
	return -1.0f;
}

float Sprite_GetHeight(const int id)
{
	if (id < sprites_size) {
		sprites[id]._height;
	}
	return -1.0f;
}

int Sprite_GetFPS(const int id)
{
	if (id < sprites_size) {
		return (int)(1.0f / sprites[id]._frame_second);
	}
	return -1;
}

int Sprite_GetVisible(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._visible;
	}
	return -1;
}

int Sprite_GetImageResource(const int id)
{
	if (id < sprites_size) {
		return sprites[id]._image_resource;
	}
	return -1;
}

Sprite* Sprite_GetSprite(const int id)
{
	if (id < sprites_size) {
		return &(sprites[id]);
	}
	return NULL;
}

void Sprite_SetPosition(const int id, const CP_Vector pos)
{
	if (id < sprites_size) {
		sprites[id]._position = pos;
	}
}

void Sprite_SetRotation(const int id, const float rot)
{
	if (id < sprites_size) {
		sprites[id]._rotation = rot;
	}
}

void Sprite_SetScale(const int id, const float scale)
{
	if (id < sprites_size) {
		sprites[id]._scale = scale;
	}
}

void Sprite_SetScaleX(const int id, const float scalex)
{
	if (id < sprites_size) {
		sprites[id]._scale_x = scalex;
	}
}

void Sprite_SetScaleY(const int id, const float scaley)
{
	if (id < sprites_size) {
		sprites[id]._scale_y = scaley;
	}
}

void Sprite_SetAlpha(const int id, const float alpha)
{
	if (id < sprites_size) {
		sprites[id]._alpha = alpha;
	}
}
