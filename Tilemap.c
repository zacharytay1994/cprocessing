#include "Tilemap.h"
#include "PhyObj.h"
#include "Sprite.h"
#include "Camera.h"
#include "LightStage.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

Tilemap tilemaps[MAX_TILEMAPS] = { 0 };
int tilemaps_size = 0;
int tilemap_debug = 0;
Tilemap_TileSheet tilesheets[MAX_TILESHEETS] = { 0 };
int tilesheets_size = 0;
int tilemap_active_tilesheet = 0;
int tilemap_active_tilesheet_cell = 0;

CP_Image tilemap_ground_normal_map;
void* tilemap_ground_normal_map_data;
int tilemap_tile_x = 40, tilemap_tile_y = 40;
void* tilemap_tile_data;
CP_Vector tilemap_test_light_position;
int tilemap_test_light;
float tilemap_ambient = 1.0f;
CP_Image tilemap_ground;
void* tilemap_og_data;
void** tilemap_tile_data_array;
CP_Image* tilemap_image_array;
int normal_initialized = 0;
int normal_count = 0;

CP_Image tilemap_highlight_cursor;
CP_Vector tilemap_highlight_position;

void Tilemap_Initialize()
{
	// loading tilesheets
	Tilemap_SetActiveTileSheet(Tilemap_LoadTileSheet("./Sprites/ground.png", 1, 1, 1));
	tilemap_ground_normal_map = CP_Image_Load("./Sprites/ground_normal_map.png");
	tilemap_ground = CP_Image_Load("./Sprites/ground.png");
	printf("%d\n", CP_Image_GetPixelBufferSize(tilemap_ground_normal_map));
	tilemap_ground_normal_map_data = malloc(CP_Image_GetPixelBufferSize(tilemap_ground_normal_map));
	tilemap_tile_data = malloc(CP_Image_GetPixelBufferSize(tilemap_ground_normal_map));
	tilemap_og_data = malloc(CP_Image_GetPixelBufferSize(tilemap_ground_normal_map));
	CP_Image_GetPixelData(tilemap_ground_normal_map, (unsigned char*)tilemap_ground_normal_map_data);
	CP_Image_GetPixelData(tilesheets[tilemap_active_tilesheet]._images[0], (unsigned char*)tilemap_tile_data);
	CP_Image_GetPixelData(tilemap_ground, (unsigned char*)tilemap_og_data);
	tilemap_test_light = LightStage_AddLight(CP_Vector_Set(0.0f,0.0f), 800.0f, 800.0f, -1.0f, 0, 100);
	//LightStage_SetPosition(tilemap_test_light, (CP_Vector) { 1400.0f, 0.0f });

	tilemap_highlight_cursor = CP_Image_Load("./Sprites/cursor.png");
	tilemap_highlight_position = (CP_Vector){ 0.0f, 0.0f };
}

