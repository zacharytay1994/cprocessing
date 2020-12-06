/*!
@file       MainMenu.c
@author     Noel Ho Sing Nam (s.ho) / Jia Min
@course     CSD1400
@section    A
@brief      Handles the main menu and the button rendering in the mainmenu
*//*__________________________________________________________________________
_*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "MainMenu.h"
#include "Scene.h"
#include "Sprite.h"
#include "Camera.h"

int MainMenu_sound_bool = 0;

void MainMenu_Initialize()
{
	Camera_Reset();
	PI = 3.149f;

	window_width	= (float)CP_System_GetWindowWidth();
	window_height	= (float)CP_System_GetWindowHeight();

	window_width_div5	= window_width / 5.0f;
	window_heigth_div4	= window_height / 4.0f;

	Menu_Button_Pos.x = window_width_div5;
	Menu_Button_Pos.y = window_heigth_div4 * 3.0f;

	Menu_Button_Box_Size.x	= 0;
	Menu_Button_Box_Size.y	= 100;

	Menu_Button_Text_Pos.x	= window_width_div5;
	Menu_Button_Text_Pos.y	= (float) window_heigth_div4 * 3.0f+(Menu_Button_Box_Size.y /2.0f);

	Menu_Vector_Button.x	= window_width_div5;
	Menu_Vector_Button.y	= window_heigth_div4 * 2.0f;

	Menu_Button_Text_Color	=  CP_Color_Create(255, 240, 255, 255);
	Menu_Button_Text_Alpha	= CP_Color_Create(0, 0, 0, 255);

	Menu_Button_Words		= "         " ;
	Menu_Button_Text_Size	= 50.0f		  ;
	Menu_Button_Visible		= 1			  ;
	Menu_Button_Length		= (float)(strlen(Menu_Button_Words) * 20);
	Menu_Button_Heigth		= 100.0f;


	menu_logo_farmology = CP_Image_Load("Assets/MainMenu/Logo_Background-02.png");
	menu_background = CP_Image_Load("Assets/MainMenu/Background-03.png");
	menu_logo_farmology_shadow = CP_Image_Load("Assets/MainMenu/Logo_Background_Shadow-04.png");
	button_logo_shadow = CP_Image_Load("Assets/MainMenu/Menu_Button_Shadow-11.png");


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
	


	Button_Settings = Button_Initialize(
		Menu_Button_Pos,
		Menu_Button_Box_Size,
		Menu_Button_Text_Pos,
		Menu_Button_Text_Color,
		Menu_Button_Text_Alpha,
		Menu_Button_Text_Size,
		Menu_Button_Words,
		Menu_Button_Visible
	);
	Button_Text_Set(Button_Settings, "Credits");
	Button_Translate(Button_Settings, window_width_div5*2.0f, 0);
	Button_Text_Translate(Button_Settings, window_width_div5*2.0f, 0);
	Button_Color_Set(Button_Settings, 195, 154, 107, 255);



	Button_Exit = Button_Initialize(
		Menu_Button_Pos,
		Menu_Button_Box_Size,
		Menu_Button_Text_Pos,
		Menu_Button_Text_Color,
		Menu_Button_Text_Alpha,
		Menu_Button_Text_Size,
		Menu_Button_Words,
		Menu_Button_Visible
	);
	Button_Text_Set(Button_Exit, "Exit");
	Button_Translate(Button_Exit, window_width_div5*3.0f, 0);
	Button_Text_Translate(Button_Exit, window_width_div5*3.0f,0);
	Button_Color_Set(Button_Exit, 195, 154, 107, 255);
	

	
	menu_font = CP_Font_Load("Assets/Fonts/zrnic rg.ttf");
	CP_Font_Set(menu_font);

	background_music = CP_Sound_LoadMusic("Assets/Music/ShortLoopHalf.wav");
	MainMenu_jump = CP_Sound_LoadMusic("Assets/Cowbell.wav");
	CP_Sound_PlayMusic(background_music);

	Menu_P_Jump_Width	= 180.0f;
	Menu_P_Jump_Height	= 180.0f;
	Menu_P_Jump_Pic_Pos = (Menu_P_Jump_Height/10.0f);
	Menu_P_Jump_Pos.x = window_width_div5;
	Menu_P_Jump_Pos.y = window_heigth_div4 * 3.0f - (Menu_Button_Box_Size.y / 2.0f) - Menu_P_Jump_Pic_Pos;
	Menu_P_Jump_Check = window_width_div5;
	Menu_P_Jump_Id = Sprite_AddSprite(Menu_P_Jump_Pos, Menu_P_Jump_Width, Menu_P_Jump_Height , "./Sprites/p_run_01.png", 2, 3, 6, 10, 0);
	
	Button_SetTempBool(1);
	return;
}

/*!
@brief Update function for mainmenu
@param  dt				- deltaTime framerate
*//*________________________________________________________________________
_*/

