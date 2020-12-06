/*!
@file       Button.c
@author     Noel Ho Sing Nam (s.ho)
@course     CSD1400
@section    A
@brief      Handles button display properties and their functions
*//*__________________________________________________________________________
_*/
#include "Button.h"
#include "Scene.h"
#include "TestScene1.h"
#include <stdio.h>
#include <stdlib.h>

int Button_temp_bool = 0;
/*!
@brief Initializes a button with default settings
@return the ID of the button
*//*________________________________________________________________________
_*/
int Button_Initialize_Default()
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

/*!
@brief Initializes a button with set data
@param  position		- The position of the button
		size			- The size ofthe button
		text_position   - The position of the text
		button_color	- The colour of the button
		text_color		- The colour of the text
		text_size		- The size of the text
		text			- The text
		visible			- Is the button visible?
@return the ID of the button
*//*________________________________________________________________________
_*/
int Button_Initialize(CP_Vector position, CP_Vector size, CP_Vector text_position, CP_Color button_color, CP_Color text_color, float text_size, char* text, char visible)
{
	struct Button new_button;

	// Set Position
	new_button.Position.x = position.x;
	new_button.Position.y = position.y;

	// Set Size
	new_button.Size.x = (float)(strlen(text)*20) ;
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
	new_button.Darken_Color = CP_Color_Create(r, g, b, button_color.a);

	// Set Text Size
	new_button.Text_Size = text_size;

	// Set Text
	sprintf_s(new_button.Text, 127, text);

	// Set Name
	sprintf_s(new_button.Name, 127, text);

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

	// Initialize image to NULL
	new_button.Image = NULL;

	// Initialize special effect boolean to off
	new_button.Enable_SpecialEffects = 0;

	// Adds button to list
	new_button.Id = Button_List_Add(&new_button);

	//return new_button;
	return new_button.Id;
}

/*!
@brief Moves a button's position
@param  id				- The ID of the button
		displacement_x	- The x-distance to translate
		displacement_y	- The y-distance to translate
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Translate(int id, float displacement_x, float displacement_y)
{
	button_list[scene_id][id].Position.x += displacement_x;
	button_list[scene_id][id].Position.y += displacement_y;
	return 1;
}

/*!
@brief Sets a button's position
@param  id				- The ID of the button
		new_x			- The x-position to set
		new_y			- The y-position to set
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Position_Set(int id, float new_x, float new_y)
{
	button_list[scene_id][id].Position.x = new_x;
	button_list[scene_id][id].Position.y = new_y;
	return 1;
}

/*!
@brief Sets a button's size
@param  id				- The ID of the button
		new_x			- The x-size to set
		new_y			- The y-size to set
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Size_Set(int id, float new_x, float new_y)
{
	button_list[scene_id][id].Size.x = new_x;
	button_list[scene_id][id].Size.y = new_y;
	return 1;
}

/*!
@brief Scales a button's size
@param  id				- The ID of the button
		scale_x			- The amount to scale the x-size by
		scale_y			- The amount to scale the y-size by
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Size_Scale(int id, float scale_x, float scale_y)
{
	button_list[scene_id][id].Size.x *= scale_x;
	button_list[scene_id][id].Size.y *= scale_y;
	return 1;
}

/*!
@brief Sets the Scene ID for the button class, used for accessing different functions
@param  id				- The ID of the scene to set
@return the id after compete
*//*________________________________________________________________________
_*/
char Button_SceneID_Set(int id)
{
	scene_id = id;
	return (char)id;
}

