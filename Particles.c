#include "Particles.h"

#include <stdio.h>

#define MAX_PARTICLE_TYPES 50
#define MAX_PARTICLES 500

int Particle_types[MAX_PARTICLE_TYPES] = { 0 };
int Particle_types_size = 0;

Particle Particle_particles[MAX_PARTICLES] = { 0 };

void Particle_Initialize()
{
	// initializing types
	Particle_AddParticleType("./Sprites/part_smoke.png", 2, 3, 6, 3);
	Particle_AddParticleType("./Sprites/effect1.png", 2, 3, 6, 3);
	Particle_AddParticleType("./Photos/Smoke-02.png", 4, 2, 8, 1);

	// initialize particle pool
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		Particle_particles[i] = (Particle){ i,1,
			Sprite_AddSpriteRepeatAuto((CP_Vector){0.0f,0.0f},1,1,Particle_types[0]),0,
			(ParticleData) {(CP_Vector){0.0f,0.0f},(CP_Vector){0.0f,0.0f},(CP_Vector){0.0f,0.0f},0.0f,0.0f,0.0f,0.0f} 
		};
		Sprite_SetVisible(Particle_particles[i]._sprite, 0);
	}
}

void Particle_Update(const float dt)
{
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		if (!Particle_particles[i]._dead) {
			// update position
			Particle_particles[i]._data._position = CP_Vector_Add(Particle_particles[i]._data._position, CP_Vector_Scale(Particle_particles[i]._data._velocity, dt));
			// set sprite data
			Sprite* sprite = Sprite_GetSprite(Particle_particles[i]._sprite);
			sprite->_position = Particle_particles[i]._data._position;
			sprite->_rotation += Particle_particles[i]._data._angular_velocity * dt;
			if (sprite->_alpha >= 0 && sprite->_alpha <= 255) {
				sprite->_alpha += Particle_particles[i]._data._alpha_change * dt;
			}
			sprite->_scale += Particle_particles[i]._data._scale_change * dt;
			if (Particle_particles[i]._data._lifetime > 0.0f) {
				Particle_particles[i]._data._lifetime -= dt;
			}
			else {
				// dies, i.e. reset particle
				Particle_particles[i]._dead = 1;
				sprite->_visible = 0;
				sprite->_alpha = 255.0f;
				sprite->_scale = 1.0f;
				Sprite_Reset(Particle_particles[i]._sprite);
			}
			//Sprite_RenderSprite(dt, Particle_particles[i]._sprite);
		}
	}
}

int Particle_InitParticle(const ParticleType type, const ParticleData data, const float alpha)
{
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		if (Particle_particles[i]._dead) {
			Particle_particles[i]._data = data;
			Sprite* sprite = Sprite_GetSprite(Particle_particles[i]._sprite);
			sprite->_image_resource = Sprite_GetImageResource( Particle_types[(int)type] );
			sprite->_position = data._position;
			sprite->_width = data._dimensions.x;
			sprite->_height = data._dimensions.y;
			sprite->_visible = 1;
			sprite->_alpha = alpha;
			Sprite_SetFPS(Particle_particles[i]._sprite, data._fps);
			Particle_particles[i]._type = type;
			Particle_particles[i]._dead = 0;
			return i;
		}
	}
	printf("NOT ENOUGH PARTICLES");
	return -1;
}

void Particle_AddParticleType(const char* path, const int col, const int row, const int frames, const int fps)
{
	if (Particle_types_size < MAX_PARTICLE_TYPES) {
		Particle_types[Particle_types_size++] = Sprite_AddSprite((CP_Vector) { -100.0f, -100.0f }, 1, 1, path, col, row, frames, fps, 0);
	}
}

void Particle_Reset(const int id)
{
	if (id < MAX_PARTICLES && id >= 0) {
		Sprite* sprite = Sprite_GetSprite(Particle_particles[id]._sprite);
		sprite->_visible = 0;
		Particle_particles[id]._dead = 1;
	}
}

void Particle_EmitOut(const ParticleType type, const CP_Vector position,
	const float upperSize, const float lowerSize,
	const float upperRot, const float lowerRot,
	const float upperSpeed, const float lowerSpeed,
	const float upperLifetime, const float lowerLifetime,
	const float upperAlphaChange, const float lowerAlphaChange,
	const float upperScaleChange, const float lowerScaleChange,
	const float alpha, const int fps, const int number)
{
	ParticleData particle_data;
	for (int i = 0; i < number; ++i) {
		particle_data._position = position;
		float size = CP_Random_RangeFloat(lowerSize, upperSize);
		particle_data._dimensions = (CP_Vector){ size,size };
		// calculate random velocity
		float random_x = CP_Random_RangeFloat(-1.0f, 1.0f);
		float random_y = CP_Random_RangeFloat(-1.0f, 1.0f);
		CP_Vector normalized_vel = CP_Vector_Normalize(CP_Vector_Set(random_x, random_y));
		particle_data._velocity = CP_Vector_Scale(normalized_vel,CP_Random_RangeFloat(lowerSpeed,upperSpeed));
		particle_data._angular_velocity = CP_Random_RangeFloat(lowerRot, upperRot);
		float random_lifetime = CP_Random_RangeFloat(lowerLifetime, upperLifetime);
		particle_data._lifetime = random_lifetime;
		float random_alpha_change = CP_Random_RangeFloat(lowerAlphaChange, upperAlphaChange);
		particle_data._alpha_change = random_alpha_change;
		float random_scale_change = CP_Random_RangeFloat(lowerScaleChange , upperScaleChange);
		particle_data._scale_change = random_scale_change;
		particle_data._fps = fps;
		Particle_InitParticle(type, particle_data, alpha);
	}
}
