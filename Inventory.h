#pragma once
#include <cprocessing.h>
#include <stdio.h>

struct inventory_item
{
	CP_Image	item_image;
	CP_Vector	item_position;
	int			item_id;
	int			slot_id;
	char		item_name[127];
	char		item_description[127];
	char		is_visible;
	char		is_dragging;
};

struct inventory_item inventory_stock[127];
float inventory_window_width;
float inventory_window_height;
float inventory_width;
float inventory_height;
float inventory_slot_width;
int inventory[64];

void Inventory_Init();
void Inventory_Open();
void Inventory_Close();
int Inventory_Add_Item(int id);
void Inventory_Update();
void Inventory_Render();

struct inventory_item Inventory_Stock_Get_Struct_By_Name(char* name);
struct inventory_item Inventory_Stock_Get_Struct_By_ID(int id);




void Inventory_Item_Create(char* name);
void Inventory_Item_Update();
void Inventory_Item_Render(int id);
void Inventory_Item_Remove();