//---------------------------------------------------------
// file:	ParallaxBackground.c
// author:	Jia Min
// email:	j.jiamin@digipen.edu
//
// brief:	Renders parallax background with for the scene
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "ParallaxBackground.h"
#include "LightStage.h"

CP_Image BG_Pic_PurpleSky_1, BG_Pic_PurpleSky_2, BG_Pic_Mountain_1, BG_Pic_Mountain_2, BG_Pic_Sun, BG_Pic_DarkPurpleSky, BG_Pic_Cloud_1, BG_Pic_Cloud_2, BG_Pic_PurpleOrangeSky, BG_Pic_OrangeYellowSky;
float t1_x, t2_x, t3_x, t4_x, t5_x, t5_xx, t6_x, t7_x, t8_x, t9_x, t10_x;
float BG_WindowWidth;
float BG_WindowHeight;
float BG_Alpha_Background = 255.0f;
float BG_Alpha_Background1 = 0.0f;
float BG_Alpha_Background2 = 255.0f;

int BG_sun_set = 1;

float BG_light_ambient = 0.0f;

void PB_Initialize()
{
	BG_Pic_PurpleSky_1 = CP_Image_Load("Photos/Parallax_Scrolling_Scene-01.png"); // Purple Background (Part 1/2) 
	BG_Pic_PurpleSky_2 = CP_Image_Load("Photos/Parallax_Scrolling_Scene-02.png"); // Purple Background (Part 2/2) 
	BG_Pic_Mountain_1 = CP_Image_Load("Photos/Parallax_Scrolling_Scene-03.png"); // Mountain (Part 1/2)
	BG_Pic_Mountain_2 = CP_Image_Load("Photos/Parallax_Scrolling_Scene-04.png"); // Mountain (Part 2/2)
	BG_Pic_Sun = CP_Image_Load("Photos/Parallax_Scrolling_Scene-05.png"); // Sun (Part 1/1)
	BG_Pic_DarkPurpleSky = CP_Image_Load("Photos/Parallax_Scrolling_Scene-06.png"); // Dark Purple Background (Part 1/1)
	BG_Pic_Cloud_1 = CP_Image_Load("Photos/Parallax_Scrolling_Scene-07.png"); // Cloud (Part 1/2)
	BG_Pic_Cloud_2 = CP_Image_Load("Photos/Parallax_Scrolling_Scene-08.png"); // Cloud (Part 2/2)
	BG_Pic_PurpleOrangeSky = CP_Image_Load("Photos/Parallax_Scrolling_Scene-09.png"); // Gradient colour from purple to orange (Part 1 / 1 )
	BG_Pic_OrangeYellowSky = CP_Image_Load("Photos/Parallax_Scrolling_Scene-10.png"); // Gradient colour from orange to yellow (Part 1 / 1 )

	BG_WindowWidth = (float)CP_System_GetWindowWidth() * 2.0f;
	BG_WindowHeight = (float)CP_System_GetWindowHeight();

	t1_x = BG_WindowWidth / 2.0f;
	t2_x = -(BG_WindowWidth / 2.0f);
	t3_x = BG_WindowWidth / 2.0f;
	t4_x = -(BG_WindowWidth / 2.0f);
	t5_x = BG_WindowHeight / 2.0f;
	t5_xx = BG_WindowWidth / 4.0f;
	t6_x = BG_WindowWidth / 4.0f;
	t7_x = BG_WindowWidth / 2.0f;
	t8_x = -(BG_WindowWidth / 2.0f);
	t9_x = BG_WindowWidth / 2.0f;
	t10_x = BG_WindowWidth / 2.0f;
}

