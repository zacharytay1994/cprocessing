#include "TestBed.h"

#include "PhyObj.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"
#include "GUI.h"
#include "ParallaxBackground.h"
#include "Inventory.h"
#include "Particles.h"
#include "Enemy.h"
#include "LightStage.h"
#include "LePlant.h"
#include "GameGUI.h"

#include <stdio.h>
#include <stdlib.h>

#define TESTBED_ZOMBIES_MAX 100
#define TESTBED_ZOMBIES_HEALTH 5
#define TESTBED_ZOMBIES_SPEED 25.0f
#define TESTBED_ZOMBIES_ATTACK_RATE 2.0f
#define TESTBED_BOMB_SPEED 20.0f
#define TESTBED_BOMB_GRAVITY 9.81f

float cam_x = 0.0f;
float cam_y = 0.0f;

int tilemap;

float TestBed_window_width;
float TestBed_window_height;
float TestBed_testbed_icon_size = 0.02f;
// temp
CP_Image TestBed_house;
CP_Image TestBed_house_modified;
CP_Image TestBed_house_normal_map;
void* TestBed_house_data;
void* TestBed_house_modified_data;
void* TestBed_house_normal_data;
CP_Vector house_position;
CP_Vector house_top_left_pos;
int House_health;
int House_max_health = 10;
float House_heart_offset_x = 0.25f;
float House_heart_offset_y = 0.05f;
float House_heart_spacing = 60.0f;

TB_Zombie tb_zombies[TESTBED_ZOMBIES_MAX];
int tb_zombies_size = 0;
int tb_zombies_init = 0;
int tb_zombie_resource;
CP_Vector tb_zombie_spawn_position;

//TB_Bomb tb_bombs[TESTBED_ZOMBIES_MAX];
//int tb_bombs_size = 0;
//int tb_bombs_init = 0;
//int tb_bombs_resource;

float tb_zombie_spawn = 3.0f;
float tb_temp = 3.0f;
int tb_check = 0;

int particle_hold = 0;

// Wave Temp - (RAY)
// Display stuff
char curr_Timer[127];
char money_display[127];
char wave_status[127];
char wave_display[127];
double timer;
double interval_counter;	// interval means "day time"
double wave_timer;
const double wave_duration = 15;	//night time duration
const double interval_delay = 10;	//day time duration
int is_interval;	// 1 - no enemy spawn, 0 - start spawn enemies
int is_wave;
int wave_count;
double spawndelay = 2;
int biggus = 0;
int souls_money = 0;

float TestBed_player_invuln_timer = 1.0f;
float TestBed_player_invuln_timer_counter = 1.0f;
int TestBed_player_invuln = 0;

