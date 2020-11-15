#pragma once

// NODE STUFF
void AStar_Node_CreateNode(const int isWalkable, const int x, const int y);
int AStar_Node_GetFCost(const int id);
int AStar_Node_Reset(const int id);