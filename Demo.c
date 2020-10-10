#include "Demo.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float ground_level = 800;
int screen_width;
int screen_height;

CP_Image player;
CP_Image ground;
CP_Image dirt;
CP_Image background;
CP_Image house;
CP_Image demo_enemy;
CP_Image heart;
CP_Image number1;

float block_width = 30.0f;
float block_height = 30.0f;

float background_x;
float background_y;
float background_parallax = 5.0f;

CP_Vector player_position;
float player_width = 30;
float player_height = 30;
float player_ground;
float player_speed = 120.0f;
int player_flip = 1;
int player_health = 10;
int player_invulnerable = 0;
float player_invulnerable_count = 0.0f;
float player_jump = 0.0f;
int player_jumping = 0;
float player_jump_strength = 3.0f;

CP_Vector house_position;
float house_width;
float house_height;
int house_health = 10;
int house_invulnerable = 0;
float house_invulnerable_count = 0.0f;

float enemy_width = 30.0f;
float enemy_height = 30.0f;
float enemy_speed = 30.0f;
int enemy_health;
int enemy_damage = 1;
int enemies_size;
Enemy* enemies;

DamageNumber* damage_numbers;
int damage_numbers_size = 0;
int damage_numbers_max_size;

int wave_spawning;
float wave_interval;
int wave_left;
float wave_interval_timer;

void Demo_Initialize()
{
	screen_width = CP_System_GetWindowWidth();
	screen_height = CP_System_GetWindowHeight();
	ground = CP_Image_Load("./ground_block.png");
	dirt = CP_Image_Load("./dirt_block.png");
	player = CP_Image_Load("./demo_player.png");
	background = CP_Image_Load("./demo_background.jpg");
	house = CP_Image_Load("./demo_house.png");
	demo_enemy = CP_Image_Load("./demo_enemy.png");
	heart = CP_Image_Load("./demo_heart.png");
	number1 = CP_Image_Load("./demo_number1.png");

	background_x = screen_width / 2.0f;
	background_y = screen_height / 2.0f;

	// set player in center of screen
	player_position.x = screen_width / 2.0f;
	player_ground = ground_level - player_height;
	player_position.y = player_ground;

	// set house variables
	house_width = 100.0f;
	house_height = 100.0f;
	house_position.x = screen_width / 2.0f;
	house_position.y = ground_level - (house_height / 2.0f) - (block_height / 2.0f); // 30.0f is block height

	// allocate enemies space
	enemies = (Enemy*)malloc(MAX_ENEMIES * sizeof(Enemy));

	// allocate damage numbers space
	damage_numbers = (DamageNumber*)malloc(NUMBERS_SIZE * sizeof(DamageNumber));
	damage_numbers_max_size = NUMBERS_SIZE;
}

void Demo_GenerateWorld()
{
}

void Demo_Render()
{
	// render background
	CP_Image_DrawAdvanced(background, background_x, background_y, (float)screen_width + 300.0f, (float)screen_height, 255, 0.0f);

	float number_blocks_x = screen_width / block_width;
	float number_blocks_y = (screen_height - ground_level) / block_height;
	// render at ground_level from right to left of the screen
	for (int x = 0; x < number_blocks_x; x++) {
		CP_Image_DrawAdvanced(ground, x * block_width, ground_level, block_width, block_height, 255, 0.0f);
		// render dirt below
		for (int y = 0; y < number_blocks_y; y++) {
			CP_Image_DrawAdvanced(dirt, x * block_width, (ground_level + 30) + y * block_height, block_width, block_height, 255, 0.0f);
		}
	}
	// render house
	CP_Image_DrawAdvanced(house, house_position.x, house_position.y, house_width, house_height, 255, 0.0f);
	// render player
	CP_Image_DrawAdvanced(player, player_position.x, player_position.y, player_flip * player_width, player_height, 255, 0.0f);
	// render enemies
	Demo_EnemiesRender();
	// render player health as hearts
	for (int i = 0; i < player_health; i++) {
		CP_Image_DrawAdvanced(heart, 100.0f + i * 30.0f, 100.0f, 30.0f, 30.0f, 255, 0.0f);
	}
	// render house health
	for (int i = 0; i < house_health; i++) {
		CP_Image_DrawAdvanced(house, screen_width - 100.0f - i * 30.0f, 100.0f, 30.0f, 30.0f, 255, 0.0f);
	}
	// render damage numbers
	Demo_DamageNumberRender();
}

void Demo_Update(const float dt)
{
	Demo_Input(dt);
	// update enemies
	Demo_EnemiesUpdate(dt);
	Demo_SpawningWave(wave_left, dt);

	// update house invulnv
	if (house_invulnerable_count > 0.0f) {
		house_invulnerable_count -= dt;
	}
	else {
		house_invulnerable = 0;
	}

	// update player invuln
	if (player_invulnerable_count > 0.0f) {
		player_invulnerable_count -= dt;
	}
	else {
		player_invulnerable = 0;
	}
	if (player_jumping) {
		if (player_jump < 3.142f) {
			player_jump += dt * player_jump_strength;
		}
		else {
			player_jumping = 0;
			player_jump = 0.0f;
		}
	}
	player_position.y = ground_level - player_height;
	player_position.y -= (float)sin((double)player_jump) * 50.0f;

	// update damage numbers
	Demo_DamageNumbersUpdate(dt);
}

void Demo_Input(const float dt)
{
	// do parallax effect to background
	// move player left if left key down, right if right key down
	if (CP_Input_KeyDown(KEY_LEFT)) {
		background_x += background_parallax * dt;
		player_position.x -= player_speed * dt;
		// flip on x
		player_flip = -1;
	}
	if (CP_Input_KeyDown(KEY_RIGHT)) {
		background_x -= background_parallax * dt;
		player_position.x += player_speed * dt;
		// flip on x
		player_flip = 1;
	}
	if (CP_Input_KeyDown(KEY_UP)) {
		if (!player_jumping) {
			player_jumping = 1;
		}
	}
}

