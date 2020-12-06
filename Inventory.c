/*!
@file       Inventory.c
@author     Noel Ho Sing Nam (s.ho)
@course     CSD1400
@section    A
@brief      Inventory system for the game
*//*__________________________________________________________________________
_*/
#include "Inventory.h"
#include <string.h>

/*!
@brief Initializes the Inventory class
*//*________________________________________________________________________
_*/
void Inventory_Init()
{
	inventory_window_width = (float)CP_System_GetWindowWidth();
	inventory_window_height = (float)CP_System_GetWindowHeight();

	if (inventory_window_width > inventory_window_height)
	{
		inventory_width = inventory_window_height * 0.5f;
		inventory_height = inventory_width;
	}
	else
	{
		inventory_width = inventory_window_width * 0.5f;
		inventory_height = inventory_width;
	}

	inventory_position.x = inventory_window_width * 0.5f - (inventory_width * 0.5f);
	inventory_position.y = inventory_window_height * 0.5f - (inventory_height * 0.5f);

	inventory_slot_width = inventory_width * 0.125f;

	for (int i = 0; i < 127; i++)
	{
		inventory_stock[i].item_id = -1;
		if (i < 64)
		{
			inventory[i] = -1;
		}
	}
	inventory_is_visible = 0;
	//is_dragging = NULL;
	return;
}

/*!
@brief Opens the inventory
*//*________________________________________________________________________
_*/
void Inventory_Open()
{
	inventory_is_visible = 1;
}

/*!
@brief Closes the inventory
*//*________________________________________________________________________
_*/
void Inventory_Close()
{
	inventory_is_visible = 0;
}

/*!
@brief Adds an item into inventory by ID
@param  id				- The ID of the item to add
@return the slot the item is stored, -1 if there is no space
*//*________________________________________________________________________
_*/
int Inventory_Add_Item_ID(int id)
{
	for (int i = 0; i < 64; i++)
	{
		if (inventory[i] == -1)
		{
			inventory[i] = id;
			return i;
		}
	}
	return -1;
}

/*!
@brief Adds an item into inventory by name
@param  name			- The name of the item to add
@return the slot the item is stored, -1 if there is no space
*//*________________________________________________________________________
_*/
int Inventory_Add_Item_Name(char* name)
{
	int id = Inventory_Stock_Get_Struct_By_Name(name).item_id;
	for (int i = 0; i < 64; i++)
	{
		if (inventory[i] == -1)
		{
			inventory[i] = id;
			return i;
		}
	}
	return -1;
}

/*!
@brief Update function for inventory class
*//*________________________________________________________________________
_*/
void Inventory_Update()
{
	Inventory_Item_Update();
}

/*!
@brief Renders the inventory
*//*________________________________________________________________________
_*/
void Inventory_Render()
{
	if (inventory_is_visible)
	{
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Settings_Fill(BROWN);
		CP_Graphics_DrawRect(inventory_position.x, inventory_position.y, inventory_width, inventory_height);

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				CP_Settings_Fill(DARK_BROWN);
				CP_Graphics_DrawRect(inventory_position.x + (j * inventory_slot_width) + inventory_slot_width * 0.1f,
									inventory_position.y + (i * inventory_slot_width) + inventory_slot_width * 0.1f,
									inventory_slot_width * 0.8f,
									inventory_slot_width * 0.8f);
				if (inventory[j+(i*8)] != -1)
				{
					Inventory_Item_Render(inventory[j + (i * 8)],
						inventory_position.x + (j * inventory_slot_width) + inventory_slot_width * 0.1f,
						inventory_position.y + (i * inventory_slot_width) + inventory_slot_width * 0.1f);
				}
			}
		}

		if (CP_Input_GetMouseWorldX() > inventory_position.x && CP_Input_GetMouseWorldX() < inventory_position.x + inventory_width
			&& CP_Input_GetMouseWorldY() > inventory_position.y && CP_Input_GetMouseWorldY() < inventory_position.y + inventory_height)
		{
			float longeststring = (float)strlen(hover_display_desc);
			if (strlen(hover_display) > strlen(hover_display_desc) || !strcmp(hover_display_desc, "No description"))
			{
				longeststring = (float)strlen(hover_display);
			}

			CP_Settings_Fill(TRANSLUCENT_WHITE);
			CP_Graphics_DrawRect(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY() - 28, longeststring * 18 + 10, 40 * (float)(1 + !!(strcmp(hover_display_desc, "No description"))));

			CP_Settings_Fill(BLACK);
			CP_Settings_TextSize(40);
			CP_Font_DrawText(hover_display, CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY());
			if (strlen(hover_display_desc) > 0 && strcmp(hover_display_desc, "No description"))
			{
				CP_Settings_TextSize(35);
				CP_Font_DrawText(hover_display_desc, CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY() + 30);
			}
		}
	}
}

