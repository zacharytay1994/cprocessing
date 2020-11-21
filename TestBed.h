#pragma once
#include "CProcessing/inc/cprocessing.h"

typedef struct TB_Zombie {
	int _id;
	int _health;
	int _moving;
	float _attack_timer;
	int _dead;
} TB_Zombie;

void TestBed_Init();
void TestBed_Update(const float dt);
void TestBed_Exit();

void TestBed_SpawnZombie();
void TestBed_UpdateZombies(const float dt);
//void TestBed_SpawnBomb(const CP_Vector position);
//void TestBed_UpdateBombs(const float dt);
void TestBed_CheckBombOnZomb();