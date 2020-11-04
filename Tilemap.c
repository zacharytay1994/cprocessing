#include "Tilemap.h"
#include "PhyObj.h"
#include <stdlib.h>
#include <stdio.h>

Tilemap tilemaps[MAX_TILEMAPS] = { 0 };
int tilemaps_size = 0;
CP_Image tilesets[Tilemap_Tile_Size] = { 0 };
int tilemap_debug = 0;


void Tilemap_Initialize()
{
	Tilemap_AddTileset(Tilemap_Solid, "dirt_block.png");
	Tilemap_AddTileset(Tilemap_Ground, "ground_block.png");
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
}

int Tilemap_AddTilemap(const int tileWidth, const int tileHeight, const int width, const int height)
{
	if (tilemaps_size < MAX_TILEMAPS) {
		Tilemap tilemap;
		tilemap._id = tilemaps_size;
		tilemap._tile_width = tileWidth;
		tilemap._tile_height = tileHeight;
		tilemap._width = width;
		tilemap._height = height;

		// initialize array of tiles
		int size = width * height;
		tilemap._tiles = malloc((unsigned long long)size * sizeof(int));
		for (int i = 0; i < size; i++) {
			tilemap._tiles[i] = 0;
		}
		tilemaps[tilemaps_size] = tilemap;
		tilemaps_size++;
		return tilemaps_size - 1;
	}
	return -1;
}

void Tilemap_AddTileset(const int tile, const char* path)
{
	if (tile < MAX_TILESET) {
		tilesets[tile] = CP_Image_Load(path);
	}
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
	CP_Vector tile_position = CP_Vector_Set(-1.0f, -1.0f);
	if (id < tilemaps_size) {
		for (int y = 0; y < tilemaps[id]._height; y++) {
			for (int x = 0; x < tilemaps[id]._width; x++) {
				int tile;
				if ((tile = tilemaps[id]._tiles[y * tilemaps[id]._width + x]) != 0) {
					tile_position.x = (float)(x * tilemaps[id]._tile_width) + half_tile_width;
					tile_position.y = (float)(y * tilemaps[id]._tile_height) + half_tile_height;
					tile_position = CP_Vector_MatrixMultiply(cam, tile_position);
					CP_Image_DrawAdvanced(tilesets[tile], tile_position.x, tile_position.y,
						(float)tilemaps[id]._tile_width, (float)tilemaps[id]._tile_height, 255, 0.0f);
				}
			}
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
			if (tilemaps[id]._tiles[y * width + x] == 1) {
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

void Tilemap_TxtLoad256(const char* file)
{
	FILE* f;
	fopen_s(&f, file, "r");
	int start_reading = 0;
	int read = 0;
	char c;
	int width = 0;
	int height = 0;
	int tilemap = -1;
	int x = 0;
	int y = 0;
	if (f) {
		while ((c=(char)fgetc(f)) != EOF) {
			if (read && tilemap) {
				if (c != '\n') {
					Tilemap_SetTile(tilemap, x++, y, (int)(c - 'a'));
					if (x >= width) {
						x = 0;
						y++;
					}
				}
			}
			else if (start_reading == 0) {
				width = (int)c;
				start_reading++;
			}
			else if (start_reading == 1) {
				if (c != '\n') {
					height = (int)c;
					start_reading++;
					read = 1;
					// create new tilemap
					tilemap = Tilemap_AddTilemap(64, 64, width, height);
				}
			}
		}
		fclose(f);
	}
	else {
		printf("Tilemap_TxtLoad :: Could not open file to load!\n");
	}
}
