#pragma once

#include <cprocessing.h>

#include "GameState.h"

struct Button
{
	CP_Vector	Position;
	CP_Vector	Size;
	CP_Vector	Text_Position;
	CP_Color	Button_Color;
	CP_Color	Text_Color;
	CP_Color	Hover_Color;
	float		Text_Size;
	float		Scale;
	int			Id;
	char		Text[127];
	char		Active;
	char		Visible;
	char		Hover;
};

struct Button button_start;
struct Button button_credits;
struct Button button_exit;
struct Button button_list[127];
int Button_List_Add(struct Button *add_button);
void Button_Update();
void Button_Render_All();
void Button_Mouse_Collision_Check_All();
char Button_Mouse_Collision_Check(struct Button *button);
char Button_Translate(int id, float displacement_x, float displacement_y);
char Button_Text_Translate(int id, float displacement_x, float displacement_y);
char Button_Text_Set(int id, char *new_text);

void MainMenu_Initialize();
struct Button Button_Initialize_Default();
struct Button Button_Initialize(CP_Vector position, CP_Vector size, CP_Vector text_position, CP_Color button_color, CP_Color text_color, float text_size, char* text, char visible);

void MainMenu_Update(const float dt);

void MainMenu_Render();
void Button_Render(struct Button *button);