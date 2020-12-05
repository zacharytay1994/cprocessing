/*!
@file       MainMenu.c
@author     Noel Ho Sing Nam (s.ho)
@course     CSD1400
@section    A
@brief      Handles the main menu and the button rendering in the mainmenu
*//*__________________________________________________________________________
_*/
#include <stdlib.h>
#include <stdio.h>
#include "MainMenu.h"
#include "Scene.h"

typedef CP_Vector CP;

void MainMenu_Initialize()
{
	window_width	= (float)CP_System_GetWindowWidth();
	window_height	= (float)CP_System_GetWindowHeight();

	window_width_div5 = window_width / 5.0f;
	window_heigth_div4 = window_height / 4.0f;

	CP Menu_Button_Pos;
	Menu_Button_Pos.x = window_width_div5;
	Menu_Button_Pos.y = window_heigth_div4 * 3.0f;

	CP Menu_Button_Box_Size;
	Menu_Button_Box_Size.x = 0;
	Menu_Button_Box_Size.y = 100;

	CP Menu_Button_Text_Pos;
	Menu_Button_Text_Pos.x = window_width_div5;
	Menu_Button_Text_Pos.y = (float) window_heigth_div4 * 3.0f+(Menu_Button_Box_Size.y /2.0f);

	CP Menu_Vector_Button;
	Menu_Vector_Button.x = window_width_div5;
	Menu_Vector_Button.y = window_heigth_div4 * 2.0f;

	Menu_Button_Text_Color =  CP_Color_Create(255, 240, 255, 255);
	Menu_Button_Text_Alpha = CP_Color_Create(0, 0, 0, 255);

	Menu_Button_Words		= "         ";
	Menu_Button_Text_Size	= 50.0f		 ;
	Menu_Button_Visible		= 1			 ;

	menu_logo_farmology = CP_Image_Load("Assets/MainMenu/Logo_Background-02.png");
	menu_background = CP_Image_Load("Assets/MainMenu/Background-03.png");


	button_start = Button_Initialize(
		Menu_Button_Pos,
		Menu_Button_Box_Size,
		Menu_Button_Text_Pos,
		Menu_Button_Text_Color,
		Menu_Button_Text_Alpha,
		Menu_Button_Text_Size,
		Menu_Button_Words,
		Menu_Button_Visible
		);
	Button_Text_Set(button_start, "Start");
	Button_Color_Set(button_start, 195, 154, 107, 255);
	


	button_survey = Button_Initialize(
		Menu_Button_Pos,
		Menu_Button_Box_Size,
		Menu_Button_Text_Pos,
		Menu_Button_Text_Color,
		Menu_Button_Text_Alpha,
		Menu_Button_Text_Size,
		Menu_Button_Words,
		Menu_Button_Visible
	);
	Button_Text_Set(button_survey, "Survey");
	Button_Translate(button_survey, window_width_div5, 0); 
	Button_Text_Translate(button_survey, window_width_div5, 0);
	Button_Color_Set(button_survey, 195, 154, 107, 255);
	


	button_settings = Button_Initialize(
		Menu_Button_Pos,
		Menu_Button_Box_Size,
		Menu_Button_Text_Pos,
		Menu_Button_Text_Color,
		Menu_Button_Text_Alpha,
		Menu_Button_Text_Size,
		Menu_Button_Words,
		Menu_Button_Visible
	);
	Button_Text_Set(button_settings, "Credits");
	Button_Translate(button_settings, window_width_div5*2.0f, 0);
	Button_Text_Translate(button_settings, window_width_div5*2.0f, 0);
	Button_Color_Set(button_settings, 195, 154, 107, 255);



	button_exit = Button_Initialize(
		Menu_Button_Pos,
		Menu_Button_Box_Size,
		Menu_Button_Text_Pos,
		Menu_Button_Text_Color,
		Menu_Button_Text_Alpha,
		Menu_Button_Text_Size,
		Menu_Button_Words,
		Menu_Button_Visible
	);
	Button_Text_Set(button_exit, "Exit");
	Button_Translate(button_exit, window_width_div5*3.0f, 0);
	Button_Text_Translate(button_exit, window_width_div5*3.0f,0);
	Button_Color_Set(button_exit, 195, 154, 107, 255);
	

	
	menu_font = CP_Font_Load("Assets/Fonts/zrnic rg.ttf");
	CP_Font_Set(menu_font);

	return;
}

/*!
@brief Update function for mainmenu
@param  dt				- deltaTime framerate
*//*________________________________________________________________________
_*/
void MainMenu_Update(const float dt)
{
	CP_Settings_Background(CP_Color_Create(255, 255, 255, 255));
	
	Button_Update();

	MainMenu_Render();

	return;
}

/*!
@brief Renders the mainmenu
*//*________________________________________________________________________
_*/
void MainMenu_Render()
{
	Background_Render();
		
	CP_Image_Draw(menu_logo_farmology, window_width * 0.5f, (window_height / 8.0f )*3.0f, 1600.0f, 350.0f, 255);

	Button_Render_All();

	return;
}

/*!
@brief Renders the background
*//*________________________________________________________________________
_*/
void Background_Render()
{
	CP_Image_Draw(menu_background, window_width * 0.5f, window_height * 0.5f, 1600.0f, 900.0f, 255);
}

/*!
@brief Exits the mainmenu
*//*________________________________________________________________________
_*/
void MainMenu_Exit()
{
	/*CP_Image_Free(&menu_background);
	CP_Image_Free(&menu_logo_farmology);*/
	CP_Font_Set(CP_Font_GetDefault());
}