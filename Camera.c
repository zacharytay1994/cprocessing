#include "Camera.h"

CP_Vector _position;
CP_Vector _inverse_position;
float _rotation;
float _inverse_rotation;
CP_Matrix _transform;

int Camera_bound = 0;	
int Camera_limit = 0;
CP_Vector Camera_vertical_limit;
CP_Vector Camera_horizontal_limit; 
CP_Vector* Camera_bound_position;

float Camera_lerp_amount = 0.2f;

CP_Matrix Camera_GetCameraTransform()
{
	return _transform;
}

void Camera_Initialize()
{
	Camera_vertical_limit = (CP_Vector){ 0.0f, 0.0f };
	Camera_horizontal_limit = (CP_Vector){ 0.0f, 0.0f };
}

void Camera_Update(const float dt)
{
	// update camera transform
	//_transform = CP_Matrix_Multiply(CP_Matrix_Rotate(_rotation), CP_Matrix_Translate(_position));
	float half_width = (float)CP_System_GetWindowWidth() / 2.0f;
	float half_height = (float)CP_System_GetWindowHeight() / 2.0f;

	_transform = CP_Matrix_Translate(_inverse_position);
	if (Camera_bound) {
		Camera_SetCameraX(CP_Math_LerpFloat(_position.x, Camera_bound_position->x - half_width, Camera_lerp_amount));
		Camera_SetCameraY(CP_Math_LerpFloat(_position.y, Camera_bound_position->y - half_height, Camera_lerp_amount));
		if (Camera_limit) {
			if (_position.x - half_width < Camera_horizontal_limit.x) {
				Camera_SetCameraX(Camera_horizontal_limit.x + half_width);
			}
			if (_position.x + half_width > Camera_horizontal_limit.y) {
				Camera_SetCameraX(Camera_horizontal_limit.y - half_width);
			}
			if (_position.y + half_height > Camera_vertical_limit.y) {
				Camera_SetCameraY(Camera_vertical_limit.y - half_height);
			}
		}
	}
}

void Camera_SetCameraX(const float x)
{
	_position.x = x;
	_inverse_position.x = -x;
}

void Camera_SetCameraY(const float y)
{
	_position.y = y;
	_inverse_position.y = -y;
}

void Camera_SetRotation(const float deg)
{
	_rotation = deg;
	_inverse_rotation = -deg;
}

void Camera_BindToPosition(const CP_Vector* position)
{
	Camera_bound_position = position;
	Camera_bound = 1;
}

void Camera_Bind(const int bind)
{
	Camera_bound = bind;
}

void Camera_SetLerp(const float lerp)
{
	Camera_lerp_amount = lerp;
}

CP_Vector Camera_ScreenToWorld(const float x, const float y)
{
	// returns C^r*C^t*_position
	return CP_Vector_Set(x+_position.x,y+_position.y);
}

void Camera_SetLimit(const int limit)
{
	Camera_limit = limit;
}

void Camera_SetVerticalLimit(const CP_Vector limit)
{
	Camera_vertical_limit = limit;
}

void Camera_SetHorizontalLimit(const CP_Vector limit)
{
	Camera_horizontal_limit = limit;
}
