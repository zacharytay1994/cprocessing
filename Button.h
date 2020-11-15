#pragma once
#include <cprocessing.h>

struct Button
{
	CP_Vector	Position;
	CP_Vector	Size;
	CP_Vector	Text_Position;
	CP_Color	Button_Color;
	CP_Color	Text_Color;
	CP_Color	Hover_Color;
	CP_Color	Darken_Color;
	CP_Image	Image;
	float		Text_Size;
	float		Scale;
	int			Id;
	char		Name[127];
	char		Text[127];
	char		Active;
	char		Visible;
	char		Hover;
	char		Darken;
	char		Enable_SpecialEffects;
};

int scene_id;
struct Button button_list[63][127];
int Button_List_Add(struct Button* add_button);
void Button_Update();
void Button_Render_All();
void Button_Mouse_Collision_Check_All();
char Button_Mouse_Collision_Check(int id);
void Button_Mouse_Collision_Click_ByText(char* text);
void Button_Mouse_Collision_Click_ById(int id);
char Button_Translate(int id, float displacement_x, float displacement_y);
char Button_Position_Set(int id, float new_x, float new_y);
char Button_Size_Set(int id, float new_x, float new_y);
char Button_Size_Scale(int id, float scale_x, float scale_y);
char Button_Text_Translate(int id, float displacement_x, float displacement_y);
char Button_Text_SetPosition(int id, float new_x, float new_y);
char Button_Text_Scale(int id, float scale);
char Button_Text_Set(int id, char* new_text);
char Button_Color_Set(int id, int r, int g, int b, int a);
char Button_Image_Set(int id, char* img);
char Button_Image_Set_Override(int id, char* img);
int Button_GetID_By_Name(char* text);
char Button_SceneID_Set(int id);
void Button_SpecialEffects_Set(int id,char x);
char Button_Active_Set(int id, char x);

//struct Button Button_Initialize_Default();
//struct Button Button_Initialize(CP_Vector position, CP_Vector size, CP_Vector text_position, CP_Color button_color, CP_Color text_color, float text_size, char* text, char visible);

void Button_Class_Init();

int Button_Initialize_Default();
int Button_Initialize(CP_Vector position, CP_Vector size, CP_Vector text_position, CP_Color button_color, CP_Color text_color, float text_size, char* text, char visible);


void Button_Render(int id);