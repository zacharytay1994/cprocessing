#include <stdlib.h>
#include <stdio.h>
#include "MainMenu.h"
#include "Scene.h"

void MainMenu_Initialize()
{
	window_width	= (float)CP_System_GetWindowWidth();
	window_height	= (float)CP_System_GetWindowHeight();

	logo_farmology			= CP_Image_Load("Assets/MainMenu/Logo.png");
	logo_farmology_size_x	= (float)CP_Image_GetWidth(logo_farmology);
	logo_farmology_size_y	= (float)CP_Image_GetHeight(logo_farmology);

	house			= CP_Image_Load("demo_house.png");
	house_size_x	= (float)CP_Image_GetWidth(house)	* 0.2f;
	house_size_y	= (float)CP_Image_GetHeight(house)	* 0.2f;

	button_start = Button_Initialize_Default();
	Button_Text_Set(button_start, "Start");
	Button_Size_Scale(button_start, 2.f, 2.f);
	Button_Text_Scale(button_start, 2.f);
	Button_Translate(button_start, 0, -150);
	Button_Text_Translate(button_start, 0, -120);

	button_credits = Button_Initialize_Default();
	Button_Text_Set(button_credits, "Settings");
	Button_Size_Scale(button_credits, 2.f, 2.f);
	Button_Text_Scale(button_credits, 2.f);
	Button_Translate(button_credits, 0, -30);
	Button_Text_Translate(button_credits, 0, 0);

	button_settings = Button_Initialize_Default();
	Button_Text_Set(button_settings, "Credits");
	Button_Size_Scale(button_settings, 2.f, 2.f);
	Button_Text_Scale(button_settings, 2.f);
	Button_Translate(button_settings, 0, 90);
	Button_Text_Translate(button_settings, 0, 120);

	button_exit = Button_Initialize_Default();
	Button_Text_Set(button_exit, "Exit");
	Button_Size_Scale(button_exit, 2.f, 2.f);
	Button_Text_Scale(button_exit, 2.f);
	Button_Translate(button_exit, 0, 210);
	Button_Text_Translate(button_exit, 0, 240);
	printf("%d", Scene_GetCurrentID());
	
	return;
}

void MainMenu_Update(const float dt)
{
	CP_Settings_Background(CP_Color_Create(255, 255, 255, 255));
	
	Button_Update();
	MainMenu_Render();
	return;
}

void MainMenu_Render()
{
	Background_Render();

	CP_Image_Draw(logo_farmology, window_width * 0.5f, window_height * 0.16f, logo_farmology_size_x, logo_farmology_size_y, 245);

	Button_Render_All();
	return;
}

void Background_Render()
{
	CP_Settings_Fill(LIGHT_BLUE);
	CP_Graphics_DrawRect(0, 0, window_width, window_height);

	CP_Settings_Fill(DARK_GREEN);
	CP_Graphics_DrawEllipse(window_width * 0.5f, window_height, window_width * 2, 500);

	CP_Image_Draw(house, window_width * 0.25f, window_height * 0.6f, house_size_x, house_size_y, 255);
}

void MainMenu_Exit()
{
	CP_Image_Free(&logo_farmology);
	CP_Image_Free(&house);
}