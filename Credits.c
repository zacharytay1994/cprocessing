/*!
@file       Credits.c
@author     Noel Ho Sing Nam (s.ho) / Jia Min
@course     CSD1400
@section    A
@brief      Renders the credits
*//*__________________________________________________________________________
_*/
#include "Credits.h"
#include <stdio.h>

/*!
@brief Initializes the Credits
*//*________________________________________________________________________
_*/
void Credits_Init()
{
	credit_background = CP_Image_Load("Assets/Credits/Credits_Background-06.png");
	credit_logo = CP_Image_Load("Assets/Credits/Credits_logo-05.png");
	credit_logo_shadow = CP_Image_Load("Assets/Credits/Credits_logo-10.png");
	credit_box = CP_Image_Load("Assets/Credits/Credits_box-07.png");
	credit_button = CP_Image_Load("Assets/Credits/Credits_button-09.png");

	Credit_Button_Width = 100.0f * 1.5f;
	Credit_Button_Height = 45.0f * 1.5f;

	GetMouseX = CP_Input_GetMouseX();
	GetMouseY = CP_Input_GetMouseY();

		
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	Credits_WindowWidth		= CP_System_GetWindowWidth();
	Credits_WindowHeight	= CP_System_GetWindowHeight();

	Credits_WindowWidth_div16	= Credits_WindowWidth	/ 16.0f;
	Credits_WindowHeight_div16	= Credits_WindowHeight	/ 16.0f;

	sprintf_s(copyright, 255, "All content (c) 2020 DigiPen Institute of Technology Singapore, all rights reserved.");
	
	sprintf_s(subtitle1, 127, "Team Members");
	sprintf_s(line1, 127, "Zachary");
	sprintf_s(line2, 127, "Jia Min");
	sprintf_s(line3, 127, "Ray");
	sprintf_s(line4, 127, "Noel");

	sprintf_s(subtitle2, 127, "Instructors");
	sprintf_s(line5, 127, "Cheng Ding Xiang");
	sprintf_s(line6, 127, "Raymond Teo");

	sprintf_s(subtitle3, 127, "President");
	sprintf_s(line7, 127, "Claude Comair");

	Time_Buffer = 2;

	custom_font = CP_Font_Load("Assets/Fonts/zrnic rg.ttf");
	CP_Font_Set(custom_font);

}

/*!
@brief Update function for Credits
*//*________________________________________________________________________
_*/
void Credits_Update(const float dt)
{
	Scene_Number = 3;

	if (Time_Buffer > 0)
	{
		Time_Buffer -= dt;
	}
	

	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		printf("ESC Pressed!");
		Scene_ChangeScene(Scene_Number);
	}

		
	if (GetMouseX < Credits_WindowWidth_div16*8.0f + Credit_Button_Width / 2.0f &&
	GetMouseX > Credits_WindowWidth_div16*8.0f - Credit_Button_Width / 2.0f &&
	GetMouseY > Credits_WindowHeight_div16*8.0f - Credit_Button_Height / 2.0f &&
	GetMouseY < Credits_WindowHeight_div16*8.0f + Credit_Button_Height / 2.0f)
	{
		if (CP_Input_MouseClicked())
		{
		Scene_ChangeScene(Scene_Number);
		}
	}
	

	Credit_Logo_Shadow.x = 0.015f * (GetMouseX - (Credits_WindowWidth_div16 * 8.0f));
	Credit_Logo_Shadow.y = 0.015f * (GetMouseY - (Credits_WindowHeight_div16 * 4.0f));

	Credits_Render();
}

/*!
@brief Renders the Credits
*//*________________________________________________________________________
_*/
void Credits_Render()
{
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Image_Draw(credit_background, Credits_WindowWidth_div16 * 8.0f, Credits_WindowHeight_div16 * 8.0f, 1600.0f, 900.0f, 255);
	CP_Image_Draw(credit_logo_shadow, Credits_WindowWidth_div16 * 8.0f - Credit_Logo_Shadow.x , Credits_WindowHeight_div16 * 4.0f - Credit_Logo_Shadow.y, 1600.0f, 350.0f, 255);
	CP_Image_Draw(credit_logo, Credits_WindowWidth_div16 * 8.0f, Credits_WindowHeight_div16 * 4.0f, 1600.0f, 350.0f, 255);
	CP_Image_Draw(credit_box, Credits_WindowWidth_div16 * 3.0f, Credits_WindowHeight_div16 * 8.0f, 270.0f, 65.0f, 80);
	CP_Image_Draw(credit_box, Credits_WindowWidth_div16 * 8.0f, Credits_WindowHeight_div16 * 8.0f, 270.0f, 65.0f, 80);
	CP_Image_Draw(credit_box, Credits_WindowWidth_div16 * 13.0f, Credits_WindowHeight_div16 * 8.0f, 270.0f, 65.0f, 80);
	CP_Image_Draw(credit_button, Credits_WindowWidth_div16, Credits_WindowHeight_div16 , Credit_Button_Width, Credit_Button_Height, 255);
	

	CP_Settings_Fill(POO_LIGHT_BROWN);
	CP_Settings_TextSize(40);
	CP_Font_DrawText(copyright, Credits_WindowWidth_div16 * 8.0f, Credits_WindowHeight_div16 * 1.0f);


	CP_Settings_Fill(POO_BROWN);
	CP_Settings_TextSize(45);
	CP_Font_DrawText(subtitle1, Credits_WindowWidth_div16 * 3.0f, Credits_WindowHeight_div16 * 8.0f);
	CP_Font_DrawText(subtitle2, Credits_WindowWidth_div16 * 8.0f, Credits_WindowHeight_div16 * 8.0f);
	CP_Font_DrawText(subtitle3, Credits_WindowWidth_div16 * 13.0f, Credits_WindowHeight_div16 * 8.0f);


	CP_Settings_TextSize(60);
	CP_Font_DrawText(line1, Credits_WindowWidth_div16 * 3.0f, Credits_WindowHeight_div16 * 10.0f);
	CP_Font_DrawText(line2, Credits_WindowWidth_div16 * 3.0f, Credits_WindowHeight_div16 * 11.0f);
	CP_Font_DrawText(line3, Credits_WindowWidth_div16 * 3.0f, Credits_WindowHeight_div16 * 12.0f);
	CP_Font_DrawText(line4, Credits_WindowWidth_div16 * 3.0f, Credits_WindowHeight_div16 * 13.0f);


	CP_Font_DrawText(line5, Credits_WindowWidth_div16 * 8.0f, Credits_WindowHeight_div16 * 10.0f);
	CP_Font_DrawText(line6, Credits_WindowWidth_div16 * 8.0f, Credits_WindowHeight_div16 * 11.0f);


	CP_Font_DrawText(line7, Credits_WindowWidth_div16 * 13.0f, Credits_WindowHeight_div16 * 10.0f);
}

/*!
@brief Exits the Credits
*//*________________________________________________________________________
_*/
void Credits_Exit()
{
	CP_Font_Set(CP_Font_GetDefault());
}