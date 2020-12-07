//---------------------------------------------------------
// file:	WaveSpawner.h
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
#pragma once
#ifndef WAVE_SPAWNER
#define WAVE_SPAWNER

typedef struct WaveDetail
{
	/*Wave Planner
		Within a wave:
			x Number of enemies is not defined since wave is based on 
				endurance/survival(Timer)

			- Types of enemies within that wave.
			- Delay before spawning (for "tough" mobs that appear in 
				mid wave) : Require "what" enemy to delay
			- Interval between each spawn(subjective to each enemy)
			- Timer before end of wave
				
	*/

	double			wave_duration;
	double			curr_WaveTimer;
	
	double			interval_duration;
	double			curr_interval;

	double			ene1_counter;
	double			ene1_delay;

	double			ene2_counter;
	double			ene2_delay;

	double			ene3_counter;
	double			ene3_delay;

}WaveDetail;

typedef struct WaveSpawner
{
	WaveDetail wave[3];
	int total_waves;
	int curr_wave;

	double interval_duration;
	double curr_interval;

}WaveSpawner;

/*how many types: A, B, C
	X, Y, Z
	
	Wave(waveTime, type, interval)
	wave(waveTime, type, type, intervals, interval, delay)
	wave(waveTime, type, type, type, interval, interval, interval, delay, delay)*/
void Generate_Waves(WaveSpawner* spawner, int Num_waves, int types, float dt);

void Update_Waves(WaveSpawner* wave, float dt);


#endif