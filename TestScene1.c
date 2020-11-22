#include "TestScene1.h"
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>


void TestScene1_Init()
{
	printf("Scene1 Initialized\n");

	//temp var for wind size to shorten the call
	wind_Width = (float)CP_System_GetWindowWidth();
	wind_Height = (float)CP_System_GetWindowHeight();

	// Scene Button GUI Temp init 
	TestScene1_BtnInit();
	Sprite_Initialize();

	//Temp House stuff to check collision
	house_posX = wind_Width / 2.7f;
	house_posY = wind_Height / 2.2f;
	house_SizeX = wind_Width / 7.f;
	house_SizeY = wind_Height / 5.f;



	tempHouseSprite_id = Sprite_AddSprite(
		(CP_Vector) {house_posX, house_posY},
		house_SizeX,
		house_SizeY,
		"demo_house.png",
		1,
		1,
		1,
		1,
		0);

}

void TestScene1_Update(const float dt)
{
	//Checks for keyboard input
	KeyInputAssign();

	/*Check if enemy collide with house. PS. Cant really think of 
		any ways to check which is what enemy at the moment. */
	for (int i = 0; i < sizeof(enemy_list)-1; ++i)
	{
		if (CheckEnemyAlive(i) == 1)
		{
			if (CheckEnemyCollision(house_posX + house_SizeX / 2, house_posY + house_SizeY / 2,
				house_posX - house_SizeX / 2, house_posY - house_SizeY / 2, i) == 1)
			{
				SetEnemyDie(i);
				//enemy_list[i].isAlive = 0;
			}
		}
	}
	// Misc Updates
	UpdateEnemy(dt);
	//printf("Scene1 updating\n");
	Button_Update();
	Sprite_RenderSprite(dt, tempHouseSprite_id);
	GUIRender();
	Camera_Update(dt);
}

void KeyInputAssign()
{
	float YspawnRange = CP_Random_RangeFloat((wind_Height/2)+200.f, (wind_Height / 2) - 200.f);
	//float lowYspawn = CP_Random_RangeFloat();
	// Debug Spawn VitC
	if (CP_Input_KeyReleased(KEY_I)) {
		CreateEnemy(10,
			(CP_Vector){wind_Width/1.1f,YspawnRange},
			(CP_Vector){100.f,100.f},
			100.f, 0);
	}
	//Debug Spawn NoOxy
	if (CP_Input_KeyReleased(KEY_O))
	{
		CreateEnemy(70,
			(CP_Vector){wind_Width / 1.1f,YspawnRange},
			(CP_Vector){100.f,100.f},
			50.f, 1);
	}
	//Debug spawn lateGuy
	if (CP_Input_KeyReleased(KEY_P))
	{
		CreateEnemy(10,
			(CP_Vector){wind_Width / 1.1f, YspawnRange},
			(CP_Vector){100.f,100.f},
			200.f, 2);
	}
	//free key input
	if (CP_Input_KeyReleased(KEY_J)) {
		/*CreateEnemy(10,
			(CP_Vector){wind_Width/1.1f,YspawnRange},
			(CP_Vector){100.f,100.f},
			100.f, 0);*/
	}
}

// in-Game UI Stuffs
void TestScene1_BtnInit()
{
	
	// Scene Button GUI Temp init 
	//################ MAIN BTN ################
	CP_Vector test_GUI_pos;
	test_GUI_pos.x = wind_Width / 1.17f;
	test_GUI_pos.y = wind_Height / 16.f;

	CP_Vector test_GUI_size;
	test_GUI_size.x = wind_Width / 10.f;
	test_GUI_size.y = wind_Height / 11.f;

	CP_Vector test_GUI_textpos;
	test_GUI_textpos.x = test_GUI_pos.x + 8;
	test_GUI_textpos.y = test_GUI_pos.y + 45;

	float test_GUI_textsize = 50.f;

	btn_popupWind = Button_Initialize(
		test_GUI_pos,
		test_GUI_size,
		test_GUI_textpos,
		CP_Color_Create(255, 255, 255, 255),
		CP_Color_Create(0, 0, 0, 255),
		test_GUI_textsize,
		"Settings",
		1
	);

	//############# CLOSE SUB BTN #############

	btn_closePopup = Button_Initialize(
		test_GUI_pos,
		(CP_Vector) {wind_Width,wind_Height/10},
		test_GUI_textpos,
		CP_Color_Create(255, 50, 50, 255),
		CP_Color_Create(0, 0, 0, 255),
		test_GUI_textsize,
		" ",
		1
	);

	mainGUI_isOpen = 0;	//closed
}

void TestScene1_BtnManager()
{
	if (mainGUI_isOpen)
	{
		mainGUI_isOpen = 0;
		printf("Clicked Btn ID: %d\n", btn_closePopup);
	}
	else
	{
		mainGUI_isOpen = 1;
		printf("Clicked Btn ID: %d\n", btn_popupWind);
	}
}

void GUIRender()
{
	if (mainGUI_isOpen)
	{
		Button_Render(btn_closePopup);
		
	}
	else
	{
		Button_Render(btn_popupWind);
		
	}
}
// End of in-Game UI Stuff

void TestScene1_Exit()
{
	printf("Scene1 exited\n");
	Sprite_Free();
}
 