int Demo_AABB(const CP_Vector p1, const float w1, const float h1, const CP_Vector p2, const float w2, const float h2)
{
	float half_w1 = w1 / 2.0f;
	float half_h1 = h1 / 2.0f;
	float half_w2 = w2 / 2.0f;
	float half_h2 = h2 / 2.0f;

	float r1 = p1.x - half_w1;
	float l1 = p1.x + half_w1;
	float t1 = p1.y - half_h1;
	float b1 = p1.y + half_h1;

	float r2 = p2.x - half_w2;
	float l2 = p2.x + half_w2;
	float t2 = p2.y - half_h2;
	float b2 = p2.y + half_h2;

	if (r2 > l1 || l2 < r1 || t2 > b1 || b2 < t1) {
		return 0;
	}
	return 1;
}

void Demo_AddEnemy(const float x)
{
	Enemy enemy = { (CP_Vector){x,ground_level - (enemy_width / 2.0f)},enemy_speed,0 };
	if (enemies_size < MAX_ENEMIES) {
		enemies[enemies_size] = enemy;
		enemies_size++;
	}
}

void Demo_EnemiesRender()
{
	for (int i = 0; i < enemies_size; i++) {
		if (enemies[i].position.x > screen_width / 2.0f) {
			CP_Image_DrawAdvanced(demo_enemy, enemies[i].position.x, enemies[i].position.y, -enemy_width, enemy_height, 255, 0.0f);
		}
		else {
			CP_Image_DrawAdvanced(demo_enemy, enemies[i].position.x, enemies[i].position.y, enemy_width, enemy_height, 255, 0.0f);
		}
	}
}

void Demo_EnemiesUpdate(const float dt)
{
	for (int i = 0; i < enemies_size; i++) {
		float x = 0.0f;
		// decide if enemy needs to go left or right
		if (enemies[i].position.x > screen_width / 2.0f) {
			x = -1.0f;
		}
		else {
			x = 1.0f;
		}
		// if touching the shelter stop moving
		if (Demo_AABB(enemies[i].position, enemy_width, enemy_height, house_position, house_width, house_height)) {
			enemies[i].speed = 0.0f;
			if (!house_invulnerable && house_health > 0) {
				Demo_DamageHouse(enemy_damage);
			}
		}
		enemies[i].position.x += enemies[i].speed * dt * x;
	}
	for (int i = 0; i < enemies_size; i++) {
		// check if enemy intersects player
		if (Demo_AABB(enemies[i].position, enemy_width, enemy_height, player_position, player_width, player_height)) {
			// damage player
			if (!player_invulnerable && player_health > 0) {
				Demo_DamagePlayer(enemy_damage);
			}
		}
	}
}

void Demo_SpawnWave(const float interval, const int count)
{
	wave_interval = interval;
	wave_spawning = 1;
	wave_interval_timer = 0.0f;
	wave_left = count;
}

void Demo_SpawningWave(const int left, const float dt)
{
	if (!wave_spawning) {
		return;
	}
	// if no more enemies to spawn this wave
	if (left <= 0) {
		wave_spawning = 0;
		return;
	}
	// else spawn enemiee
	// spawn enemies at intervals
	if (wave_interval_timer < wave_interval) {
		wave_interval_timer += dt;
		return;
	}
	else { // random to decide spawning on which side
		if (CP_Random_RangeInt(0, 1) == 0) {
			Demo_AddEnemy(0.0f);
		}
		else {
			Demo_AddEnemy((float)screen_width);
		}
		wave_left--;
		wave_interval_timer = 0.0f;
	}
}

void Demo_DamagePlayer(const int dmg)
{
	int damage = dmg > player_health ? dmg - player_health : dmg;
	player_health = player_health > 0 ? player_health - damage : 0;
	player_invulnerable = 1;
	player_invulnerable_count = 1.0f;
	Demo_AddDamageNumbers(player_position.x, player_position.y);
}

void Demo_DamageHouse(const int dmg)
{
	int damage = dmg > house_health ? dmg - house_health : dmg;
	house_health = house_health > 0 ? house_health - damage : 0;
	house_invulnerable = 1;
	house_invulnerable_count = 1.0f;
	Demo_AddDamageNumbers(house_position.x, house_position.y);
}

void Demo_AddDamageNumbers(const float x, const float y)
{
	if (damage_numbers_size < damage_numbers_max_size - 1) {
		DamageNumber number = { (CP_Vector) { x, y },1.0f,0 };
		damage_numbers[damage_numbers_size] = number;
		damage_numbers_size++;
	}
	else {
		DamageNumber* check = (DamageNumber*)realloc(damage_numbers, sizeof(DamageNumber) * damage_numbers_max_size * 2);
		if (check != NULL) {
			damage_numbers = check;
		}
	}
}

void Demo_DamageNumbersUpdate(const float dt)
{
	for (int i = 0; i < damage_numbers_size; i++) {
		damage_numbers[i].position.y -= 30.0f * dt;
		if (damage_numbers[i].alpha > 0.0f) {
			damage_numbers[i].alpha -= dt;
		}
		else {
			damage_numbers[i].alpha = 0.0f;
		}
	}
}

void Demo_DamageNumberRender()
{
	for (int i = 0; i < damage_numbers_size; i++) {
		CP_Image_DrawAdvanced(number1, damage_numbers[i].position.x, damage_numbers[i].position.y, 30.0f, 30.0f, (int)(damage_numbers[i].alpha * 255.0f), 0.0f);
	}
}