///////// Inventory Stock
/*!
@brief Gets an item by name
@param  name			- The name of the item to get
@return the struct of the item
*//*________________________________________________________________________
_*/
struct inventory_item Inventory_Stock_Get_Struct_By_Name(char* name)
{
	for (int i = 0; i < 127; i++)
	{
		if (!strcmp(inventory_stock[i].item_name,name))
		{
			return inventory_stock[i];
		}
	}
	return inventory_stock[126];
}

/*!
@brief Gets an item by id
@param  id				- The ID of the item to get
@return the struct of the item
*//*________________________________________________________________________
_*/
struct inventory_item Inventory_Stock_Get_Struct_By_ID(int id)
{
	for (int i = 0; i < 127; i++)
	{
		if (inventory_stock[i].item_id == id)
		{
			return inventory_stock[i];
		}
	}

	return inventory_stock[126];
}

///////// Inventory Item
/*!
@brief Creates a new item
@param  name			- The name of the item to create
*//*________________________________________________________________________
_*/
void Inventory_Item_Create(char* name)
{
	struct inventory_item new_item;

	// Initialize Image
	new_item.item_image = NULL;

	// Initialize position
	new_item.item_position.x = 0;
	new_item.item_position.y = 0;

	// Set name
	sprintf_s(new_item.item_name, 127, name);
	
	// Initialize description
	sprintf_s(new_item.item_description, 127, "No description");
	
	// Initialize visible boolean
	new_item.is_visible = 0;

	// Initialize dragging boolean
	//new_item.is_dragging = 0;

	// Set item_id
	for (int i = 0; i < 127; i++)
	{
		if (inventory_stock[i].item_id == -1)
		{
			new_item.item_id = i;
			inventory_stock[i] = new_item;
			return;
		}
	}

	return;
}

/*!
@brief Update function for individual item, handles clicking on items
*//*________________________________________________________________________
_*/
void Inventory_Item_Update()
{
	if (inventory_is_visible)
	{
		if (CP_Input_MouseClicked())
		{
			if (CP_Input_GetMouseWorldX() > inventory_position.x && CP_Input_GetMouseWorldX() < inventory_position.x + inventory_width
				&& CP_Input_GetMouseWorldY() > inventory_position.y && CP_Input_GetMouseWorldY() < inventory_position.y + inventory_height)
			{
				int local_selection_x = (int)((CP_Input_GetMouseWorldX() - inventory_position.x) / inventory_slot_width);
				int local_selection_y = (int)((CP_Input_GetMouseWorldY() - inventory_position.y) / inventory_slot_width);
				Inventory_Item_Use_ID(inventory[local_selection_x + (local_selection_y * 8)]);
			}
		}
		else
		{
			if (CP_Input_GetMouseWorldX() > inventory_position.x && CP_Input_GetMouseWorldX() < inventory_position.x + inventory_width
				&& CP_Input_GetMouseWorldY() > inventory_position.y && CP_Input_GetMouseWorldY() < inventory_position.y + inventory_height)
			{
				int local_selection_x = (int)((CP_Input_GetMouseWorldX() - inventory_position.x) / inventory_slot_width);
				int local_selection_y = (int)((CP_Input_GetMouseWorldY() - inventory_position.y) / inventory_slot_width);
				sprintf_s(hover_display, 127, "%s", Inventory_Stock_Get_Struct_By_ID(inventory[local_selection_x + (local_selection_y * 8)]).item_name);
				sprintf_s(hover_display_desc, 127, "%s", Inventory_Stock_Get_Struct_By_ID(inventory[local_selection_x + (local_selection_y * 8)]).item_description);
			}
			else
			{
				sprintf_s(hover_display, 127, "");
				sprintf_s(hover_display_desc, 127, "");
			}
		}
	}
}

