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

#include <stdio.h>

#define TESTBED_ZOMBIES_MAX 100
#define TESTBED_ZOMBIES_HEALTH 5
#define TESTBED_ZOMBIES_SPEED 25.0f
#define TESTBED_ZOMBIES_ATTACK_RATE 2.0f
#define TESTBED_BOMB_SPEED 20.0f
#define TESTBED_BOMB_GRAVITY 9.81f

float cam_x = 0.0f;
float cam_y = 0.0f;

int tilemap;

// temp
CP_Image TestBed_house;
CP_Vector house_position;
int House_health;
int House_max_health = 10;
float House_heart_offset_x = 50.0f;
float House_heart_offset_y = 100.0f;
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


void TestBed_Init()
{
	printf("Switched to testbed.\n");

	// temp
	TestBed_house = CP_Image_Load("./Sprites/house.png");
	house_position = (CP_Vector){ 1000.0f,1065.0f };
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


	// Setting up tilemaps -
	Tilemap_Initialize();
	tilemap = Tilemap_TxtLoad256("prototype_map.txt");
	//tilemap = Tilemap_AddTilemap(64, 64, 40, 20, 0, 0);
	Tilemap_GeneratePhyObjs(tilemap);
	Player_Initialize();
	PB_Initialize();
	Inventory_Init();
	Inventory_Item_Create("poop");
	Inventory_Add_Item_Name("poop");
	Inventory_Item_Set_Image("poop", "demo_test.png");

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
	Particle_Initialize();

	Enemy_Initialize(); // Initialize enemy sprites and values- (RAY)

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


}

void TestBed_Update(const float dt)
{
	//// UPDATES
	PB_Update(dt);
	Player_Update(dt);
	GUI_Update(dt);
	Inventory_Update();
	Particle_Update(dt);
	// Enemy Functions - (RAY)
	/*DayNightManager(dt);
	UpdateEnemy(dt);*/

	if(is_interval == 0) // if not day time, spawn enemies
	{
		if (wave_count % 10 == 0 && wave_count != 0)
		{
		//	printf("is 10th wave\n");
		}
		if (spawndelay <= 0.0)	// gap between each enemy spawn
		{
			CreateEnemy(10.f,
				(CP_Vector){ 2300.0f,1150.0f },
				(CP_Vector){200.f,200.f},
				100.f, 3);	//spawn type 3 enemy(toothpaste guy)

			spawndelay = CP_Random_RangeFloat(3.f/((float)wave_count*0.5f),7.f/ ((float)wave_count * 0.5f));
		}
		spawndelay -= dt;
	}

	//// RENDERS
	Tilemap_Render(tilemap, Camera_GetCameraTransform());
	//Tilemap_Debug_Render(tilemap, Camera_GetCameraTransform());
	Player_Render();

	/*Camera_SetCameraX(cam_x);
	Camera_SetCameraY(cam_y);*/

	// temp
	// render house in the middle
	CP_Vector TestBed_house_position = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), house_position);
	CP_Image_Draw(TestBed_house, TestBed_house_position.x, TestBed_house_position.y, 300.0f, 300.0f, 255);
	for (int j = 0; j < House_health; j++) {
		CP_Image_Draw(TestBed_house, House_heart_offset_x + j * House_heart_spacing, House_heart_offset_y, 45.0f, 45.0f, 255);
	}

	if (CP_Input_KeyReleased(KEY_P)) {
		//TestBed_SpawnZombie();
		//Sprite_AddSprite(tb_zombie_spawn_position, 200.0f, 200.0f, "./Sprites/slime1.png", 2, 3, 6, 5, 0);
	}
	TestBed_UpdateZombies(dt);
	//TestBed_UpdateBombs(dt);
	//TestBed_CheckBombOnZomb();
	/*if (CP_Input_MouseClicked()) {
		CP_Vector position = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		TestBed_SpawnBomb(position);
	}*/
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
		Particle_EmitOut(PT_Dust, world_coords, 30.0f, 20.0f, 30.0f, -30.0f, 50.0f, -50.0f, 0.5f, 0.2f, -50.0f, -80.0f, 0.2f, 0.1f, 50.0f, 5, 5);
	}
	if (CP_Input_KeyReleased(KEY_K)) {
		//Particle_Reset(particle_hold);
	}
}