void Tilemap_Debug_Render(const int id, const CP_Matrix cam)
{
	// check if id is valid 
	if (id >= tilemaps_size) {
		return;
	}
	// find how many horizontal grid lines needed
	int grid_width = (CP_System_GetWindowWidth() / tilemaps[id]._tile_width);
	int grid_height = (CP_System_GetWindowHeight() / tilemaps[id]._tile_height);

	int tile_width = tilemaps[id]._tile_width;
	int tile_height = tilemaps[id]._tile_height;

	float screen_width = (float)CP_System_GetWindowWidth();
	float screen_height = (float)CP_System_GetWindowHeight();
	// render horizontal lines
	for (int x = 0; x < grid_width+1; x++) {
		CP_Vector start = { (float)(x * tile_width), 0.0f };
		// transform based on camera
		start = CP_Vector_MatrixMultiply(cam, start);
		// wrap around
		if (start.x > screen_width) {
			start.x -= (grid_width + 1) * tile_width;
		}
		else if (start.x < 0.0f) {
			start.x += (grid_width + 1) * tile_width;
		}
		// render lines
		CP_Graphics_DrawLine(start.x, 0.0f, start.x, screen_height);
	}
	// render vertical lines
	for (int y = 0; y < grid_height+1; y++) {
		CP_Vector start = { 0.0f, (float)(y * tile_height) };
		// transform based on camera
		start = CP_Vector_MatrixMultiply(cam, start);
		// wrap around
		if (start.y > screen_height) {
			start.y -= (grid_height + 1) * tile_height;
		}
		else if (start.y < 0.0f) {
			start.y += (grid_height + 1) * tile_height;
		}
		// render lines
		CP_Graphics_DrawLine(0.0f, start.y, screen_width, start.y);
	}
	// render red lines
	float area_width = (float)(tilemaps[id]._width * tile_width);
	float area_height = (float)(tilemaps[id]._height * tile_height);
	CP_Vector top_left = CP_Vector_Set(0.0f, 0.0f);
	CP_Vector top_right = CP_Vector_Set(area_width, 0.0f);
	CP_Vector bottom_left = CP_Vector_Set(0.0f, area_height);
	CP_Vector bottom_right = CP_Vector_Set(area_width, area_height);
	// apply cam transform
	top_left = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), top_left);
	top_right = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), top_right);
	bottom_left = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), bottom_left);
	bottom_right = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), bottom_right);
	CP_Settings_Stroke((CP_Color) { 255, 0, 0, 255 });
	CP_Graphics_DrawLine(top_left.x, top_left.y, top_right.x, top_right.y);
	CP_Graphics_DrawLine(top_right.x, top_right.y, bottom_right.x, bottom_right.y);
	CP_Graphics_DrawLine(bottom_right.x, bottom_right.y, bottom_left.x, bottom_left.y);
	CP_Graphics_DrawLine(bottom_left.x, bottom_left.y, top_left.x, top_left.y);
	CP_Settings_Stroke((CP_Color) { 0, 0, 0, 255 });
}

int Tilemap_AddTilemap(const int tileWidth, const int tileHeight, const int width, const int height, const int offsetx, const int offsety)
{
	if (tilemaps_size < MAX_TILEMAPS) {
		Tilemap tilemap;
		tilemap._id = tilemaps_size;
		tilemap._tile_width = tileWidth;
		tilemap._tile_height = tileHeight;
		tilemap._width = width;
		tilemap._height = height;
		tilemap._offset_x = offsetx;
		tilemap._offset_y = offsety;

// initialize array of tiles
int size = width * height;
tilemap._tiles = malloc((unsigned long long)size * sizeof(int));
for (int i = 0; i < size; i++) {
	tilemap._tiles[i] = -1;
}
tilemaps[tilemaps_size] = tilemap;
tilemaps_size++;
return tilemaps_size - 1;
	}
	return -1;
}

int Tilemap_LoadTileSheet(const char* path, const int row, const int col, const int frames)
{
	if (tilesheets_size < MAX_TILESHEETS) {
		tilesheets[tilesheets_size] = (Tilemap_TileSheet){ tilesheets_size,frames,{0} };
	}
	else {
		printf("Tilemap_LoadTileSheet :: Amount of tilesheets exceeded!");
		return -1;
	}
	CP_Image sheet = CP_Image_Load(path);
	// generate sub images - maybe temp solution
	int frame_count = 0;
	float percent_width = 1.0f / (float)col;
	float percent_height = 1.0f / (float)row;
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			if (frame_count < frames) {
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
				tilesheets[tilesheets_size]._images[frame_count] = Sprite_GenerateSubImage(u0, v0, u1, v1, sheet);
				frame_count++;
			}
			else { // end of frames, end generating subimages
				break;
			}
		}
	}
	tilesheets_size++;
	return tilesheets_size - 1;
}

int Tilemap_GetTile(const int id, const int x, const int y)
{
	if (id < tilemaps_size && x < tilemaps[id]._width && y < tilemaps[id]._height) {
		if (x < tilemaps[id]._width && y < tilemaps[id]._height) {
			return tilemaps[id]._tiles[y * tilemaps[id]._width + x];
		}
	}
	return -1;
}