void PB_Update(const float dt)
{
	if (t1_x >= BG_WindowWidth * 1.5f)
	{
		t1_x = -(BG_WindowWidth / 2.0f);
	}

	if (t2_x >= BG_WindowWidth * 1.5f)
	{
		t2_x = -(BG_WindowWidth / 2.0f);
	}

	if (t3_x >= BG_WindowWidth * 1.5f)
	{
		t3_x = -(BG_WindowWidth / 2.0f);
	}
	else if (t3_x <= -BG_WindowWidth * 0.5f)
	{
		t3_x = BG_WindowWidth * 1.5f;
	}

	if (t4_x >= BG_WindowWidth * 1.5f)
	{
		t4_x = -(BG_WindowWidth / 2.0f);
	}
	else if (t4_x <= -BG_WindowWidth * 0.5f)
	{
		t4_x = BG_WindowWidth * 1.5f;
	}

	if (t5_x >= BG_WindowHeight * 1.5f)
	{
		t5_x = -(BG_WindowHeight / 2.0f);
	}

	if (t7_x >= BG_WindowWidth * 1.5f)
	{
		t7_x = -(BG_WindowWidth / 2.0f);
	}

	if (t8_x >= BG_WindowWidth * 1.5f)
	{
		t8_x = -(BG_WindowWidth / 2.0f);
	}


	else
	{

		if (BG_sun_set) {
			if (BG_Alpha_Background <= 0.0f) {
				// dark purple sky increase
				BG_Alpha_Background1 += 25.0f * CP_System_GetDt();
				if (BG_Alpha_Background1 > 255.0f) {
					BG_sun_set = 0;
				}
			}
			else {
				// purple sky decrease
				BG_Alpha_Background -= 10.0f * CP_System_GetDt();
			}
			BG_light_ambient = (BG_Alpha_Background1 - BG_Alpha_Background + 255.0f) / 2.0f;
		}
		else {
			if (BG_Alpha_Background1 > 0.0f) {
				// dark purple sky decrease
				BG_Alpha_Background1 -= 25.0f * CP_System_GetDt();
			}
			else {
				// purple sky increase
				BG_Alpha_Background += 10.0f * CP_System_GetDt();
				if (BG_Alpha_Background > 255.0f) {
					BG_sun_set = 1;
				}
			}
			BG_light_ambient = (BG_Alpha_Background1 - BG_Alpha_Background + 255.0f) / 2.0f;
		}
		LightStage_SetAmbient((int)BG_light_ambient - 100);

		t1_x += 125.0f * CP_System_GetDt();
		CP_Image_Draw(BG_Pic_PurpleSky_1, t1_x, BG_WindowHeight / 2.0f, BG_WindowWidth, BG_WindowHeight, (int)BG_Alpha_Background);
		t2_x += 125.0f * CP_System_GetDt();
		CP_Image_Draw(BG_Pic_PurpleSky_2, t2_x, BG_WindowHeight / 2.0f, BG_WindowWidth, BG_WindowHeight, (int)BG_Alpha_Background);
		CP_Image_Draw(BG_Pic_DarkPurpleSky, t6_x, BG_WindowHeight / 2.0f, BG_WindowWidth, BG_WindowHeight, (int)BG_Alpha_Background1);

		t7_x += 100.0f * CP_System_GetDt();
		CP_Image_Draw(BG_Pic_Cloud_1, t7_x, BG_WindowHeight / 2.0f, BG_WindowWidth, BG_WindowHeight, 200);

		t8_x += 100.0f * CP_System_GetDt();
		CP_Image_Draw(BG_Pic_Cloud_2, t8_x, BG_WindowHeight / 2.0f, BG_WindowWidth, BG_WindowHeight, 200);

		t5_x += 5.0f * CP_System_GetDt();
		t5_xx += 5.0f * CP_System_GetDt();
		CP_Image_Draw(BG_Pic_Sun, t5_xx, t5_x, BG_WindowWidth, BG_WindowHeight, 50);

		if (CP_Input_KeyDown(KEY_LEFT))
		{
			t3_x += 200.0f * CP_System_GetDt();
			t4_x += 200.0f * CP_System_GetDt();
		}

		if (CP_Input_KeyDown(KEY_RIGHT))
		{
			t3_x -= 200.0f * CP_System_GetDt();
			t4_x -= 200.0f * CP_System_GetDt();
		}

		CP_Image_Draw(BG_Pic_Mountain_1, t3_x, BG_WindowHeight / 2.0f, BG_WindowWidth, BG_WindowHeight, 255);

		CP_Image_Draw(BG_Pic_Mountain_2, t4_x, BG_WindowHeight / 2.0f, BG_WindowWidth, BG_WindowHeight, 255);
	}
}

void PB_Exit()
{
}
