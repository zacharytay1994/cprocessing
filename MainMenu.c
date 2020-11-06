#include <stdlib.h>
#include <stdio.h>
#include "MainMenu.h"

void MainMenu_Initialize()
{
	button_start = Button_Initialize_Default();
	Button_Translate(button_start.Id, 0, -50);
	Button_Text_Translate(button_start.Id, 0, -50);
	Button_Text_Set(button_start.Id, "Start");
	button_credits = Button_Initialize_Default();
	Button_Text_Set(button_credits.Id, "Credits");
	button_exit = Button_Initialize_Default();
	Button_Translate(button_exit.Id, 0, 50);
	Button_Text_Translate(button_exit.Id, 0, 50);
	Button_Text_Set(button_exit.Id, "Exit");
	Button_Image_Set_Override(button_exit.Id, "ground_block.png");
	Button_SpecialEffects_Set(button_exit.Id, 1);
	
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
	Button_Render_All();
	return;
}

void MainMenu_Exit()
{

}