/*!
@brief Renders an item in the inventory
@param  id				- The ID of the item to render
		x				- The x-position of the item
		y				- the y-position of the item
*//*________________________________________________________________________
_*/
void Inventory_Item_Render(int id, float x, float y)
{
	if (inventory_stock[id].item_image != NULL)
	{
		CP_Image_Draw(inventory_stock[id].item_image, 
			x + inventory_slot_width * 0.5f - inventory_slot_width * 0.1f,
			y + inventory_slot_width * 0.5f - inventory_slot_width * 0.1f,
			inventory_slot_width * 0.8f, 
			inventory_slot_width * 0.8f, 
			255);
	}
	else
	{
		CP_Settings_Fill(BLUE);
		CP_Graphics_DrawRect(x, y,
			inventory_slot_width * 0.8f,
			inventory_slot_width * 0.8f);
	}
}

/*!
@brief Removes an item from inventory by name
@param  name			- The name of the item to remove
@return the slot where the item was removed, -1 if not found
*//*________________________________________________________________________
_*/
int Inventory_Item_Remove_Name(char* name)
{
	int id = Inventory_Stock_Get_Struct_By_Name(name).item_id;
	for (int i = 63; i >= 0; i--)
	{
		if (inventory[i] == id)
		{
			inventory[i] = -1;
			return i;
		}
	}
	return -1;
}

/*!
@brief Removes an item from inventory by ID
@param  id				- The ID of the item to remove
@return the slot where the item was removed, -1 if not found
*//*________________________________________________________________________
_*/
int Inventory_Item_Remove_ID(int id)
{
	for (int i = 63; i >= 0; i--)
	{
		if (inventory[i] == id)
		{
			inventory[i] = -1;
			return i;
		}
	}
	return -1;
}

/*!
@brief Handles all the item specific function after it is used
@param  name			- The name of the item to use
*//*________________________________________________________________________
_*/
void Inventory_Item_Use_Name(char* name)
{
	if (!strcmp(name, "Speed Up"))
	{
		//Inventory_Add_Item_Name("poop");
		Player_Add_Powerup(1, 10);
		Inventory_Item_Remove_Name(name);
	}
	else if (!strcmp(name, "trash"))
	{
		Inventory_Add_Item_Name("trash");
		Inventory_Add_Item_Name("trash");
	}
	else if (!strcmp(name, "Health Potion"))
	{
		Player_Add_Health(1);
		Inventory_Item_Remove_Name(name);
	}
	else if (!strcmp(name, "Add MaxHealth Flower"))
	{
		Player_Add_MaxHealth(1);
		Inventory_Item_Remove_Name(name);
	}
	else if (!strcmp(name, "Thorns"))
	{
		Player_Lose_Health(1);
		Inventory_Item_Remove_Name(name);
	}
	else if (!strcmp(name, "Radioactive Thorns"))
	{
		Player_Lose_MaxHealth(1);
		Inventory_Item_Remove_Name(name);
	}
}

/*!
@brief Uses an item in the inventory, calls function by item ID
@param  id				- The ID of the item to use
*//*________________________________________________________________________
_*/
void Inventory_Item_Use_ID(int id)
{
	Inventory_Item_Use_Name(inventory_stock[id].item_name);
}

/*!
@brief Sets the image of an item, get item via name
@param  name			- The name of the item to set
		image			- The image to set
@return 1 after function completes
*//*________________________________________________________________________
_*/
int Inventory_Item_Set_Image(char* name, char* image)
{
	inventory_stock[Inventory_Stock_Get_Struct_By_Name(name).item_id].item_image = CP_Image_Load(image);
	return 1;
}

/*!
@brief Sets the description of an item, get item via name
@param  name			- The name of the item to set
		text			- The text to set
@return 1 after function completes
*//*________________________________________________________________________
_*/
int Inventory_Item_Set_Description(char* name, char* text)
{
	sprintf_s(inventory_stock[Inventory_Stock_Get_Struct_By_Name(name).item_id].item_description, 127, "%s", text);
	return 1;
}

/*!
@brief Frees the memory used for images by the inventory
*//*________________________________________________________________________
_*/
void Inventory_Item_Free()
{
	for (int i = 0; i < 127; i++)
	{
		if (inventory_stock[i].item_id != -1)
		{
			inventory_stock[i].item_id = -1;
			if (inventory_stock[i].item_image != NULL)
			{
				//FREE CODE not here, due to causing errors
			}
		}
	}
}