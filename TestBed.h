#pragma once
#include "CProcessing/inc/cprocessing.h"

typedef struct TB_Zombie {
	int _id;
	int _health;
	int _moving;
	float _attack_timer;
	int _dead;
} TB_Zombie;

CP_Sound BGM;

void TestBed_Init();
void TestBed_Update(const float dt);
void TestBed_Exit();

void TestBed_SpawnZombie();
void TestBed_UpdateZombies(const float dt);
//void TestBed_SpawnBomb(const CP_Vector position);
//void TestBed_UpdateBombs(const float dt);
void TestBed_CheckBombOnZomb();
void TestBed_CheckPlayerOnZomb(const float dt);

// Enemy/Wave functions(RAY)
void TestBed_SpawnBean(const CP_Vector position);
void WaveUpdate(float dt);
void DayNightManager(float dt);

void TestBed_RandomRainCrates(const float dt);
void TestBed_DrawLineToPlayer(const float dt);
void TestBed_AddMissile(const CP_Vector position, const int type);
void TestBed_UpdateRenderMissiles(const float dt);