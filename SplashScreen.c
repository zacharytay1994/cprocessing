/*!
@file       SplashScreen.c
@author     Noel Ho Sing Nam (s.ho)
@course     CSD1400
@section    A
@brief      Handles the splashscreen fade effect and rendering
*//*__________________________________________________________________________
_*/
#include "SplashScreen.h"
#include "Scene.h"

/*!
@brief Initializes the splashscreen data
*//*________________________________________________________________________
_*/
void SplashScreen_Init()
{
	DigiPen_Logo = CP_Image_Load("Assets/DigiPen_BLACK.png");
	DigiPen_Logo_width = (float)CP_Image_GetWidth(DigiPen_Logo);
	DigiPen_Logo_height = (float)CP_Image_GetHeight(DigiPen_Logo);

	splashscreen_fade_toggle = 0;
	splashscreen_fade_amount_white = 0;
	splashscreen_timer = 0;

	splashscreen_window_width = (float)CP_System_GetWindowWidth();
	splashscreen_window_height = (float)CP_System_GetWindowHeight();
}

/*!
@brief Update function for splashscreen
@param  dt				- deltaTime framerate
*//*________________________________________________________________________
_*/
void SplashScreen_Update(const float dt)
{
	splashscreen_timer += dt *10;
	if (splashscreen_fade_toggle == 0 && splashscreen_timer > 0.01f)
	{
		splashscreen_timer -= 0.001f * 10.0f;
		splashscreen_fade_amount_white += 2 * 10;
		if (splashscreen_fade_amount_white >= 255)
		{
			splashscreen_fade_toggle = 1;
			splashscreen_timer = 0;
		}
	}
	else if (splashscreen_fade_toggle == 1 && splashscreen_timer > 30)
	{
		splashscreen_fade_toggle = 2;
		splashscreen_timer = 0;
	}
	else if (splashscreen_fade_toggle == 2 && splashscreen_timer > 0.01f)
	{
		splashscreen_timer -= 0.001f * 10.0f;
		splashscreen_fade_amount_white -= 3 * 10;
		if (splashscreen_fade_amount_white <= 0)
		{
			Scene_ChangeScene(3);
		}
	}
	SplashScreen_Render();
	//return;
}

/*!
@brief Renders the splashscreen
*//*________________________________________________________________________
_*/
void SplashScreen_Render()
{
	CP_Settings_Background(CP_Color_Create(splashscreen_fade_amount_white, splashscreen_fade_amount_white, splashscreen_fade_amount_white, 255));
	CP_Image_Draw(DigiPen_Logo, splashscreen_window_width * 0.5f, splashscreen_window_height * 0.5f, DigiPen_Logo_width, DigiPen_Logo_height, 255);
	//return;
}

/*!
@brief Exits the splashscreen
*//*________________________________________________________________________
_*/
void SplashScreen_Exit()
{
	//CP_Image_Free(&DigiPen_Logo);
	//return;
}