void Tilemap_SetTile(const int id, const int x, const int y, const int tile)
{
	if (id < tilemaps_size && x < tilemaps[id]._width && y < tilemaps[id]._height) {
		tilemaps[id]._tiles[y * tilemaps[id]._width + x] = tile;
	}
}

void Tilemap_SetTileToBrush(const int id, const int x, const int y)
{
	if (id < tilemaps_size && x < tilemaps[id]._width && y < tilemaps[id]._height) {
		tilemaps[id]._tiles[y * tilemaps[id]._width + x] = tilemap_active_tilesheet_cell;
	}
}

void Tilemap_HighlightMouseTile(const int id)
{
	if (id < tilemaps_size) {
		CP_Vector mouse = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		//mouse = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), mouse);
		tilemap_highlight_position = Tilemap_WorldToGrid(id, mouse.x, mouse.y);
		mouse.x = tilemaps[id]._tile_width * tilemap_highlight_position.x + ((float)tilemaps[id]._tile_width / 2.0f);
		mouse.y = tilemaps[id]._tile_height * tilemap_highlight_position.y + ((float)tilemaps[id]._tile_height / 2.0f);
		mouse = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), mouse);
		CP_Image_Draw(tilemap_highlight_cursor, mouse.x,
			mouse.y, (float)tilemaps[id]._tile_width, (float)tilemaps[id]._tile_width, 20);
	}
}

int Tilemap_GetValidGroundTiles(const int id, CP_Vector* container)
{
	int i = 0;
	if (id < tilemaps_size) {
		for (int y = tilemaps[id]._height - 1; y >= 0; y--) {
			for (int x = 0; x < tilemaps[id]._width; x++) {
				// check if cell is empty
				if (tilemaps[id]._tiles[y * tilemaps[id]._width + x] == -1) {
					// bottom has thing
					if (y + 1 < tilemaps[id]._height) {
						if (tilemaps[id]._tiles[(y + 1) * tilemaps[id]._width + x] != -1) {
							container[i++] = (CP_Vector){ (float)x,(float)y };
						}
					}
				}
			}
		}
	}
	return i;
}

CP_Vector Tilemap_WorldToGrid(const int id, const float x, const float y)
{
	if (id < tilemaps_size && x >= 0.0f && y >= 0.0f) {
		return (CP_Vector){ (float)((int)(x / tilemaps[id]._tile_width)), (float)((int)(y / tilemaps[id]._tile_height)) };
	}
	return CP_Vector_Set(-1.0f, -1.0f);
}