/*!
@brief Moves a button's text position
@param  id				- The ID of the button
		displacement_x	- The x-distance to translate
		displacement_y	- The y-distance to translate
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Text_Translate(int id, float displacement_x, float displacement_y)
{
	button_list[scene_id][id].Text_Position.x += displacement_x;
	button_list[scene_id][id].Text_Position.y += displacement_y;
	return 1;
}

/*!
@brief Scales a button's text size
@param  id				- The ID of the button
		scale_x			- The amount to scale the x-size by
		scale_y			- The amount to scale the y-size by
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Text_Scale(int id, float scale)
{
	button_list[scene_id][id].Text_Size *= scale;
	return 1;
}

/*!
@brief Sets a button's text position
@param  id				- The ID of the button
		new_x			- The x-position to set
		new_y			- The y-position to set
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Text_SetPosition(int id, float new_x, float new_y)
{
	button_list[scene_id][id].Text_Position.x = new_x;
	button_list[scene_id][id].Text_Position.y = new_y;
	return 1;
}

/*!
@brief Sets a button's text
@param  id				- The ID of the button
		new_text		- The text to set
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Text_Set(int id, char* new_text)
{
	sprintf_s(button_list[scene_id][id].Text, 127, new_text);
	return 1;
}

/*!
@brief Sets a button's colour
@param  id				- The ID of the button
		r				- The red value
		g				- The green value
		b				- The blue value
		a				- The alpha value
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Color_Set(int id, int r, int g, int b, int a)
{
	CP_Color new_color = CP_Color_Create(r, g, b, a);
	button_list[scene_id][id].Button_Color = new_color;
	
	// Set Hover Color
	int nr = (r + 100 > 255) ? 255 : r + 100;
	int ng = (g + 100 > 255) ? 255 : g + 100;
	int nb = (b + 100 > 255) ? 255 : b + 100;
	button_list[scene_id][id].Hover_Color = CP_Color_Create(nr, ng, nb, a);

	// Set Darken Color
	nr = (r - 100 < 0) ? 0 : r - 100;
	ng = (g - 100 < 0) ? 0 : g - 100;
	nb = (b - 100 < 0) ? 0 : b - 100;
	button_list[scene_id][id].Darken_Color = CP_Color_Create(nr, ng, nb, a);
	return 1;
}

/*!
@brief Sets a button's name (not for display)
@param  id				- The ID of the button
		new_text		- The name to set
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Name_Set(int id, char* new_text)
{
	sprintf_s(button_list[scene_id][id].Name, 127, new_text);
	return 1;
}

/*!
@brief Adds a button into button class, required as the code loops through the list
@param  add_button		- The button to add
@return the id if complete, 0 if there is no more memory space
*//*________________________________________________________________________
_*/
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

/*!
@brief Update function for button class
*//*________________________________________________________________________
_*/
void Button_Update()
{
	Button_Mouse_Collision_Check_All();
}

/*!
@brief Loops through all the buttons, checks if the mouse is touching any
*//*________________________________________________________________________
_*/
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
				if (CP_Input_MouseDown(MOUSE_BUTTON_1))
				{
					button_list[scene_id][i].Darken = 1;
				}
				else if (button_list[scene_id][i].Darken)
				{
					CP_Sound_Play(button_click);
					Button_Mouse_Collision_Click_ById(i);
					button_list[scene_id][i].Darken = 0;
				}
			}
			else
			{
				button_list[scene_id][i].Darken = 0;
			}
		}
	}
}