void TestBed_Init()
{
	printf("Switched to testbed.\n");
	TestBed_window_width = (float)CP_System_GetWindowWidth();
	TestBed_window_height = (float)CP_System_GetWindowHeight(); 
	TestBed_testbed_icon_size *= TestBed_window_width;
	// temp
	TestBed_house = CP_Image_Load("./Sprites/house.png");
	TestBed_house_data = malloc(CP_Image_GetPixelBufferSize(TestBed_house));
	wind_Width = (float)CP_System_GetWindowWidth();
	wind_Height = (float)CP_System_GetWindowHeight();
	TestBed_house_data = malloc(CP_Image_GetPixelBufferSize(TestBed_house));
	CP_Image_GetPixelData(TestBed_house, TestBed_house_data);

	TestBed_house_modified = CP_Image_CreateFromData(160, 160, TestBed_house_data);
	TestBed_house_modified_data = malloc(CP_Image_GetPixelBufferSize(TestBed_house_modified));
	CP_Image_GetPixelData(TestBed_house_modified, TestBed_house_modified_data);

	TestBed_house_normal_map = CP_Image_Load("./Sprites/house_normal.png");
	TestBed_house_normal_data = malloc(CP_Image_GetPixelBufferSize(TestBed_house_normal_map));
	CP_Image_GetPixelData(TestBed_house_normal_map, TestBed_house_normal_data);

	house_position = (CP_Vector){ 1000.0f,1045.0f };
	house_top_left_pos = CP_Vector_Subtract(house_position, (CP_Vector) { 300.0f, 300.0f });
	House_health = House_max_health;
	// temp zombie
	tb_zombie_spawn_position = (CP_Vector){ 3300.0f,1150.0f };

	// Wave Init - (RAY)
	timer = 0;
	interval_counter = 0;
	wave_timer = 0;
	is_interval = 1;
	is_wave = 0;
	wave_count = 0;
	biggus = 0;

	souls_money = 0;


	// Setting up tilemaps -
	Tilemap_Initialize();
	tilemap = Tilemap_TxtLoad256("prototype_map.txt");
	//tilemap = Tilemap_AddTilemap(64, 64, 40, 20, 0, 0);
	Tilemap_GeneratePhyObjs(tilemap);
	Player_Initialize();
	PB_Initialize();
	Inventory_Init();
	Inventory_Item_Create("Speed Up");
	Inventory_Add_Item_Name("Speed Up");
	Inventory_Item_Set_Image("Speed Up", "demo_test.png");
	Inventory_Item_Set_Description("Speed Up", "Increases speed temporarily");

	Inventory_Item_Create("Add Health Flower");
	Inventory_Item_Set_Image("Add Health Flower", "Assets/Items/AddHealthFlower.png");
	Inventory_Item_Set_Description("Add Health Flower", "Increases health by 1");
	Inventory_Add_Item_Name("Add Health Flower");

	Inventory_Item_Create("Add MaxHealth Flower");
	Inventory_Item_Set_Image("Add MaxHealth Flower", "Assets/Items/AddMaxHealthFlower.png");
	Inventory_Item_Set_Description("Add MaxHealth Flower", "Increases maximum health by 1");
	Inventory_Add_Item_Name("Add MaxHealth Flower");

	Inventory_Item_Create("Thorns");
	Inventory_Item_Set_Image("Thorns", "Assets/Items/Thorns.png");
	Inventory_Item_Set_Description("Thorns", "Lose 1 health");
	Inventory_Add_Item_Name("Thorns");

	Inventory_Item_Create("Radioactive Thorns");
	Inventory_Item_Set_Image("Radioactive Thorns", "Assets/Items/RadioThorns.png");
	Inventory_Item_Set_Description("Radioactive Thorns", "Lose 1 maximum health");
	Inventory_Add_Item_Name("Radioactive Thorns");

	BGM = CP_Sound_LoadMusic("Assets/Music/ShortLoopHalf.wav");
	CP_Sound_PlayMusic(BGM);

	Enemy_Initialize(); // Initialize enemy sprites and values- (RAY)
	Particle_Initialize();

	/*Sprite_InitData s_data = { (CP_Vector) { 100.0f,100.0f },100.0f,100.0f,"dirt_block.png",1,1,1,1,1 };
	int parent = GUI_AddRootContainer((CP_Vector) { 100.0f, 100.0f }, CP_Vector_Set(30.0f, 30.0f), s_data);
	s_data._position.x = 100.0f;
	s_data._path = "ground_block.png";
	s_data._width = 70.0f;
	s_data._height = 70.0f;
	parent = GUI_AddContainer(parent, (CP_Vector) { 200.0f, 200.0f }, CP_Vector_Set(30.0f, 30.0f), s_data);
	s_data._position.x = 100.0f;
	s_data._path = "ground_block.png";
	s_data._width = 30.0f;
	s_data._height = 30.0f;
	GUI_AddContainer(parent, (CP_Vector) { 200.0f, 200.0f }, CP_Vector_Set(30.0f, 30.0f), s_data);*/

	Camera_SetLimit(1);
	Camera_SetHorizontalLimit(CP_Vector_Set(-(float)CP_System_GetWindowWidth()/2.0f+64.0f, 1760.0f-64.0f));
	Camera_SetVerticalLimit(CP_Vector_Set(0.0f,830.0f));
	//CP_Vector position = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
	//TestBed_SpawnBomb(position);
	TestBed_SpawnZombie();
	LightStage_Initialize();
	LePlant_Init();
	LePlant_BindTilemap(tilemap);
	GameGUI_Init();
	CP_Font_Set(CP_Font_Load("Assets/Fonts/zrnic rg.ttf"));
}

