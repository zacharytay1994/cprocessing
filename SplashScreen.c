#include "SplashScreen.h"
#include "Scene.h"
#include <stdio.h>

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
	return;
}

void SplashScreen_Update(const float dt)
{
	splashscreen_timer += dt;
	if (splashscreen_fade_toggle == 0 && splashscreen_timer > 0.001f)
	{
		splashscreen_timer -= 0.001f;
		splashscreen_fade_amount_white += 2;
		if (splashscreen_fade_amount_white >= 255)
		{
			splashscreen_fade_toggle = 1;
			splashscreen_timer = 0;
		}
	}
	else if (splashscreen_fade_toggle == 1 && splashscreen_timer > 3)
	{
		splashscreen_fade_toggle = 2;
		splashscreen_timer = 0;
	}
	else if (splashscreen_fade_toggle == 2 && splashscreen_timer > 0.001f)
	{
		splashscreen_timer -= 0.001f;
		splashscreen_fade_amount_white -= 3;
		if (splashscreen_fade_amount_white <= 0)
		{
			Scene_ChangeScene(3);
		}
	}
	SplashScreen_Render();
	return;
}

void SplashScreen_Render()
{
	CP_Settings_Background(CP_Color_Create(splashscreen_fade_amount_white, splashscreen_fade_amount_white, splashscreen_fade_amount_white, 255));
	CP_Image_Draw(DigiPen_Logo, splashscreen_window_width * 0.5f, splashscreen_window_height * 0.5f, DigiPen_Logo_width, DigiPen_Logo_height, 255);
	return;
}

void SplashScreen_Exit()
{
	CP_Image_Free(&DigiPen_Logo);
	return;
}