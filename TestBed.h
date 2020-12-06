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
void DayNightManager(float dt);