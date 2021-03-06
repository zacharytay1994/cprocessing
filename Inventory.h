/*!
@file       Inventory.h
@author     Noel Ho Sing Nam (s.ho)
@course     CSD1400
@section    A
@brief      Contains the item struct and functions for creating, adding, using, removing items within inventory
*//*__________________________________________________________________________
_*/
#pragma once
#include <cprocessing.h>
#include <stdio.h>
#include "Button.h"
#include "Player.h"

#define				BROWN	CP_Color_Create(150, 75, 0, 150)
#define			DARK_BROWN	CP_Color_Create(125, 50, 0, 150)
#define				BLUE	CP_Color_Create(0, 0, 255, 255)
#define				BLACK	CP_Color_Create(0, 0, 0, 255)
#define	TRANSLUCENT_WHITE	CP_Color_Create(255, 255, 255, 200)

struct inventory_item
{
	CP_Image	item_image;
	CP_Vector	item_position;
	int			item_id;
	char		item_name[127];
	char		item_description[127];
	char		is_visible;
};

CP_Vector inventory_position;
struct inventory_item inventory_stock[127];
float inventory_window_width;
float inventory_window_height;
float inventory_width;
float inventory_height;
float inventory_slot_width;
int inventory[64];
char inventory_is_visible;
//char is_dragging;
char hover_display[127];
char hover_display_desc[127];

void Inventory_Init();
void Inventory_Open();
void Inventory_Close();
int Inventory_Add_Item_ID(int id);
int Inventory_Add_Item_Name(char* name);
void Inventory_Update();
void Inventory_Render();
int Inventory_Visible();

struct inventory_item Inventory_Stock_Get_Struct_By_Name(char* name);
struct inventory_item Inventory_Stock_Get_Struct_By_ID(int id);
int Inventory_Item_Set_Image(char* name, char* image);
int Inventory_Item_Set_Description(char* name, char* text);

void Inventory_Item_Create(char* name);
void Inventory_Item_Update();
void Inventory_Item_Render(int id, float x, float y);
int Inventory_Item_Remove_ID(int id);
int Inventory_Item_Remove_Name(char* name);
void Inventory_Item_Use_Name(char* name);
void Inventory_Item_Use_ID(int id);