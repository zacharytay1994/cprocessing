#include "LightStage.h"
#include "Camera.h"
#include <stdio.h>

#define LIGHTSTAGE_MAX_LIGHTS 50

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

CP_Image LightStage_light;
CP_Image LightStage_overlay;

CP_Vector LightStage_screen_dimensions;
CP_Vector LightStage_screen_center;

int LightStage_ambient = 0;
LightStage_Light LightStage_lights[LIGHTSTAGE_MAX_LIGHTS] = { 0 };

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
	//// render stuff as add
	//CP_Settings_BlendMode(CP_BLEND_ADD);
	//CP_Image_Draw(LightStage_light, CP_Input_GetMouseX(), CP_Input_GetMouseY(), 100.0f, 100.0f, 100);
	//CP_Settings_BlendMode(CP_BLEND_ALPHA);
}

void LightStage_Exit() {

}

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
			//if (LightStage_lights[i]._alpha > 0.1f) {
			//	//LightStage_lights[i]._alpha = CP_Math_LerpFloat(0.0f, LightStage_lights[i]._alpha, LightStage_lights[i]._fade * dt);
			//	if (LightStage_lights[i]._fade > 0.0f) {
			//		LightStage_lights[i]._alpha -= LightStage_lights[i]._fade * dt;
			//	}
			//}
			//else {
			//	LightStage_lights[i]._active = 0;
			//}
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
