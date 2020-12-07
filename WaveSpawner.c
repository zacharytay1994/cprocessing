//---------------------------------------------------------
// file:	WaveSpawner.c
// author:	Ray
//
// email:	L.khengray@digipen.edu
//
// brief:	Handles wave spawning in the game
//
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "WaveSpawner.h"
#include "Enemy.h"



void Generate_Waves(WaveSpawner* spawner, int Num_waves, int types, float dt)
{
	// Define interval duration
	spawner->interval_duration = 10;
	// Init curr interval waittime
	spawner->curr_interval = spawner->interval_duration;
	// Define total waves in game
	spawner->total_waves = Num_waves;
	// Init Wave timer
	spawner->wave[spawner->curr_wave].curr_WaveTimer = spawner->wave[spawner->curr_wave].wave_duration;

}


void Update_Waves(WaveSpawner* spawner, float dt)
{
	// Wave running, 
	spawner->wave[spawner->curr_wave].curr_WaveTimer -= dt;

	// If wave time up
	if (spawner->wave[spawner->curr_wave].curr_WaveTimer <= 0)
	{
		// Interval running
		spawner->curr_interval -= dt;
		// if interval time up 
		if (spawner->curr_interval <= 0)
		{
			// reset interval time
			spawner->curr_interval = spawner->interval_duration;
			// go next wave if not end of waves
			if (spawner->curr_wave < spawner->total_waves)
			{
				// increment curr wave
				spawner->curr_wave++;
				// reset wave timer to tne next wave timer duration
				spawner->wave[spawner->curr_wave].curr_WaveTimer = spawner->wave[spawner->curr_wave].wave_duration;
			}
		}
	}

	
}