void TestBed_Update(const float dt)
{
	//CP_Image_Draw(TestBed_house, (float)CP_Input_GetMouseX(), (float)CP_Input_GetMouseY(), 100.0f, 100.0f, 255);
	//// UPDATES
	PB_Update(dt);
	// temp
	// render house in the middle
	CP_Vector TestBed_house_position = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), house_position);
	//CP_Vector mouse_pos = (CP_Vector){ (float)CP_Input_GetMouseX(), (float)CP_Input_GetMouseY() };
	CP_Vector* pos = LightStage_GetLightPositionsArray();
	int pos_size = LightStage_GetLightPositionsSize();
	LightStage_ApplyNormalMap(TestBed_house_modified_data, TestBed_house_normal_data, TestBed_house_data, house_top_left_pos,
		300.0f, 300.0f, 160, 160, pos, pos_size);
	CP_Image_UpdatePixelData(TestBed_house_modified, TestBed_house_modified_data);
	//// RENDERS
	Tilemap_Render(tilemap, Camera_GetCameraTransform());
	CP_Image_Draw(TestBed_house_modified, TestBed_house_position.x, TestBed_house_position.y, 300.0f, 300.0f, 255);
	UpdateEnemy(dt);
	//Tilemap_Debug_Render(tilemap, Camera_GetCameraTransform());
	Player_Render();
	for (int j = 0; j < House_health; j++) {
		CP_Image_Draw(TestBed_house, House_heart_offset_x*TestBed_window_width + j * House_heart_spacing, House_heart_offset_y*TestBed_window_height, TestBed_testbed_icon_size, TestBed_testbed_icon_size, 255);
	}
	Player_Update(dt);
	GUI_Update(dt);
	Inventory_Update();
	// Enemy Functions - (RAY)
	DayNightManager(dt);

	// if(is_interval == 0) // if not day time, spawn enemies
	// {
	// 	if (wave_count % 10 == 0 && wave_count != 0 && biggus == 0)
	// 	{
	// 		// if wave 10 and 30% of wave duration left(cloes to end of wave)
	// 		if (wave_timer <= (wave_duration / 10 * 3))
	// 		{
	// 			// spawn miniboss
	// 			CreateEnemy(50.f,
	// 			(CP_Vector){ 2300.0f,1150.0f },
	// 			(CP_Vector){200.f,200.f},
	// 			50.f, 4);	

	// 			biggus = 1;
	// 		}
	// 	}
	// 	if (spawndelay <= 0.0)	// delay between each enemy spawn
	// 	{
	// 		CreateEnemy(2.f,
	// 			(CP_Vector){ 2300.0f,1150.0f },
	// 			(CP_Vector){200.f,200.f},
	// 			100.f, 3);	//spawn type 3 enemy(toothpaste guy)

	// 		spawndelay = CP_Random_RangeFloat(3.f/((float)wave_count*0.5f),7.f/ ((float)wave_count * 0.5f));
	// 	}
	// 	spawndelay -= dt;
	// }
	WaveUpdate(dt);

	/*Camera_SetCameraX(cam_x);
	Camera_SetCameraY(cam_y);*/


	if (CP_Input_KeyReleased(KEY_P)) {
		//TestBed_SpawnZombie();
		//Sprite_AddSprite(tb_zombie_spawn_position, 200.0f, 200.0f, "./Sprites/slime1.png", 2, 3, 6, 5, 0);
	}
	TestBed_UpdateZombies(dt);
	//TestBed_UpdateBombs(dt);
	TestBed_CheckBombOnZomb();
	/*if (CP_Input_MouseClicked()) {
		CP_Vector position = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		TestBed_SpawnBomb(position);
	}*/
	Particle_Update(dt);
	Inventory_Render();
	/*if (tb_zombie_spawn < 0.0f) {
		TestBed_SpawnZombie();
		tb_zombie_spawn = 3.0f;
	}
	else {
		tb_zombie_spawn -= dt;
	}*/
	if (tb_temp < 0.0f) {
		if (!tb_check) {
			Player_temp();
			tb_check = 1;
		}
	}
	else {
		tb_temp -= dt;
	}

	if (CP_Input_KeyReleased(KEY_J)) {
		CP_Vector world_coords = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		Particle_EmitOut(PT_Dust, world_coords, 30.0f, 20.0f, 30.0f, -30.0f, 50.0f, -50.0f, 0.5f, 0.2f, -50.0f, -80.0f, 0.2f, 0.1f, 50.0f, 5, 5, 0);
	}
	if (CP_Input_KeyReleased(KEY_K)) {
		//Particle_Reset(particle_hold);
	}
	LePlant_Render(dt);
	LightStage_Render();
	LightStage_Update(dt);
	LePlant_Update(dt);
	Tilemap_HighlightMouseTile(tilemap);
	GameGUI_Render(dt); 
	TestBed_CheckPlayerOnZomb(dt);
	LePlant_CheckBeanWithPlayerPosition(Player_GetPosition(0), 50.0f, 50.0f);
}

