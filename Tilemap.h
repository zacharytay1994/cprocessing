#pragma once

#include "CProcessing/inc/cprocessing.h"

#define MAX_TILEMAPS 5

enum Tile {
	Empty,
	Solid
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

void Tilemap_Initialize();
void Tilemap_Update(const float dt);
void Tilemap_Render();
// renders debug info, e.g. grid-lines etc
void Tilemap_Debug_Render(const int id, const CP_Matrix cam);

int Tilemap_AddTilemap(const int tileWidth, const int tileHeight, const int width, const int height);
int Tilemap_GetTile(const int id, const int x, const int y);
CP_Vector Tilemap_WorldToGrid(const int id, const float x, const float y);
int Tilemap_RenderTileGrid(const int id);