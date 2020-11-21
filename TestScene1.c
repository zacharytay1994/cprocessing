#include "TestScene1.h"
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>
#include "PhyObj.h"


PhyObjBoundingCircle* test_circle;

char curr_Timer[127];
char wave_status[127];
double timer;
double interval_counter;
double wave_counter;
const double wave_duration = 10;
const double interval_delay = 5;
float YspawnRange;
int is_interval;
int is_wave;

void TestScene1_Init()
{
	printf("Scene1 Initialized\n");

	//temp var for wind size to shorten the call
	wind_Width = (float)CP_System_GetWindowWidth();
	wind_Height = (float)CP_System_GetWindowHeight();

	// Scene Button GUI Temp init 
	TestScene1_BtnInit();
	Sprite_Initialize();

	//Inventory_Init();
	CP_Font_DrawText("Wave Timer: ", wind_Width + 50.f, wind_Height - 50.f);

	Enemy_Initialize();
	PhyObj_Initialize();

	//Temp House stuff to check collision
	house_posX = wind_Width / 2.7f;
	house_posY = wind_Height / 2.2f;
	house_SizeX = wind_Width / 7.f;
	house_SizeY = wind_Height / 5.f;


	test_circle = PhyObj_AddCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 0.f, 30.f, 1.f);
	test_circle->super._visible = 1;

	//Temp Gameplay vars
	houseHP = 100.f;
	timer = 0;
	interval_counter = 0;
	wave_counter = 0;
	is_interval = 1;
	is_wave = 0;
	

	tempHouseHP_spriteId = Sprite_AddSprite(
		(CP_Vector) {house_posX - 250.f, house_posY-130},
		houseHP * 10.f,30,
		"half_redBox.png",
		1,1,1,1,0);

	tempHouseSprite_id = Sprite_AddSprite(
		(CP_Vector) {house_posX, house_posY},
		house_SizeX,
		house_SizeY,
		"demo_house.png",
		1,1,1,1,0);

}

void TestScene1_Update(const float dt)
{
	//Checks for keyboard input
	KeyInputAssign();

	//TEST BALL FOR COLLISION
	test_circle->super._position.x = CP_Input_GetMouseX();
	test_circle->super._position.y = CP_Input_GetMouseY();
	//printf("ballRad: %f\n", test_circle->_radius);

	/*Check if enemy collide with house. 
	PS. Cant really think of 
		any ways to check which is what enemy at the moment. */
	for (int i = 0; i < sizeof(enemy_list)-1; ++i)
	{
		if (CheckEnemyAlive(i) == 1)
		{
			//HOUSE COLLISION
			if (CheckEnemyCollision(house_posX + house_SizeX / 2, house_posY + house_SizeY / 2,
				house_posX - house_SizeX / 2, house_posY - house_SizeY / 2, i) == 1)
			{
				//SetEnemyDie(i);
				SetEnemySpeed(i, 0.f);
				houseHP -= dt * (float)(GetEnemyDMG(i));
				Sprite_SetWidth(tempHouseHP_spriteId, houseHP * 10.f);
			}
			//TEST BALL COLLISION
			if (CheckEnemyCollision(test_circle->super._position.x + test_circle->_radius,
				test_circle->super._position.y + test_circle->_radius,
				test_circle->super._position.x - test_circle->_radius,
				test_circle->super._position.y - test_circle->_radius, i) == 1)
			{
				//SetEnemyDie(i);
				SetEnemyHP(i, GetEnemyHP(i) - dt*10.f);
			}
		}
	}

	//Auto Spawn Enemy
	{
		/*WAVE SYSTEM:
			CreateWave 1 :
				- Spawn enemy type 1 every N seconds		   time X ---->
				- Spawn for X duration.						   x   x   x   x   x   x
					- When X duration up, end wave
				
			CreateWave 2 : 
				- Spawn enemy type 1 every N seconds		   time X ---->
				- Spawn enemy type 2 every P seconds		   x   xY   x   xY   x   xY
					- When X duration up, end wave
			
			CreateWave 3 :
				- Spawn enemy type 1 every N seconds		   time X ---->
				- Spawn enemy type 2 every P seconds		   x   xY   x   xY   x   xY
				- After 'A' secs, begin type 3 spawning		           Z
				- Enemy type 3 spawn randomly 				   ------->
					- btwn v - u range of secs					'A' secs
					- When X duration up, end wave

			*/

	}

	if (is_interval == 1)
	{
		if (interval_counter <= 0)
		{
			interval_counter = interval_delay;
			is_interval = 0;
		}
		interval_counter -= dt;
		sprintf_s(wave_status, 127, "(DAY) time left: %.0f", interval_counter);
		CP_Font_DrawText(wave_status, 350, 50);
	}
	else	// not interval, spawning enemy waves
	{
		if (wave_counter <= 0)
		{
			wave_counter = wave_duration;
			is_interval = 1;
		}
		wave_counter -= dt;
		sprintf_s(wave_status, 127, "(NIGHT) time left: %.0f", wave_counter);
		CP_Font_DrawText(wave_status, 350, 50);
	}

	// Time
	timer += dt;
	sprintf_s(curr_Timer, 127, "Time: %.0f", timer);
	CP_Font_DrawText(curr_Timer, 20, 50);

	// Misc Updates
	UpdateEnemy(dt);

	//Spawn Enemy Waves
	YspawnRange = CP_Random_RangeFloat((wind_Height / 2) + 200.f, (wind_Height / 2) - 200.f);
	SpawnEnemyWaves(dt);

	//printf("Scene1 updating\n");
	Button_Update();
	Sprite_RenderSprite(dt, tempHouseSprite_id);
	Sprite_RenderSprite(dt, tempHouseHP_spriteId);
	GUIRender();
	//Inventory_Render();
	Camera_Update(dt);
	PhyObj_Update(dt);
	PhyObj_Render();
}

