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
}

void TestScene1_Update(const float dt)
{
	// Debug Spawn VitC
	if (CP_Input_KeyReleased(KEY_I)) {
		CreateEnemy(10,
			(CP_Vector){(float)CP_System_GetWindowWidth()/1.1f,(float)CP_System_GetWindowHeight() / 2},
			(CP_Vector){100.f,100.f},
			100.f, 0);
	}
	//Debug Spawn NoOxy
	if (CP_Input_KeyReleased(KEY_O))
	{
		CreateEnemy(10,
			(CP_Vector){(float)CP_System_GetWindowWidth() / 1.1f,(float)CP_System_GetWindowHeight() / 2},
			(CP_Vector){100.f,100.f},
			100.f, 1);
	}
	//Debug spawn lateGuy
	if (CP_Input_KeyReleased(KEY_P))
	{
		CreateEnemy(10,
			(CP_Vector){(float)CP_System_GetWindowWidth() / 1.1f,(float)CP_System_GetWindowHeight() / 2},
			(CP_Vector){100.f,100.f},
			100.f, 2);
	}
	if (CP_Input_KeyDown(KEY_J)) {
		printf("myBtn ID: %d", btn_popupWind.Id);
	}

	//if(Button_Mouse_Collision_Click_ById(btn_popupWind))

	// Misc Updates
	UpdateEnemy(dt);
	//printf("Scene1 updating\n");
	Button_Update();
	
	GUIRender();
	Camera_Update(dt);
}

// Enemy Stuff - TODO (Move to Enemy class)
void CreateEnemy(int hp, CP_Vector position, CP_Vector size, float speed, int enemy_type)
{
	struct Enemy new_enemy;

	// Enemy Type Dependant variables
	char *path;
	int enem_sprite_col;
	int enem_sprite_row;
	int enem_sprite_frames;
	int enem_sprite_animate_speed;
	

	// Basic Enemy Variables
	new_enemy.health = hp;
	new_enemy.position = position;
	new_enemy.speed = speed;
	new_enemy.enem_Size = size;

	switch (enemy_type)
	{
	case 0:	// VitaminC
	{
		path = ("demo_enemy.png");
		enem_sprite_col = 1;
		enem_sprite_row = 1;
		enem_sprite_frames = 1;
		enem_sprite_animate_speed = 1;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };
		
		break;
	}
	case 1:	// NoOxygen
	{
		path = ("demo_enemy2.png");
		new_enemy.enem_Size.x = size.x * 2.5f;
		new_enemy.enem_Size.y = size.y * 2.5f;
		enem_sprite_col = 1;
		enem_sprite_row = 1;
		enem_sprite_frames = 1;
		enem_sprite_animate_speed = 1;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };
		
		break;
	}
	case 2:	// Late4Class
	{
		path = ("demo_player.png");
		new_enemy.enem_Size.x = size.x * 1.2f;
		new_enemy.enem_Size.y = size.y * 1.2f;
		enem_sprite_col = 8;
		enem_sprite_row = 1;
		enem_sprite_frames = 8;
		enem_sprite_animate_speed = 50;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };
		
		break;
	}
	default:	// ???
	{
		path = ("demo_enemy.png");
		enem_sprite_col = 1;
		enem_sprite_row = 1;
		enem_sprite_frames = 1;
		enem_sprite_animate_speed = 1;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };
		
		break;
	}
	}

	// Assign Enemy Sprite
	new_enemy.ene_sprite_id = Sprite_AddSprite(
		new_enemy.position, 
		-(new_enemy.enem_Size.x), 
		new_enemy.enem_Size.y, 
		path, 
		enem_sprite_col,
		enem_sprite_row,
		enem_sprite_frames, 
		enem_sprite_animate_speed);

	new_enemy.isAlive = 1;	//1 - alive, 0 - dead
	printf("NEWeneScaleX: %2f, NEWeneScaleY: %2f\n", (new_enemy.enem_HitboxScale.x), (new_enemy.enem_HitboxScale.y));
	// Add enemy to list
	new_enemy.ene_id = Add_Enem_toList(&new_enemy);
	//Debug
	printf("Enemy created at x: %f, y: %f\n", new_enemy.position.x, new_enemy.position.y);
}

int Add_Enem_toList(struct Enemy* add_enem)
{
	for (int i = 0; i < 127; i++)
	{
		if (enemy_list[i].isAlive == 0)	
		{
			enemy_list[i] = *add_enem;
			enemy_list[i].ene_id = i;
			// Debug
			printf("Enemy added\n");
			return i;
		}
	}
	return 0;
}

void UpdateEnemy(const float dt)
{
	for (int i = 0; i < 127; ++i)
	{
		if (enemy_list[i].isAlive == 0)
		{
			// if Enemy is "dead", set sprite visible off
			Sprite_SetVisible(enemy_list[i].ene_sprite_id, 1);
			continue;
		}
		else
		{
			//Update Movement/sprite movement 
			enemy_list[i].position.x -= dt * (enemy_list[i].speed);
			Sprite_SetPosition(enemy_list[i].ene_sprite_id, enemy_list[i].position);
			
			//Only render alive enemies
			Sprite_RenderSprite(dt, enemy_list[i].ene_sprite_id);
			
			//if out of map (left boundaries only, not like enemy gonna move right...right?)
			if (CheckEnemyCollision(0.f, wind_Height, -10.f, 0.f, i) == 1)
			{
				enemy_list[i].isAlive = 0;
			}
		}

	}
}

int CheckEnemyCollision(float maxPos_X, float maxPos_Y, float minPos_X, float minPos_Y, int enemy_id)
{
	// Get enemy max hitbox boundery
	float maxhitBox_posX = enemy_list[enemy_id].position.x + (enemy_list[enemy_id].enem_HitboxScale.x / 2.f);
	float minhitBox_posX = enemy_list[enemy_id].position.x - (enemy_list[enemy_id].enem_HitboxScale.x / 2.f);
	// Get enemy min hitbox boundary
	float maxhitBox_posY = enemy_list[enemy_id].position.y + (enemy_list[enemy_id].enem_HitboxScale.y / 2.f);
	float minhitBox_posY = enemy_list[enemy_id].position.y - (enemy_list[enemy_id].enem_HitboxScale.y / 2.f);

	// Compares objbox with enemy hitbox
	if (maxPos_X >= minhitBox_posX &&
		maxPos_Y >= minhitBox_posY)
	{
		return 1;
	}
	else if (minPos_X <= maxhitBox_posX &&
		minPos_Y >= maxhitBox_posY)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
// End of Enemy Stuff

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
		printf("Clicked Btn ID: %d\n", btn_closePopup.Id);
	}
	else
	{
		mainGUI_isOpen = 1;
		printf("Clicked Btn ID: %d\n", btn_popupWind.Id);
	}
}

void GUIRender()
{
	if (mainGUI_isOpen)
	{
		Button_Render(btn_closePopup.Id);
		
	}
	else
	{
		Button_Render(btn_popupWind.Id);
		
	}
}
// End of in-Game UI Stuff

void TestScene1_Exit()
{
	printf("Scene1 exited\n");
}
 