void TestBed_Exit()
{
	printf("Exited TestBed.");
	CP_Sound_StopAll();
	Tilemap_Free();
	PB_Exit();
	LightStage_Exit();
	LePlant_Exit();
}

void WaveUpdate(float dt)
{
	if(is_interval == 0) // if not day time, spawn enemies
	{
		// if 10th wave and no boss spawn yet
		if (wave_count % 10 == 0 && wave_count != 0 && biggus == 0)
		{
			// 30% of wave duration left(cloes to end of wave)
			if (wave_timer <= (wave_duration / 10 * 3))
			{

				// spawn miniboss
				CreateEnemy(30.f,
				(CP_Vector){ 2300.0f,1150.0f },
				(CP_Vector){200.f,200.f},
				30.f, 5);	

				biggus = 1;
			}
		}
		if (spawndelay <= 0.0)	// delay between each enemy spawn
		{
			if(wave_count >= 5)
			{
				int rand_Type = CP_Random_RangeInt(0, 10);
				// chance of tougher enemies increase with every wave
				if(rand_Type <= 3 + (wave_count * 0.025f))
				{
					CreateEnemy(3.f,
						(CP_Vector){ 2300.0f,1150.0f },
						(CP_Vector){200.f,200.f},
						60.f, 4);	//spawn type 3 enemy(strawberryJam guy)
				}
				else
				{
					CreateEnemy(1.f,
						(CP_Vector){ 2300.0f,1150.0f },
						(CP_Vector){200.f,200.f},
						100.f, 3);	//spawn type 3 enemy(toothpaste guy)

				}
			}
			else
			{
				CreateEnemy(1.f,
						(CP_Vector){ 2300.0f,1150.0f },
						(CP_Vector){200.f,200.f},
						100.f, 3);	//spawn type 3 enemy(toothpaste guy)
			}

			spawndelay = CP_Random_RangeFloat(3.f/((float)wave_count*0.5f),7.f/ ((float)wave_count * 0.5f));
		}
		spawndelay -= dt;
	}
}