void MainMenu_Update(const float dt)
{
	Menu_P_Jump_Get_Pos = Sprite_GetPosition(Menu_P_Jump_Id);

	MouseX = CP_Input_GetMouseX();
	MouseY = CP_Input_GetMouseY();
	
	CP_Settings_Background(CP_Color_Create(255, 255, 255, 255));
	
	Menu_Button_PosX = window_width_div5;
	Menu_Button_PosY = (window_heigth_div4 * 3.0f) + Menu_Button_Heigth / 2.0f;	
	Menu_P_Jump_Pos_Speed = 200.0f * dt;			

	if (MouseY < Menu_Button_PosY + Menu_Button_Heigth / 2.0f &&
		MouseY > Menu_Button_PosY - Menu_Button_Heigth / 2.0f)
	{
		if (MouseX < Menu_Button_PosX + Menu_Button_Length / 2.0f &&
			MouseX > Menu_Button_PosX - Menu_Button_Length / 2.0f)
		{
			Menu_P_Jump_Check = Menu_Button_PosX;
		}
		if (MouseX < Menu_Button_PosX * 2 + Menu_Button_Length / 2.0f &&
			MouseX > Menu_Button_PosX * 2 - Menu_Button_Length / 2.0f)
		{
			Menu_P_Jump_Check = Menu_Button_PosX * 2;
		}
		if (MouseX < Menu_Button_PosX * 3 + Menu_Button_Length / 2.0f &&
			MouseX > Menu_Button_PosX * 3 - Menu_Button_Length / 2.0f)
		{
			Menu_P_Jump_Check = Menu_Button_PosX * 3;
		}
		if (MouseX < Menu_Button_PosX * 4 + Menu_Button_Length / 2.0f &&
			MouseX > Menu_Button_PosX * 4 - Menu_Button_Length / 2.0f)
		{
			Menu_P_Jump_Check = Menu_Button_PosX * 4;
		}
	}

	Menu_P_Jump_Distance = Menu_P_Jump_Check - Menu_P_Jump_Get_Pos.x;

	if (Menu_P_Jump_Distance > 5.0f || Menu_P_Jump_Distance < -5.0f)
	{
		if (Menu_P_Jump_Distance > 0)
		{

			Sprite_SetFlip(Menu_P_Jump_Id, -1);
			Menu_P_Jump_Pos.x += Menu_P_Jump_Pos_Speed;
		}
		else if (Menu_P_Jump_Distance < 0)
		{
			Sprite_SetFlip(Menu_P_Jump_Id, 0);
			Menu_P_Jump_Pos.x -= Menu_P_Jump_Pos_Speed;
		}
		if (MainMenu_sound_bool) { MainMenu_sound_bool = 0; }
	}
	else
	{
		if (!MainMenu_sound_bool) { CP_Sound_Play(MainMenu_jump); MainMenu_sound_bool = 1; }
		
		Menu_P_Jump_Get_Pos.x = Menu_P_Jump_Check;
	}
	
	float val = (float)sin((Menu_P_Jump_Get_Pos.x - window_width_div5) / (window_width_div5 * 3.0f) * PI * 3.0f);

	if (val > 0.0f)
	{
		val *= -1;
	}
	
	Menu_P_Jump_Pos.y = window_heigth_div4 *3.0f - (Menu_Button_Box_Size.y / 2.0f) - Menu_P_Jump_Pic_Pos + val * 250.0f;
	
	Sprite_SetPosition(Menu_P_Jump_Id, Menu_P_Jump_Pos);





	Menu_Logo_Farmology_Shadow.x = 0.02f * (MouseX - (window_width * 0.5f));
	Menu_Logo_Farmology_Shadow.y = 0.02f * (MouseY - ((window_height / 8.0f) * 3.0f));




	Menu_Button_Text_Shadow.y	= 0.015f * (MouseY - Menu_Button_PosY);
	Menu_Button_Text_Shadow.x	= 0.015f * (MouseX - Menu_Button_PosX);
	Menu_Button_Text_Shadow_1.x = 0.015f * (MouseX - Menu_Button_PosX * 2);
	Menu_Button_Text_Shadow_2.x = 0.015f * (MouseX - Menu_Button_PosX * 3);
	Menu_Button_Text_Shadow_3.x = 0.015f * (MouseX - Menu_Button_PosX * 4);





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
	
	CP_Image_Draw(button_logo_shadow, Menu_Button_PosX- Menu_Button_Text_Shadow.x, Menu_Button_PosY- Menu_Button_Text_Shadow.y, 180.0f, 100.0f, 150);
	CP_Image_Draw(button_logo_shadow, Menu_Button_PosX * 2 - Menu_Button_Text_Shadow_1.x, Menu_Button_PosY - Menu_Button_Text_Shadow.y, 180.0f, 100.0f, 150);
	CP_Image_Draw(button_logo_shadow, Menu_Button_PosX * 3 - Menu_Button_Text_Shadow_2.x, Menu_Button_PosY - Menu_Button_Text_Shadow.y, 180.0f, 100.0f, 150);
	CP_Image_Draw(button_logo_shadow, Menu_Button_PosX * 4 - Menu_Button_Text_Shadow_3.x, Menu_Button_PosY - Menu_Button_Text_Shadow.y, 180.0f, 100.0f, 150);


	CP_Image_Draw (menu_logo_farmology_shadow, (window_width * 0.5f)- Menu_Logo_Farmology_Shadow.x, ((window_height / 8.0f) * 3.0f)- Menu_Logo_Farmology_Shadow.y, 1600.0f, 350.0f, 150);

	CP_Image_Draw (menu_logo_farmology, window_width * 0.5f, (window_height / 8.0f )*3.0f, 1600.0f, 350.0f, 255);

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
	Sprite_SetVisible(Menu_P_Jump_Id, 0);
	Button_Escape();
}