void TestBed_Exit()
{
	printf("Exited TestBed.");
	Tilemap_Free();
	PB_Exit();
}

void DayNightManager(float dt)
{
	// if DAY time
	if (is_interval == 1)
	{
		interval_counter -= dt;

		// once DAY time is over
		if (interval_counter <= 0)
		{
			interval_counter = interval_delay + wave_count;
			is_interval = 0;	// Set to NIGHT time
		}

		// Display if DAY
		sprintf_s(wave_status, 127, "(DAY) time left: %.0f", interval_counter);
		CP_Settings_Fill((CP_Color) { 255, 255, 255, 255 });
		CP_Settings_TextSize(50.f);
		CP_Font_DrawText(wave_status, 1150, 50);
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
		CP_Settings_Fill((CP_Color) { 255, 100, 100, 255 });
		CP_Font_DrawText(wave_status, 1150, 50);

		/*if (wave_count % 10 == 0 && wave_count != 0)
		{
			sprintf_s(wave_status, 127, "WARNING!");
			CP_Settings_Fill((CP_Color) { 255, 100, 100, 255 });
			CP_Font_DrawText(wave_status, 1150, 150);
		}*/
	}

	// Time (jus an ordinary counter)
	timer += dt;
	sprintf_s(curr_Timer, 127, "Time: %.0f", timer);
	CP_Settings_Fill((CP_Color) { 255, 255, 255, 255 });
	CP_Font_DrawText(curr_Timer, 20, 170);

	// Simple wave count (nothing much)
	sprintf_s(wave_display, 127, "WAVE %d", wave_count);
	CP_Settings_Fill((CP_Color) { 255, 255, 255, 255 });
	CP_Font_DrawText(wave_display, 1150, 100);
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
			House_health -= 1;
			//SetEnemySpeed(i, 0.f);
			SetEnemyHP(i, 0.f);
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
	//CP_Vector zombs_pos;
	//CP_Vector bombs_pos;
	//float zomb_half_x = 80.0f;
	//float zomb_half_y = 80.0f;
	//float bomb_half_x = 25.0f;
	//float bomb_half_y = 25.0f;
	//for (int i = 0; i < tb_bombs_size; i++) {
	//	if (!tb_bombs[i]._dead) {
	//		for (int j = 0; j < tb_zombies_size; j++) {
	//			if (!tb_zombies[j]._dead && !tb_bombs[i]._dead) {
	//				zombs_pos = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), Sprite_GetPosition(tb_zombies[j]._id));
	//				bombs_pos = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), Sprite_GetPosition(tb_bombs[i]._id));
	//				// do aabb
	//				if (!(zombs_pos.x - zomb_half_x > bombs_pos.x + bomb_half_x ||
	//					zombs_pos.x + zomb_half_x < bombs_pos.x - bomb_half_x ||
	//					zombs_pos.y - zomb_half_y > bombs_pos.y + bomb_half_y ||
	//					zombs_pos.y + zomb_half_y < bombs_pos.y - bomb_half_y)) {
	//					// hit, minus zombs health by 1 turn bomb dead
	//					tb_zombies[j]._health -= 1;
	//					if (tb_zombies[j]._health <= 0) {
	//						tb_zombies[j]._dead = 1;
	//						Sprite_SetVisible(tb_zombies[j]._id, 0);
	//					}
	//					tb_bombs[i]._dead = 1;
	//					Sprite_SetVisible(tb_bombs[i]._id, 0);
	//				}
	//			}
	//		}
	//	}
	//}
}
