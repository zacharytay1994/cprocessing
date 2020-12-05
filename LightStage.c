#include "LightStage.h"
#include "Camera.h"
#include <math.h>
#include <stdio.h>

#define LIGHTSTAGE_MAX_LIGHTS 50
#define LIGHTSTAGE_LIGHT_RADIUS 1500

/*____________________________________________________________________________________________________________________________________*/
// VEC3 definition, just a 3 component vector
/*____________________________________________________________________________________________________________________________________*/
typedef struct Vec3 {
	float x, y, z;
} Vec3;

float	Vec3_Dot(Vec3 lhs, Vec3 rhs) { return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z); }
float	Vec3_Mag(Vec3 vec) { return (float)sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z)); }
Vec3	Vec3_Normalize(Vec3 vec) {
	float mag = Vec3_Mag(vec);
	return (Vec3) { vec.x / mag, vec.y / mag, vec.z / mag };
}
Vec3	Vec3_Sub(Vec3 lhs, Vec3 rhs) { return (Vec3) { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }

/*____________________________________________________________________________________________________________________________________*/
// Definition of light "Object".
/*____________________________________________________________________________________________________________________________________*/
struct LightStage_Light {
	CP_Vector	_position;
	float		_size;
	float		_fadeUp;
	float		_fadeDown;
	int			_active;
	float		_initialAlpha;
	float		_midAlpha;
	int			_faded_up;
};

/*____________________________________________________________________________________________________________________________________*/
// Various variables (vv)
/*____________________________________________________________________________________________________________________________________*/
		CP_Image	LightStage_light;										// the cp_image resource to use as the light
		CP_Image	LightStage_overlay;										// the cp_image resource to use as the blended ambient light
	   CP_Vector	LightStage_screen_dimensions;							// cached screen dimension
	   CP_Vector	LightStage_screen_center;								// cached screen center
			 int	LightStage_ambient = 0;									// ambient light
LightStage_Light	LightStage_lights[LIGHTSTAGE_MAX_LIGHTS] = { 0 };		// container to hold all the lights
	   CP_Vector	LightStage_positions[LIGHTSTAGE_MAX_LIGHTS] = { 0 };	// container to hold light positions
			 int	LightStage_positions_size = 0;							// size for the above container ^

void LightStage_Initialize() {
	LightStage_light = CP_Image_Load("./Sprites/test_light.png");
	LightStage_overlay = CP_Image_Load("./Sprites/light_overlay.png");
	LightStage_screen_dimensions = (CP_Vector){ (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight() };
	LightStage_screen_center = (CP_Vector){ LightStage_screen_dimensions.x/2.0f, LightStage_screen_dimensions.y/2.0f };
}

void LightStage_Update(const float dt) {
	LightStage_UpdateAndRenderLights(dt);
}

void LightStage_Render() {
	// render overlay as subtract
	CP_Settings_BlendMode(CP_BLEND_SUBTRACT);
	CP_Image_Draw(LightStage_overlay, LightStage_screen_center.x, LightStage_screen_center.y, LightStage_screen_dimensions.x, LightStage_screen_dimensions.y, LightStage_ambient);
	CP_Settings_BlendMode(CP_BLEND_ALPHA);
}

void LightStage_Exit() {}

int LightStage_AddLight(const CP_Vector position, const float size, const float fadeUp, const float fadeDown, const float initialAlpha, const float midAlpha)
{
	for (int i = 0; i < LIGHTSTAGE_MAX_LIGHTS; ++i) {
		if (!LightStage_lights[i]._active) {
			LightStage_lights[i]._active = 1;
			LightStage_lights[i]._position = position;
			LightStage_lights[i]._fadeUp = fadeUp;
			LightStage_lights[i]._fadeDown = fadeDown;
			LightStage_lights[i]._size = size;
			LightStage_lights[i]._initialAlpha = initialAlpha;
			LightStage_lights[i]._midAlpha = midAlpha;
			LightStage_lights[i]._faded_up = 0;
			return i;
		}
	}
	printf("LightStage::MAX LIGHTS EXCEEDED!");
	return -1;
}

void LightStage_UpdateAndRenderLights(const float dt)
{
	// set blend mode to add
	CP_Settings_BlendMode(CP_BLEND_ADD);
	for (int i = 0; i < LIGHTSTAGE_MAX_LIGHTS; ++i) {
		if (LightStage_lights[i]._active) {
			// fade alpha by set amount
			if (!LightStage_lights[i]._faded_up) {
				if (LightStage_lights[i]._initialAlpha < LightStage_lights[i]._midAlpha) {
					if (LightStage_lights[i]._fadeUp > 0.0f) {
						LightStage_lights[i]._initialAlpha += LightStage_lights[i]._fadeUp * dt;
					}
				}
				else {
					LightStage_lights[i]._initialAlpha = LightStage_lights[i]._midAlpha;
					LightStage_lights[i]._faded_up = 1;
				}
			}
			else {
				if (LightStage_lights[i]._initialAlpha > 0.0f) {
					if (LightStage_lights[i]._fadeDown > 0.0f) {
						LightStage_lights[i]._initialAlpha -= LightStage_lights[i]._fadeDown * dt;
					}
				}
				else {
					LightStage_lights[i]._active = 0;
				}
			}
			CP_Vector position = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), LightStage_lights[i]._position);
			CP_Image_Draw(LightStage_light, position.x, position.y,
				LightStage_lights[i]._size, LightStage_lights[i]._size, (int)LightStage_lights[i]._initialAlpha);
		}
	}
	CP_Settings_BlendMode(CP_BLEND_ALPHA);
}