void DayNightManager(float dt)
{
	// if DAY time
	if (is_interval == 1)
	{
		interval_counter -= dt;
		if(biggus == 1)
			biggus = 0;
		// once DAY time is over
		if (interval_counter <= 0)
		{
			interval_counter = interval_delay + wave_count;
			is_interval = 0;	// Set to NIGHT time
		}

		// Display if DAY
		sprintf_s(wave_status, 127, "(DAY) time left: %.0f", interval_counter);
		/*CP_Settings_Fill((CP_Color) { 255, 255, 255, 255 });
		CP_Settings_TextSize(50.f);
		CP_Font_DrawText(wave_status, 1150, 50);*/
		GameGUI_DrawText((CP_Color) { 255, 255, 255, 255 }, wave_status, 0.98f, 0.02f, 0.03f, CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
	}
	else	// not interval, spawning enemy waves
	{
		wave_timer -= dt;

		// if NIGHT is over
		if (wave_timer <= 0)
		{
			wave_timer = wave_duration;
			is_interval = 1;	// set to DAY time
			wave_count++;		// set next WAVE
		}

		// Display if NIGHT
		sprintf_s(wave_status, 127, "(NIGHT) time left: %.0f", wave_timer);
		/*CP_Settings_Fill((CP_Color) { 255, 100, 100, 255 });
		CP_Font_DrawText(wave_status, 1150, 50);*/
		GameGUI_DrawText((CP_Color) { 255, 100, 100, 255 }, wave_status, 0.98f, 0.02f, 0.03f, CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);

		if (wave_count % 10 == 0 && wave_count != 0)
		{
			sprintf_s(wave_status, 127, "WARNING!");
			/*CP_Settings_Fill((CP_Color) { 255, 100, 100, 255 });
			CP_Font_DrawText(wave_status, 1150, 150);*/
			GameGUI_DrawText((CP_Color) { 255, 100, 100, 255 }, wave_status, 0.98f, 0.02f, 0.03f, CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
		}
	}

	// Time (jus an ordinary counter)
	timer += dt;
	sprintf_s(curr_Timer, 127, "Time: %.0f", timer);
	CP_Settings_Fill((CP_Color) { 255, 255, 255, 255 });
	CP_Font_DrawText(curr_Timer, wind_Width/15, wind_Height/5.5f);

	sprintf_s(money_display, 127, "Souls: %d", souls_money);
	CP_Settings_Fill((CP_Color) { 175, 205, 255, 255 });
	CP_Font_DrawText(money_display, wind_Width / 15, wind_Height / 4);

	// Simple wave count (nothing much)
	sprintf_s(wave_display, 127, "WAVE %d", wave_count);
	/*CP_Settings_Fill((CP_Color) { 255, 255, 255, 255 });
	CP_Font_DrawText(wave_display, 1150, 100);*/
	GameGUI_DrawText((CP_Color) { 255, 255, 255, 255 }, wave_display, 0.98f, 0.07f, 0.03f, CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_TOP);
}

void TestBed_SpawnBean(const CP_Vector position)
{

}

void TestBed_SpawnZombie()
{
	if (!tb_zombies_init) {
		tb_zombie_resource = Sprite_AddSprite(tb_zombie_spawn_position, 200.0f, 200.0f, "./Sprites/slime1.png", 2, 3, 6, 5, 0);
		tb_zombies_init = 1;
		if (tb_zombies_size < TESTBED_ZOMBIES_MAX) {
			tb_zombies[tb_zombies_size++] = (TB_Zombie){ tb_zombie_resource, TESTBED_ZOMBIES_HEALTH, 1, 0.0f, 0 };
		}
	}
	else {
		if (tb_zombies_size < TESTBED_ZOMBIES_MAX) {
			tb_zombies[tb_zombies_size++] = (TB_Zombie){
				Sprite_AddSpriteRepeatAuto(tb_zombie_spawn_position, 200.0f, 200.0f, tb_zombie_resource), TESTBED_ZOMBIES_HEALTH, 1, 0.0f, 0 };
		}
	}
}

void TestBed_UpdateZombies(const float dt)
{
	for (int i = 0; i < sizeof(enemy_list)-1; i++) {
		if (CheckEnemyAlive(i) == 0) {
			continue;
		}
		//CP_Vector position = Sprite_GetPosition(enemy_list[i].ene_id);
		/*if (tb_zombies[i]._moving) {
			position.x -= TESTBED_ZOMBIES_SPEED * dt;
		}*/
		//Sprite_SetPosition(tb_zombies[i]._id, position);
		/*if (position.x - house_position.x < 120.0f) {
			tb_zombies[i]._moving = 0;
			if (tb_zombies[i]._attack_timer <= 0.0f) {
				House_health -= 1;
				tb_zombies[i]._attack_timer = TESTBED_ZOMBIES_ATTACK_RATE;
			}
			else {
				tb_zombies[i]._attack_timer -= dt;
			}
		}*/

		

		// check if within range of house, if within range attack
		if (CheckEnemyCollision(house_position.x + 130, house_position.y + 130,
			house_position.x - 130, house_position.y - 130, i) == 1)
		{
			//SetEnemySpeed(i, 0.f);
			SetEnemyHP(i, 0.f);

			House_health -= GetEnemyDMG(i);
				
		}
	}
}

//void TestBed_SpawnBomb(const CP_Vector position)
//{
//	CP_Vector player_position = /*CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), */Player_GetPosition(0);
//	// calculate direction
//	CP_Vector direction = CP_Vector_Normalize(CP_Vector_Subtract(position, player_position));
//	direction = CP_Vector_Scale(direction, TESTBED_BOMB_SPEED);
//	if (!tb_bombs_init) {
//		tb_bombs_resource = Sprite_AddSprite(player_position, 50.0f, 50.0f, "proto_bomb.png", 1, 1, 1, 1, 0);
//		tb_bombs_init = 1;
//		if (tb_bombs_size < TESTBED_ZOMBIES_MAX) {
//			tb_bombs[tb_bombs_size++] = (TB_Bomb){ tb_bombs_resource, 0, direction };
//		}
//	}
//	else {
//		if (tb_bombs_size < TESTBED_ZOMBIES_MAX) {
//			tb_bombs[tb_bombs_size++] = (TB_Bomb){
//				Sprite_AddSpriteRepeatAuto(player_position, 50.0f, 50.0f, tb_bombs_resource), 0, direction };
//		}
//	}
//}

//void TestBed_UpdateBombs(const float dt)
//{
//	// update bomb velocity
//	for (int i = 0; i < tb_bombs_size; i++) {
//		if (tb_bombs[i]._dead) { continue; }
//		tb_bombs[i]._velocity.y += TESTBED_BOMB_GRAVITY * dt;
//		// apply velocity
//		CP_Vector new_pos = CP_Vector_Add(Sprite_GetPosition(tb_bombs[i]._id), tb_bombs[i]._velocity);
//		Sprite_SetPosition(tb_bombs[i]._id, new_pos);
//	}
//}

void TestBed_CheckBombOnZomb()
{
	for (int i = 0; i < Player_GetProjectilesSize(); ++i) {
		if (!Player_ProjectileDead(i)) {
			//int tt_sprite = Player_GetProjectileID(i);
			//printf("Projectile: %d, pos: %f, %f\n", i, Sprite_GetPosition(tt_sprite).x, Sprite_GetPosition(tt_sprite).y);
			for (int j = 0; j < ENEMY_MAX_ENEMIES; ++j) {
				if (enemy_list[j]._initialized && enemy_list[j].isAlive) {
					CP_Vector projectile_position = Sprite_GetPosition(Player_GetProjectileID(i));
					if (CheckEnemyCollision(projectile_position.x + 25.0f, projectile_position.y + 25.0f,
						projectile_position.x - 25.0f, projectile_position.y - 25.0f, j)) {
						if (GetEnemyHP(j) <= 1)
							souls_money += GetEnemyMoney(j);
						EnemyTakeDamage(j, 1);
						Player_SetProjectileDead(i, 1);
						LightStage_DeactivateLight(Player_GetProjectileLight(i));
						LightStage_AddLight(projectile_position, 300.0f, 1600.0f, 200.0f, 0, 100);
						Particle_EmitOut(PT_Star, projectile_position, 50.0f, 100.0f, -30.0f, -30.0f, 150.0f, -150.0f, 0.8f, 0.3f, -50.0f, -80.0f, 0.04f, 0.02f, 120.0f, 10, 5, 0);
					}
				}
			}
		}
	}
}

void TestBed_CheckPlayerOnZomb(const float dt)
{
	if (!TestBed_player_invuln) {
		CP_Vector pos = Player_GetPosition(0);
		for (int j = 0; j < ENEMY_MAX_ENEMIES; ++j) {
			if (enemy_list[j]._initialized && enemy_list[j].isAlive) {
				if (CheckEnemyCollision(pos.x + 30.0f, pos.y + 30.0f,
					pos.x - 30.0f, pos.y - 30.0f, j)) {
					Player_Lose_Health(1);
					TestBed_player_invuln = 1;
				}
			}
		}
	}
	else {
		if (TestBed_player_invuln_timer_counter > 0.0f) {
			TestBed_player_invuln_timer_counter -= dt;
		}
		else {
			TestBed_player_invuln = 0;
			TestBed_player_invuln_timer_counter = TestBed_player_invuln_timer;
		}
	}
}
