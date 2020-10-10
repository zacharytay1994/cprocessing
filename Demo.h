#pragma once

#define MAX_ENEMIES 100
#define NUMBERS_SIZE 50

#include <cprocessing.h>

extern float ground_level;
extern int screen_width;
extern int screen_height;

extern CP_Image player;
extern CP_Image ground;
extern CP_Image dirt;
extern CP_Image background;
extern CP_Image house;
extern CP_Image demo_enemy;
extern CP_Image heart;
extern CP_Image number1;

extern float block_width;
extern float block_height;

extern float background_x;
extern float background_y;
extern float background_parallax;

extern CP_Vector player_position;
extern float player_width;
extern float player_height;
extern float player_ground;
extern float player_speed;
extern int player_flip;
extern int player_health;
extern int player_invulnerable;
extern float player_invulnerable_count;
extern float player_jump;
extern int player_jumping;
extern float player_jump_strength;

extern CP_Vector house_position;
extern float house_width;
extern float house_height;
extern int house_health;
extern int house_invulnerable;
extern float house_invulnerable_count;

extern float enemy_width;
extern float enemy_height;
extern float enemy_speed;
extern int enemy_health;
extern int enemy_damage;
typedef struct Enemy {
	CP_Vector position;
	float speed;
	int dead;
} Enemy;
extern int enemies_size;
extern Enemy* enemies;

typedef struct DamageNumber {
	CP_Vector position;
	float alpha;
	int dead;
} DamageNumber;
extern DamageNumber* damage_numbers;
extern int damage_numbers_size;
extern int damage_numbers_max_size;

extern int wave_spawning;
extern float wave_interval;
extern int wave_left;
extern float wave_interval_timer;

void Demo_Initialize();
// generates a flat ground
void Demo_GenerateWorld();
void Demo_Render();
void Demo_Update(const float dt);
void Demo_Input(const float dt);
int Demo_AABB(const CP_Vector p1, const float w1, const float h1, const CP_Vector p2, const float w2, const float h2);

void Demo_AddEnemy(const float x);
void Demo_EnemiesRender();
void Demo_EnemiesUpdate(const float dt);
void Demo_SpawnWave(const float interval, const int count);
void Demo_SpawningWave(const int left, const float dt);
void Demo_DamagePlayer(const int dmg);
void Demo_DamageHouse(const int dmg);

void Demo_AddDamageNumbers(const float x, const float y);
void Demo_DamageNumbersUpdate(const float dt);
void Demo_DamageNumberRender();
