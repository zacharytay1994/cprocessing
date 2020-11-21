#pragma once
#ifndef WAVE_SPAWNER
#define WAVE_SPAWNER

typedef struct WaveDetail
{
	double			wave_duration;
	const double	spawndelay;
	double			curr_spawnTimer;
}WaveDetail;

typedef struct WaveSpawner
{
	WaveDetail wave[3];
	int curr_wave;

}WaveSpawner;



void Generate_Waves(WaveSpawner* spawner, int Num_waves, int types, float dt);

void Update_Waves(WaveDetail* wave, float dt);


#endif