void Tilemap_Render(const int id, const CP_Matrix cam)
{
	if (id >= tilemaps_size) {
		return;
	}
	float half_tile_width = (float)tilemaps[id]._tile_width / 2.0f;
	float half_tile_height = (float)tilemaps[id]._tile_height / 2.0f;
	float twice_offset_x = (float)(tilemaps[id]._offset_x * 2);
	float twice_offset_y = (float)(tilemaps[id]._offset_y * 2);
	float half_offset_x = (float)tilemaps[id]._offset_x / 2.0f;
	float half_offset_y = (float)tilemaps[id]._offset_y / 2.0f;
	CP_Vector tile_position = CP_Vector_Set(-1.0f, -1.0f);
	int count = 0;
	if (id < tilemaps_size) {
		for (int y = tilemaps[id]._height-1; y >= 0; y--) {
			for (int x = 0; x < tilemaps[id]._width; x++) {
				int tile;
				if ((tile = tilemaps[id]._tiles[y * tilemaps[id]._width + x]) != -1) {
					tile_position.x = (float)(x * tilemaps[id]._tile_width) + half_tile_width;
					tile_position.y = (float)(y * tilemaps[id]._tile_height) + half_tile_height;
					if (CP_Input_KeyDown(KEY_P)) {
						printf("TILEMAPX: %f, TILEMAPY: %f\n", tile_position.x, tile_position.y);
					}
					if (normal_initialized) {
						CP_Vector* pos = LightStage_GetLightPositionsArray();
						int pos_size = LightStage_GetLightPositionsSize();
						LightStage_ApplyNormalMap(tilemap_tile_data_array[count], tilemap_ground_normal_map_data,
							tilemap_og_data, tile_position, (float)tilemaps[id]._tile_width, (float)tilemaps[id]._tile_height,
							10.0f,10.0f,pos, pos_size);
						CP_Image_UpdatePixelData(tilemap_image_array[count], (unsigned char*)(tilemap_tile_data_array[count]));
						tile_position = CP_Vector_MatrixMultiply(cam, tile_position);
						if (CP_Input_KeyDown(KEY_L)) {
							printf("--------------------------------------- %d Start Entry ---------------------------------------\n", count);
							for (int i = 0; i < 100; ++i) {
								printf("R:%-4d,G:%-4d,B:%-4d,A:%-4d |", ((unsigned char*)(tilemap_tile_data_array[count]))[i * 4],
									((unsigned char*)(tilemap_tile_data_array[count]))[i * 4 + 1],
									((unsigned char*)(tilemap_tile_data_array[count]))[i * 4 + 2],
									((unsigned char*)(tilemap_tile_data_array[count]))[i * 4 + 3]);
								if (i % 10 == 0) {
									printf("\n");
								}
							}
							printf("\n");
						}
						CP_Image_DrawAdvanced(tilemap_image_array[count], tile_position.x - half_offset_x, tile_position.y - half_offset_y,
							(float)tilemaps[id]._tile_width + twice_offset_x, (float)tilemaps[id]._tile_height + twice_offset_y, 255, 0.0f);
					}
					++count;
				}
			}
		}
	}
	// initialize normal data
	if (!normal_initialized) {
		tilemap_tile_data_array = malloc(count * sizeof(void*));
		tilemap_image_array = (CP_Image*)malloc(count * sizeof(CP_Image));
		normal_count = count;
		if (tilemap_tile_data_array && tilemap_image_array) {
			for (int i = 0; i < count; ++i) {
				tilemap_tile_data_array[i] = malloc(400);
				tilemap_image_array[i] = CP_Image_CreateFromData(10,10, tilemap_og_data);
			}
			normal_initialized = 1;
		}
	}
}

void Tilemap_Free()
{
	for (int i = 0; i < tilemaps_size; i++) {
		if (tilemaps[i]._tiles != NULL) {
			free(tilemaps[i]._tiles); // ??
		}
	}
	free(tilemap_ground_normal_map_data);
	free(tilemap_tile_data);
	free(tilemap_og_data);
	for (int i = 0; i < normal_count; ++i) {
		free(tilemap_tile_data_array[i]);
	}
	free(tilemap_tile_data_array);
	free(tilemap_image_array);
}

void Tilemap_GeneratePhyObjs(const int id)
{
	if (!(id < tilemaps_size)) {
		return;
	}
	int width = tilemaps[id]._width;
	int height = tilemaps[id]._height;
	int tile_width = tilemaps[id]._tile_width;
	int tile_height = tilemaps[id]._tile_height;

	int new_row = 0;
	int row_count = 0;
	float center_x = (float)tile_width / 2.0f;
	float center_y = (float)tile_height / 2.0f;
	float box_width = (float)tile_width;
	float box_height = (float)tile_height;

	for (int y = 0; y < height; ++y) {
		center_y = (float)(y * tile_height) + (float)(tile_height / 2.0f);
		for (int x = 0; x < width; ++x) { // if tile in tilemap is solid
			if (tilemaps[id]._tiles[y * width + x] >= 0) {
				if (!new_row) {
					new_row = 1; 
				}
				++row_count;
				// calculate new center_x and new width
				center_x = (float)((x+1) * tile_width) - (float)(row_count * tile_width) / 2.0f;
				box_width = (float)(row_count * tile_width);
			}
			else {
				if (new_row) {
					new_row = 0;
					row_count = 0;
					// create new static box
					PhyObj_AddAABox(center_x, center_y, 0.0f, box_width/2.0f, box_height/2.0f, 0.6f);
				}
			}
			if (x == width - 1 && new_row) {
				new_row = 0;
				row_count = 0;
				// create new static box
				PhyObj_AddAABox(center_x, center_y, 0.0f, box_width / 2.0f, box_height / 2.0f, 0.6f);
			}
		}
	}
}

