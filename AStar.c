#include "AStar.h"
#include <stdlib.h>


// NODE STUFF
typedef struct AStar_Node {
	int _id;
	int _g_cost;
	int _h_cost;
	int _grid_x;
	int _grid_y;
	int _is_walkable;
	AStar_Node* _parent;
	int _in_closed;
	int _in_open;
} AStar_Node;

void AStar_Node_CreateNode(const int isWalkable, const int x, const int y) {
	return (AStar_Node) { 0, 0, x, y, isWalkable, NULL, 0, 0 };
}

// PATHFINDING GRID
typedef struct AStar_PathfindingGrid {
	AStar_Node* _node_grid;
	int _grid_size_x;
	int _grid_size_y;
} AStar_PathfindingGrid;

void AStar_Pathfinding_Init() {

}
