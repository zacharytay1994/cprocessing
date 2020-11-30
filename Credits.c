#include "Credits.h"
#include <stdio.h>

void Credits_Init()
{
	Credits_WindowWidth = CP_System_GetWindowWidth();
	Credits_WindowHeight = CP_System_GetWindowHeight();

	sprintf_s(copyright, 255, "All content (c) 2020 DigiPen Institute of Technology Singapore, all rights reserved.");
	sprintf_s(title1, 127, "CREDITS");

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

	time_buffer = 2;

	custom_font = CP_Font_Load("Assets/Fonts/zrnic rg.ttf");
	CP_Font_Set(custom_font);
}

void Credits_Update(const float dt)
{
	if (time_buffer > 0)
	{
		time_buffer -= dt;
	}
	else
	{
		if (CP_Input_MouseClicked())
		{
			Scene_ChangeScene(3);
		}
	}

	Credits_Render();
}

void Credits_Render()
{
	CP_Settings_Background(BLACK);
	
	CP_Settings_Fill(GREEN_TINT);
	CP_Settings_TextSize(100);
	CP_Font_DrawText(subtitle1, Credits_WindowWidth * 0.12f, Credits_WindowHeight * 0.4f);
	CP_Font_DrawText(subtitle2, Credits_WindowWidth * 0.6f, Credits_WindowHeight * 0.4f);
	CP_Font_DrawText(subtitle3, Credits_WindowWidth * 0.61f, Credits_WindowHeight * 0.7f);

	CP_Settings_Fill(WHITE);
	CP_Settings_TextSize(50);
	CP_Font_DrawText(copyright, Credits_WindowWidth * 0.08f, Credits_WindowHeight * 0.05f);

	CP_Settings_TextSize(200);
	CP_Font_DrawText(title1, Credits_WindowWidth * 0.5f - 250, Credits_WindowHeight * 0.25f);

	CP_Settings_TextSize(60);
	CP_Font_DrawText(line1, Credits_WindowWidth * 0.23f + 0, Credits_WindowHeight * 0.5f);
	CP_Font_DrawText(line2, Credits_WindowWidth * 0.23f + 5, Credits_WindowHeight * 0.57f);
	CP_Font_DrawText(line3, Credits_WindowWidth * 0.23f + 25, Credits_WindowHeight * 0.64f);
	CP_Font_DrawText(line4, Credits_WindowWidth * 0.23f + 20, Credits_WindowHeight * 0.71f);

	CP_Font_DrawText(line5, Credits_WindowWidth * 0.6f + 5, Credits_WindowHeight * 0.5f);
	CP_Font_DrawText(line6, Credits_WindowWidth * 0.6f + 45, Credits_WindowHeight * 0.57f);

	CP_Font_DrawText(line7, Credits_WindowWidth * 0.61f + 25, Credits_WindowHeight * 0.76f);
}

void Credits_Exit()
{
	CP_Font_Set(CP_Font_GetDefault());
}