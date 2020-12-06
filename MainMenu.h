/*!
@file       MainMenu.h
@author     Noel Ho Sing Nam (s.ho)/ Jia Min
@course     CSD1400
@section    A
@brief      Contains the data and functions for rendering the main menu
*//*__________________________________________________________________________
_*/
#pragma once
#include <cprocessing.h>
#include "Button.h"

#define       GREEN	CP_Color_Create(0, 255, 0, 255)
#define  DARK_GREEN	CP_Color_Create(0, 155, 0, 255)
#define	  	   BLUE	CP_Color_Create(0, 0, 255, 255)
#define   DARK_BLUE	CP_Color_Create(0, 0, 155, 255)
#define  LIGHT_BLUE	CP_Color_Create(125, 125, 255, 255)


typedef CP_Vector CP;

int button_start;
int button_survey;
int Button_Settings;
int Button_Exit;
int	Menu_P_Jump_Id;
int Menu_P_Jump_Count;

CP_Color Menu_Button_Text_Color;
CP_Color Menu_Button_Text_Alpha;
char* Menu_Button_Words;
char  Menu_Button_Visible;


CP			Menu_Button_Pos;
CP			Menu_Button_Box_Size;
CP			Menu_Button_Text_Pos;
CP			Menu_Button_Text_Shadow;
CP			Menu_Button_Text_Shadow_1;
CP			Menu_Button_Text_Shadow_2;
CP			Menu_Button_Text_Shadow_3;
CP			Menu_Vector_Button;
CP			Menu_P_Jump_Pos;
CP			Menu_P_Jump_Get_Pos;
CP			Cursor_To_Go;
CP			Menu_Logo_Farmology_Shadow;

CP_Image	menu_background;
CP_Image	menu_logo_farmology;
CP_Image	menu_logo_farmology_shadow;
CP_Image	button_logo_shadow;
CP_Font		menu_font;

float PI;

float window_width;
float window_height;
float window_width_div5;
float window_heigth_div4;

float MouseX;
float MouseY;

float Menu_Button_Text_Size;
float Menu_Button_Length;
float Menu_Button_Heigth;
float Menu_Button_PosX;
float Menu_Button_PosY;
float Menu_P_Jump_Check;
float Menu_P_Jump_Distance;

float Menu_P_Jump_Pos_Speed;
float Menu_P_Jump_Pos_Add;
float Menu_P_Jump_Width;
float Menu_P_Jump_Height;
float Menu_P_Jump_Pic_Pos;


void MainMenu_Initialize();
void MainMenu_Update(const float dt);
void MainMenu_Render();
void Background_Render();
void MainMenu_Exit();