void LightStage_SetAmbient(const int ambient)
{
	LightStage_ambient = ambient;
}

void LightStage_SetPosition(const int id, const CP_Vector position)
{
	if (id < LIGHTSTAGE_MAX_LIGHTS) {
		LightStage_lights[id]._position = position;
	}
}

void LightStage_DeactivateLight(const int id)
{
	if (id < LIGHTSTAGE_MAX_LIGHTS) {
		LightStage_lights[id]._active = 0;
	}
}

void LightStage_ApplyNormalMap(void* data, void* normal_data, void* og_data, CP_Vector topLeftPosition, const float width, const float height, const float pixelWidth, const float pixelHeight, CP_Vector* lightPositions, int numberOfLights)
{
	// og_data (calculated with) normal_data => output altered data
	unsigned char* array = (unsigned char*)data;
	unsigned char* normal = (unsigned char*)normal_data;
	unsigned char* og_array = (unsigned char*)og_data;

	// some spicy variables
	int r, g, b;
	float intensity = 0.0f;
	float ambient = 0.2f;
	ambient = ambient < 0.3f ? 0.3f : ambient;
	float normal_scale;
	float distance_scale;
	Vec3 normal_vec3 = (Vec3){ 0.0f,0.0f,0.0f };
	Vec3 light_vec3 = (Vec3){ 0.0f,0.0f,0.0f };
	Vec3 light_pos = (Vec3){ 0.0f,0.0f,0.0f };
	Vec3 pixel_pos = (Vec3){ 0.0f,0.0f,0.0f };

	// calculating intensity based on distance and angle from light source, OoO dot product
	// ghetto way of doing, forgot the actual formula
	for (int i = 0; i < pixelWidth * pixelHeight; ++i) {
		intensity = 0.0f;
		pixel_pos.x = topLeftPosition.x + width * ((float)(i % (int)pixelWidth) / pixelWidth);
		pixel_pos.y = topLeftPosition.y + height * ((float)(i / (int)pixelHeight) / pixelHeight);
		pixel_pos.z = 0.0f;
		normal_vec3.x = ((float)(normal[i * 4] - 127) / 128.0f);
		normal_vec3.y = ((float)(normal[i * 4 + 1] - 127) / 128.0f);
		normal_vec3.z = ((float)(normal[i * 4 + 2] - 127) / 128.0f);

		for (int j = 0; j < numberOfLights; ++j) {
			light_pos = (Vec3){lightPositions[j].x, lightPositions[j].y, 300.0f};
			light_vec3 = Vec3_Sub(light_pos, pixel_pos);
			normal_scale = Vec3_Dot(Vec3_Normalize(light_vec3), Vec3_Normalize(normal_vec3));
			distance_scale = (LIGHTSTAGE_LIGHT_RADIUS - Vec3_Mag(light_vec3)) / LIGHTSTAGE_LIGHT_RADIUS;
			// mod intensity
			if (normal_scale < 0.0f) { normal_scale *= -1.0f; }
			if (distance_scale < 0.0f) { distance_scale = 0.0f; }
			intensity += distance_scale * normal_scale + ambient;
		}
		intensity = intensity > 1.0f ? 1.0f : intensity;
		intensity = intensity < 0.0f ? 0.0f : intensity;

		r = (int)((int)og_array[i * 4] * intensity);
		g = (int)((int)og_array[i * 4 + 1] * intensity);
		b = (int)((int)og_array[i * 4 + 2] * intensity);

		array[i * 4] = (unsigned char)r;
		array[i * 4 + 1] = (unsigned char)g;
		array[i * 4 + 2] = (unsigned char)b;
	}
}

CP_Vector* LightStage_GetLightPositionsArray()
{
	LightStage_positions_size = 0;
	for (int i = 0; i < LIGHTSTAGE_MAX_LIGHTS; ++i) {
		if (LightStage_lights[i]._active) {
			LightStage_positions[LightStage_positions_size++] = LightStage_lights[i]._position;
		}
	}
	return LightStage_positions;
}

int LightStage_GetLightPositionsSize()
{
	return LightStage_positions_size;
}
