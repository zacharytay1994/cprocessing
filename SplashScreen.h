#pragma once
#include <cprocessing.h>

CP_Image DigiPen_Logo;
int		splashscreen_fade_amount_white;
char	splashscreen_fade_toggle;
float	splashscreen_timer;
float	splashscreen_window_width;
float	splashscreen_window_height;
float	DigiPen_Logo_width;
float	DigiPen_Logo_height;

void SplashScreen_Init();

void SplashScreen_Update(const float dt);

void SplashScreen_Render();

void SplashScreen_Exit();