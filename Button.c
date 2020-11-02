#include "Button.h"
#include <stdio.h>

struct Button Button_Initialize_Default()
{
	CP_Vector default_pos;
	default_pos.x = (float)CP_System_GetWindowWidth() / 2;
	default_pos.y = (float)CP_System_GetWindowHeight() / 2;

	CP_Vector default_text_pos;
	default_text_pos.x = (float)CP_System_GetWindowWidth() / 2 + 8;
	default_text_pos.y = (float)CP_System_GetWindowHeight() / 2 + 40;

	CP_Vector default_size;
	default_size.x = 250;
	default_size.y = 50;

	return Button_Initialize(
		default_pos,
		default_size,
		default_text_pos,
		CP_Color_Create(255, 240, 255, 255),
		CP_Color_Create(0, 0, 0, 255),
		50.0f,
		"Text Not Set",
		1
	);

}

struct Button Button_Initialize(CP_Vector position, CP_Vector size, CP_Vector text_position, CP_Color button_color, CP_Color text_color, float text_size, char* text, char visible)
{
	struct Button new_button;

	// Set Position
	new_button.Position.x = position.x;
	new_button.Position.y = position.y;

	// Set Size
	new_button.Size.x = (float)(strlen(text) * 20) + 8;
	new_button.Size.y = size.y;

	// Set Text Position
	new_button.Text_Position.x = text_position.x;
	new_button.Text_Position.y = text_position.y;

	// Set Color
	new_button.Button_Color = button_color;

	// Set Text Color
	new_button.Text_Color = text_color;

	// Set Hover Color
	int r = (button_color.r + 100 > 255) ? 255 : button_color.r + 100;
	int g = (button_color.g + 100 > 255) ? 255 : button_color.g + 100;
	int b = (button_color.b + 100 > 255) ? 255 : button_color.b + 100;
	new_button.Hover_Color = CP_Color_Create(r, g, b, button_color.a);

	// Set Darken Color
	r = (button_color.r - 100 < 0) ? 0 : button_color.r - 100;
	g = (button_color.g - 100 < 0) ? 0 : button_color.g - 100;
	b = (button_color.b - 100 < 0) ? 0 : button_color.b - 100;
	new_button.Hover_Color = CP_Color_Create(r, g, b, button_color.a);

	// Set Text Size
	new_button.Text_Size = text_size;

	// Set Text
	sprintf_s(new_button.Text, 127, text);

	// Set Scale
	new_button.Scale = 1.0f;

	// Sets the button status as active
	new_button.Active = 1;

	// Sets the button as visible
	new_button.Visible = visible;

	// Initialize hover state
	new_button.Hover = 0;

	// Initialize darken state
	new_button.Darken = 0;

	// Adds button to list
	new_button.Id = Button_List_Add(&new_button);

	return new_button;
}

char Button_Translate(int id, float displacement_x, float displacement_y)
{
	button_list[scene_id][id].Position.x += displacement_x;
	button_list[scene_id][id].Position.y += displacement_y;
	return 1;
}

char Button_Text_Translate(int id, float displacement_x, float displacement_y)
{
	button_list[scene_id][id].Text_Position.x += displacement_x;
	button_list[scene_id][id].Text_Position.y += displacement_y;
	return 1;
}

char Button_Text_Set(int id, char* new_text)
{
	sprintf_s(button_list[scene_id][id].Text, 127, new_text);
	return 1;
}

int Button_List_Add(struct Button* add_button)
{
	for (int i = 0; i < 127; i++)
	{
		if (button_list[scene_id][i].Active == 0)
		{
			button_list[scene_id][i] = *add_button;
			button_list[scene_id][i].Id = i;
			return i;
		}
	}
	return 0;
}

void Button_Update()
{
	Button_Mouse_Collision_Check_All();
}

void Button_Mouse_Collision_Check_All()
{
	for (int i = 0; i < 127; i++)
	{
		if (button_list[scene_id][i].Active == 0)
		{
			return;
		}
		else if (button_list[scene_id][i].Visible)
		{
			if (Button_Mouse_Collision_Check(i))
			{
				if (CP_Input_MouseClicked())
				{
					button_list[scene_id][i].Darken = 1;
					Button_Mouse_Collision_Click_ById(i);
				}
				else
				{
					button_list[scene_id][i].Darken = 0;
				}
			}
		}
	}
}

char Button_Mouse_Collision_Check(int id)
{
	if (CP_Input_GetMouseWorldX() > button_list[scene_id][id].Position.x
		&& CP_Input_GetMouseWorldX() < button_list[scene_id][id].Position.x + button_list[scene_id][id].Size.x
		&& CP_Input_GetMouseWorldY() > button_list[scene_id][id].Position.y
		&& CP_Input_GetMouseWorldY() < button_list[scene_id][id].Position.y + button_list[scene_id][id].Size.y)
	{
		button_list[scene_id][id].Hover = 1;
		return 1;
	}
	else
	{
		button_list[scene_id][id].Hover = 0;
	}

	return 0;
}

int Button_GetID_By_Name(char* text)
{
	for (int i = 0; i < 127; i++)
	{
		if (strcmp(button_list[scene_id][i].Text, text))
		{
			return i;
		}
	}
	return -1;
}

void Button_Mouse_Collision_Click_ById(int id)
{
	switch (id)
	{
	case 0:		// Start
	{
		break;
	}
	case 1:		// Credits
	{
		break;
	}
	case 2:		// Exit
	{
		CP_Engine_Terminate();
		break;
	}
	}
}

void Button_Mouse_Collision_Click_ByText(char* text)
{
	Button_Mouse_Collision_Click_ById(Button_GetID_By_Name(text));
	return;
}

void Button_Render_All()
{
	for (int i = 0; i < 127; i++)
	{
		if (button_list[scene_id][i].Active == 0)
		{
			return;
		}
		else if (button_list[scene_id][i].Visible)
		{
			Button_Render(i);
		}
	}
}

void Button_Render(int id)
{
	CP_Settings_Fill(button_list[scene_id][id].Button_Color);
	if (button_list[scene_id][id].Darken)
	{
		CP_Settings_Fill(button_list[scene_id][id].Darken_Color);
	}
	else if (button_list[scene_id][id].Hover)
	{
		CP_Settings_Fill(button_list[scene_id][id].Hover_Color);
	}
	CP_Graphics_DrawRect(button_list[scene_id][id].Position.x, button_list[scene_id][id].Position.y, button_list[scene_id][id].Size.x * button_list[scene_id][id].Scale, button_list[scene_id][id].Size.y * button_list[scene_id][id].Scale);
	CP_Settings_Fill(button_list[scene_id][id].Text_Color);
	CP_Settings_TextSize(button_list[scene_id][id].Text_Size);
	CP_Font_DrawText(button_list[scene_id][id].Text, button_list[scene_id][id].Text_Position.x, button_list[scene_id][id].Text_Position.y);
}