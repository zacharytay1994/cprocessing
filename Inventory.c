#include "Inventory.h"
#include <string.h>

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

void Inventory_Open()
{
	inventory_is_visible = 1;
}

void Inventory_Close()
{
	inventory_is_visible = 0;
}

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

void Inventory_Update()
{
	if (inventory_is_visible)
	{
		Inventory_Item_Update();
	}
}

void Inventory_Render()
{
	if (inventory_is_visible)
	{
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
	}
}

// Inventory Stock
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

// Inventory Item
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

void Inventory_Item_Update()
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
}

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

void Inventory_Item_Use_Name(char* name)
{
	if (!strcmp(name, "poop"))
	{
		Inventory_Add_Item_Name("poop");
	}
	else if (!strcmp(name, "trash"))
	{
		Inventory_Add_Item_Name("trash");
		Inventory_Add_Item_Name("trash");
	}
}

void Inventory_Item_Use_ID(int id)
{
	Inventory_Item_Use_Name(inventory_stock[id].item_name);
}

int Inventory_Item_Set_Image(char* name, char* image)
{
	inventory_stock[Inventory_Stock_Get_Struct_By_Name(name).item_id].item_image = CP_Image_Load(image);
	return 1;
}

void Inventory_Item_Free()
{
	for (int i = 0; i < 127; i++)
	{
		if (inventory_stock[i].item_id != -1)
		{
			inventory_stock[i].item_id = -1;
			if (inventory_stock[i].item_image != NULL)
			{
				//FREE CODE
			}
		}
	}
}