/*!
@brief Checks if a mouse is hovering over a button
@param  id				- The ID of the button
@return 1 if mouse is over button, 0 if otherwise
*//*________________________________________________________________________
_*/
char Button_Mouse_Collision_Check(int id)
{
	if (!Button_temp_bool) {
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
	}
	else {
		if (CP_Input_GetMouseWorldX() > button_list[scene_id][id].Position.x - button_list[scene_id][id].Size.x/2.0f
			&& CP_Input_GetMouseWorldX() < button_list[scene_id][id].Position.x + button_list[scene_id][id].Size.x/2.0f
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
	}
	return 0;
}

/*!
@brief Gets a button's ID via name
@param  text			- The name of the button (not displayed)
@return the ID of the button, -1 if it doesn't exist
*//*________________________________________________________________________
_*/
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

/*!
@brief Functions deciding what happens after button is clicked
@param  id				- The ID of the button
*//*________________________________________________________________________
_*/
void Button_Mouse_Collision_Click_ById(int id)
{
	printf("Button Pressed: Scene %d, ID: %d, \"%s\"\n", Scene_GetCurrentID(), id, button_list[Scene_GetCurrentID()][id].Text);
	switch (Scene_GetCurrentID())
	{
		case 0:
		{
			switch (id)
			{
				case 4:
				{
					TestScene1_BtnManager();
					//go next case
					break;
				}
				case 5:
				{
					//Run TestScene1_BtnManager() again to close it
					break;
				}
			}
			break;
		}
		case 3:
		{
			switch (id)
			{
				case 0:		// Start
				{
					Scene_ChangeScene(2); //0 - testScene 1
					//Scene_ChangeScene(0); 
					//0 - testScene 1
					//2 - zac testbed
					break;
				}
				case 1:		// Survey
				{
					#ifdef _WIN32 
						system("start https://forms.gle/wiLHNBcqdAMYVNKY9");
					#elif __APPLE__ 
						system("open https://forms.gle/wiLHNBcqdAMYVNKY9");
					#elif __linux__ 
						system("xdg-open https://forms.gle/wiLHNBcqdAMYVNKY9");
					#endif
					break;
				}
				case 2:		// Credits
				{
					Scene_ChangeScene(6);
					break;
				}
				case 3:		// Exit
				{
					CP_Engine_Terminate();
					break;
				}
			}
			break;
		}
	}
}

/*!
@brief Checks for mouse collission with specific button via name
@param  text			- The name of the button (not displayed)
*//*________________________________________________________________________
_*/
void Button_Mouse_Collision_Click_ByText(char* text)
{
	Button_Mouse_Collision_Click_ById(Button_GetID_By_Name(text));
	return;
}

/*!
@brief Renders all the buttons
*//*________________________________________________________________________
_*/
void Button_Render_All()
{
	for (int i = 0; i < 127; i++)
	{
		if (button_list[scene_id][i].Id == -1)
		{
			return;
		}
		else if (button_list[scene_id][i].Visible)
		{
			Button_Render(i);
		}
	}
}

/*!
@brief Renders a specific button by id
@param  id				- The ID of the button
*//*________________________________________________________________________
_*/
void Button_Render(int id)
{

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	CP_Settings_Fill(button_list[scene_id][id].Button_Color);
	if (button_list[scene_id][id].Darken)
	{
		CP_Settings_Fill(button_list[scene_id][id].Darken_Color);
	}
	else if (button_list[scene_id][id].Hover)
	{
		CP_Settings_Fill(button_list[scene_id][id].Hover_Color);
	}
	if (button_list[scene_id][id].Image == NULL || button_list[scene_id][id].Enable_SpecialEffects)
	{
		CP_Graphics_DrawRect(button_list[scene_id][id].Position.x - (button_list[scene_id][id].Size.x / 2), button_list[scene_id][id].Position.y, button_list[scene_id][id].Size.x * button_list[scene_id][id].Scale, button_list[scene_id][id].Size.y * button_list[scene_id][id].Scale);
	}
	if (button_list[scene_id][id].Image != NULL)
	{
		CP_Image_Draw(button_list[scene_id][id].Image, 
			button_list[scene_id][id].Position.x,
			button_list[scene_id][id].Position.y + (button_list[scene_id][id].Size.y / 2),
			button_list[scene_id][id].Size.x,
			button_list[scene_id][id].Size.y,
			255 - (!!button_list[scene_id][id].Enable_SpecialEffects * 55));
	}

	CP_Settings_Fill(button_list[scene_id][id].Text_Color);
	CP_Settings_TextSize(button_list[scene_id][id].Text_Size);
	CP_Font_DrawText(button_list[scene_id][id].Text, button_list[scene_id][id].Text_Position.x, button_list[scene_id][id].Text_Position.y);
}

void Button_SetTempBool(int b)
{
	Button_temp_bool = b;
}

/*!
@brief Sets a button's image, replace button size with image size
@param  id				- The ID of the button
		img				- The image to set
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Image_Set_Override(int id, char* img)
{
	button_list[scene_id][id].Image = CP_Image_Load(img);
	button_list[scene_id][id].Size.x = (float)CP_Image_GetWidth(button_list[scene_id][id].Image);
	button_list[scene_id][id].Size.y = (float)CP_Image_GetHeight(button_list[scene_id][id].Image);
	return 1;
}

/*!
@brief Sets a button's image, size stays te same
@param  id				- The ID of the button
		img				- The image to set
@return 1 after function is complete
*//*________________________________________________________________________
_*/
char Button_Image_Set(int id, char* img)
{
	button_list[scene_id][id].Image = CP_Image_Load(img);
	return 1;
}

/*!
@brief Sets a button's special effects level
@param  id				- The ID of the button
		x				- The level of special effects to set
*//*________________________________________________________________________
_*/
void Button_SpecialEffects_Set(int id, char x)
{
	button_list[scene_id][id].Enable_SpecialEffects = x;
	return;
}

/*!
@brief Sets a button to active
@param  id				- The ID of the button
		x				- Toggle for button active
@return x after function is complete
*//*________________________________________________________________________
_*/
char Button_Active_Set(int id, char x)
{
	button_list[scene_id][id].Active = x;
	return x;
}

void Button_Escape()
{
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
}

/*!
@brief Initializes the button class
*//*________________________________________________________________________
_*/
void Button_Class_Init()
{
	for (int i = 0; i < 63; i++)
	{
		for (int j = 0; j < 127; j++)
		{
			button_list[i][j].Id = -1;
		}
	}

	button_click = CP_Sound_Load("Assets/Cowbell.wav");
}