void KeyInputAssign()
{
	
	//float lowYspawn = CP_Random_RangeFloat();
	// Debug Spawn VitC
	if (CP_Input_KeyReleased(KEY_I)) {
		CreateEnemy(10.f,
			(CP_Vector){wind_Width/1.1f,YspawnRange},
			(CP_Vector){100.f,100.f},
			100.f, 0);
	}
	//Debug Spawn NoOxy
	if (CP_Input_KeyReleased(KEY_O))
	{
		CreateEnemy(50.f,
			(CP_Vector){wind_Width / 1.1f,YspawnRange},
			(CP_Vector){100.f,100.f},
			50.f, 1);
	}
	//Debug spawn lateGuy
	if (CP_Input_KeyReleased(KEY_P))
	{
		CreateEnemy(10.f,
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


	////Inventory
	//if (CP_Input_KeyDown(KEY_TAB))
	//{
	//	inventory_is_visible = 1;
	//}
	//else
	//{
	//	inventory_is_visible = 0;
	//}

	if (CP_Input_MouseClicked())
	{
		//test_circle = PhyObj_AddCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 5.f, 5.f, 1.f);
		//test_circle->super._visible = 1;
		//PhyObj_AddCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 30.f, 30.f, 1.f)->super._visible = 1;
	}
}

//Gameplay Nonsense
void SpawnEnemyWaves(const float dt)
{
	if(is_interval == 0)
		SpawnWave_1(dt);
	//SpawnWave_2(dt);
	//SpawnWave_3(dt);
}

double spawndelay = 2;
void SpawnWave_1(const float dt)
{
	
	if (spawndelay <= 0.0)
	{
		CreateEnemy(10.f,
			(CP_Vector){wind_Width/1.1f,YspawnRange},
			(CP_Vector){100.f,100.f},
			100.f, 0);

		spawndelay = 2;
	}
	spawndelay -= dt;

}

void SpawnWave_2(const float dt)
{

}

void SpawnWave_3(const float dt)
{

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
		Button_Active_Set(btn_closePopup, 0);
		Button_Active_Set(btn_popupWind, 1);
		printf("Clicked Btn ID: %d\n", btn_closePopup);
	}
	else
	{
		mainGUI_isOpen = 1;
		Button_Active_Set(btn_closePopup, 1);
		Button_Active_Set(btn_popupWind, 0);
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
 