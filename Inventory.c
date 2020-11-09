#include "Inventory.h"

void Inventory_Init()
{
	inventory_window_width = (float)CP_System_GetWindowWidth();
	inventory_window_height = (float)CP_System_GetWindowHeight();

	for (int i = 0; i < 127; i++)
	{
		inventory_stock[i].item_id = -1;
	}
	return;
}

void Inventory_Open()
{

}

void Inventory_Close()
{

}

int Inventory_Add_Item(int id)
{
	return -1;
}

void Inventory_Update()
{

}

void Inventory_Render()
{

}

// Inventory Stock
struct inventory_item Inventory_Stock_Get_Struct_By_Name(char* name)
{
	for (int i = 0; i < 127; i++)
	{
		if (inventory_stock[i].item_name == name)
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

	// Initialize slot_id
	new_item.slot_id = -1;

	// Set name
	sprintf_s(new_item.item_name, 127, name);
	
	// Initialize description
	sprintf_s(new_item.item_description, 127, "No description");
	
	// Initialize visible boolean
	new_item.is_visible = 0;

	// Initialize dragging boolean
	new_item.is_dragging = 0;

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

}

void Inventory_Item_Render(int id)
{
	if (inventory_stock[id].item_image != NULL)
	{

	}
	else
	{

	}
}

void Inventory_Item_Remove()
{

}