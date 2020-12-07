//---------------------------------------------------------
// file:	Particles.h
// author:	Zachary
// email:	z.tay@digipen.edu
//
// brief:	Particle system with simple emissions
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "Sprite.h"

#include "CProcessing/inc/cprocessing.h"

#define PARTICLE_SYSTEM_MAX 50

typedef enum ParticleType {
	//PT_Nothing,
	PT_Dust,
	PT_Star,
	PT_Bean,
	PT_Droplets
} ParticleType;

typedef enum ParticleSystemType {
	PS_EmitOut,
	PS_EmitIn
} ParticleSystemType;

typedef struct Particles {
	int			_id;
	CP_Vector	_emit_position;
} Particles;

typedef struct ParticleData {
	CP_Vector	_position;
	CP_Vector	_dimensions;
	CP_Vector	_velocity;
	float		_angular_velocity;
	float		_lifetime;
	float		_alpha_change;
	float		_scale_change;
	int			_fps;
	int			_gravity;
} ParticleData;

typedef struct Particle {
	int				_id;
	int				_dead;
	int				_sprite;
	ParticleType	_type;
	ParticleData	_data;
} Particle;

void Particle_Initialize();
void Particle_Update(const float dt);

int	Particle_InitParticle(const ParticleType type, const ParticleData data, const float alpha);
void Particle_AddParticleType(const char* path, const int col, const int row, const int frames, const int fps);
void Particle_Reset(const int id);

void Particle_EmitOut(const ParticleType type, const CP_Vector position,
	const float upperSize, const float lowerSize,
	const float upperRot, const float lowerRot,
	const float upperSpeed, const float lowerSpeed,
	const float upperLifetime, const float lowerLifetime,
	const float upperAlphaChange, const float lowerAlphaChange, 
	const float upperScaleChange, const float lowerScaleChange,
	const float alpha, const int fps, const int number,
	const int gravity);