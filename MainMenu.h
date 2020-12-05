#pragma once

#include <cprocessing.h>

#include "Button.h"

#define       GREEN	CP_Color_Create(0, 255, 0, 255)
#define  DARK_GREEN	CP_Color_Create(0, 155, 0, 255)
#define	  	   BLUE	CP_Color_Create(0, 0, 255, 255)
#define   DARK_BLUE	CP_Color_Create(0, 0, 155, 255)
#define  LIGHT_BLUE	CP_Color_Create(125, 125, 255, 255)


int button_start;
int button_survey;
int button_settings;
int button_exit;


CP_Color Menu_Button_Text_Color;
CP_Color Menu_Button_Text_Alpha;
char* Menu_Button_Words;
char  Menu_Button_Visible;


CP_Image	menu_background;


CP_Image	menu_logo_farmology;
float		logo_farmology_size_x;
float		logo_farmology_size_y;


CP_Image	house;
float		house_size_x;
float		house_size_y;


float window_width;
float window_height;
float window_width_div5;
float window_heigth_div4;
float Menu_Button_Text_Size;

void MainMenu_Initialize();
void MainMenu_Update(const float dt);
void MainMenu_Render();
void Background_Render();
void MainMenu_Exit();