void Tilemap_SetActiveTileSheet(const int id)
{
	if (id < tilesheets_size) {
		tilemap_active_tilesheet = id;
	}
}

void Tilemap_SetTileSheetBrush(const int id)
{
	if (id < MAX_TILESHEET_CELLS) {
		tilemap_active_tilesheet_cell = id;
	}
}

void Tilemap_TxtSave256(const int id, const char* file)
{
	if (id < tilemaps_size) {
		FILE* f;
		fopen_s(&f, file, "w");
		if (f) {
			int width = tilemaps[id]._width;
			int height = tilemaps[id]._height;
			fputc((char)width, f);
			fputc('\n', f);
			fputc((char)height, f);
			fputc('\n', f);
			fputc((char)(tilemaps[id]._offset_x+'A'), f);
			fputc('\n', f);
			fputc((char)(tilemaps[id]._offset_y+'A'), f);
			fputc('\n', f);
			fputc((char)tilemaps[id]._tile_width, f);
			fputc('\n', f);
			fputc((char)tilemaps[id]._tile_height, f);
			fputc('\n', f);

			// write width and height at top of file
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					fputc((char)(tilemaps[id]._tiles[y * width + x] + 'a'), f);
				}
				fputc('\n', f);
			}
			fclose(f);
		}
		else {
			printf("Tilemap_TxtSave :: File not saved successfully!\n");
		}
	}
}

int Tilemap_TxtLoad256(const char* file)
{
	FILE* f;
	fopen_s(&f, file, "r");
	int start_reading = 0;
	int read = 0;
	char c;
	int width = 0;
	int height = 0;
	int tile_width = 0;
	int tile_height = 0;
	int offset_x = 0;
	int offset_y = 0;
	int tilemap = -1;
	int x = 0;
	int y = 0;
	if (f) {
		while ((c=(char)fgetc(f)) != EOF) {
			if (read && tilemap >= 0) {
				if (c != '\n') {
					Tilemap_SetTile(tilemap, x++, y, (int)(c - 'a'));
					if (x >= width) {
						x = 0;
						y++;
					}
				}
			}
			switch (start_reading) {
			case 0:
				width = (int)c;
				start_reading++;
				break;
			case 1:
				if (c != '\n' && c != '\0') {
					height = (int)c;
					start_reading++;
				}
				break;
			case 2:
				if (c != '\n' && c != '\0') {
					offset_x = (int)(c-'A');
					start_reading++;
				}
				break;
			case 3:
				if (c != '\n' && c != '\0') {
					offset_y = (int)(c-'A');
					start_reading++;
				}
				break;
			case 4:
				if (c != '\n' && c != '\0') {
					tile_width = (int)c;
					start_reading++;
				}
				break;
			case 5:
				if (c != '\n' && c != '\0') {
					tile_height = (int)c;
					start_reading++;
					read = 1;
					// create new tilemap
					tilemap = Tilemap_AddTilemap(tile_width, tile_height, width, height, offset_x, offset_y);
				}
				break;
			}
		}
		fclose(f);
	}
	else {
		printf("Tilemap_TxtLoad :: Could not open file to load!\n");
	}
	return tilemap;
}

void Tilemap_TestFunction()
{
	CP_Image_Draw(tilesheets[tilemap_active_tilesheet]._images[0], CP_Input_GetMouseX(), CP_Input_GetMouseY(), 30.0f, 30.0f, 255);
}
