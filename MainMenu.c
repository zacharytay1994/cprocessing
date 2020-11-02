#include <stdlib.h>
#include <stdio.h>
#include "MainMenu.h"

void MainMenu_Initialize()
{
	// Initializes all buttons as inactive
	for (int i = 0; i < 127; i++)
	{
		button_list[i].Active = 0;
	}

	button_start = Button_Initialize_Default();
	Button_Translate(button_start.Id, 0, -50);
	Button_Text_Translate(button_start.Id, 0, -50);
	Button_Text_Set(button_start.Id, "Start");
	button_credits = Button_Initialize_Default();
	Button_Text_Set(button_credits.Id, "Credits");
	button_exit = Button_Initialize_Default();
	Button_Translate(button_exit.Id, 0, 50);
	Button_Text_Translate(button_exit.Id, 0, 50);
	Button_Text_Set(button_exit.Id, "Exit");
	
	return;
}

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

	// Adds button to list
	new_button.Id = Button_List_Add(&new_button);

	return new_button;
}

char Button_Translate(int id, float displacement_x, float displacement_y)
{
	button_list[id].Position.x += displacement_x;
	button_list[id].Position.y += displacement_y;
	return 1;
}

char Button_Text_Translate(int id, float displacement_x, float displacement_y)
{
	button_list[id].Text_Position.x += displacement_x;
	button_list[id].Text_Position.y += displacement_y;
	return 1;
}

char Button_Text_Set(int id, char *new_text)
{
	sprintf_s(button_list[id].Text, 127, new_text);
	return 1;
}

int Button_List_Add(struct Button* add_button)
{
	for (int i = 0; i < 127; i++)
	{
		if (button_list[i].Active == 0)
		{
			button_list[i] = *add_button;
			button_list[i].Id = i;
			return i;
		}
	}
	return 0;
}

void MainMenu_Update(const float dt)
{
	CP_Settings_Background(CP_Color_Create(255, 255, 255, 255));
	
	Button_Update();
	return;
}

void Button_Update()
{
	Button_Mouse_Collision_Check_All();
}

void Button_Mouse_Collision_Check_All()
{
	for (int i = 0; i < 127; i++)
	{
		if (button_list[i].Active == 0)
		{
			return;
		}
		else if (button_list[i].Visible)
		{
			Button_Mouse_Collision_Check(&button_list[i]);
		}
	}
}

char Button_Mouse_Collision_Check(struct Button* button)
{
	if (CP_Input_GetMouseWorldX() > button->Position.x
		&& CP_Input_GetMouseWorldX() < button->Position.x + button->Size.x
		&& CP_Input_GetMouseWorldY() > button->Position.y
		&& CP_Input_GetMouseWorldY() < button->Position.y + button->Size.y)
	{
		button->Hover = 1;
		return 1;
	}
	else
	{
		button->Hover = 0;
	}

	return 0;
}

void MainMenu_Render()
{
	Button_Render_All();
	return;
}

void Button_Render_All()
{
	for (int i = 0; i < 127; i++)
	{
		if (button_list[i].Active == 0)
		{
			return;
		}
		else if(button_list[i].Visible)
		{
			Button_Render(&button_list[i]);
		}
	}
}

void Button_Render(struct Button *button)
{
	CP_Settings_Fill(button->Button_Color);
	if (button->Hover)
	{
		CP_Settings_Fill(button->Hover_Color);
	}
	CP_Graphics_DrawRect(button->Position.x, button->Position.y, button->Size.x * button->Scale, button->Size.y * button->Scale);
	CP_Settings_Fill(button->Text_Color);
	CP_Settings_TextSize(button->Text_Size);
	CP_Font_DrawText(button->Text, button->Text_Position.x, button->Text_Position.y);
}