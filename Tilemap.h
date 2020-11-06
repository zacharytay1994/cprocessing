#pragma once

#include "CProcessing/inc/cprocessing.h"

#define MAX_TILEMAPS 5
#define MAX_TILESET 16
#define MAX_TILESHEETS 16
#define MAX_TILESHEET_CELLS 32

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
	int* _tiles; // contains all tiles on the grid
	int _offset_x;
	int _offset_y;

} Tilemap;

typedef struct Tilemap_TileSheet {
	int			_id;
	int			_size;
	CP_Image	_images[MAX_TILESHEET_CELLS];
} Tilemap_TileSheet;

extern Tilemap		tilemaps[MAX_TILEMAPS];
extern int			tilemaps_size;
extern CP_Image		tilesets[Tilemap_Tile_Size];
extern int			tilemap_debug;

void		Tilemap_Initialize();
void		Tilemap_Debug_Render(const int id, const CP_Matrix cam);
int			Tilemap_AddTilemap(const int tileWidth, const int tileHeight, const int width, const int height, const int offsetx, const int offsety);
void		Tilemap_AddTileset(const int tile, const char* path);
int			Tilemap_LoadTileSheet(const char* path, const int row, const int col, const int frames);
int			Tilemap_GetTile(const int id, const int x, const int y);
void		Tilemap_SetTile(const int id, const int x, const int y, const int tile);
void		Tilemap_SetTileToBrush(const int id, const int x, const int y);
CP_Vector	Tilemap_WorldToGrid(const int id, const float x, const float y);
void		Tilemap_Render(const int id, const CP_Matrix cam);
void		Tilemap_Free();
void		Tilemap_GeneratePhyObjs(const int id);
void		Tilemap_SetActiveTileSheet(const int id);
void		Tilemap_SetTileSheetBrush(const int id);

void		Tilemap_TxtSave256(const int id, const char* file);		// only saves tilemaps of <= 256 per dimensions
int			Tilemap_TxtLoad256(const char* file);					// only saves tilemaps of <= 256 per dimensions