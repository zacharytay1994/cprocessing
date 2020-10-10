#pragma once

#include "CProcessing/inc/cprocessing.h"

#define MAX_TILEMAPS 5
#define MAX_TILESET 16

enum Tilemap_Tile {
	Tilemap_Empty,
	Tilemap_Solid,
	Tilemap_Ground,
	Tilemap_Tile_Size // all tiles must be added above this
};

typedef struct Tilemap {
	int _id;
	int _tile_width;
	int _tile_height;
	int _width;
	int _height;
	int* _tiles;

} Tilemap;

extern Tilemap* tilemaps;
extern int tilemaps_size;
extern CP_Image tilesets[Tilemap_Tile_Size];

void Tilemap_Initialize();
void Tilemap_Update(const float dt);
void Tilemap_Render();
// renders debug info, e.g. grid-lines etc
void Tilemap_Debug_Render(const int id, const CP_Matrix cam);

int Tilemap_AddTilemap(const int tileWidth, const int tileHeight, const int width, const int height);
void Tilemap_AddTileset(const int tile, const char* path);
int Tilemap_GetTile(const int id, const int x, const int y);
void Tilemap_SetTile(const int id, const int x, const int y, const int tile);
CP_Vector Tilemap_WorldToGrid(const int id, const float x, const float y);
void Tilemap_RenderTileGrid(const int id, const CP_Matrix cam);