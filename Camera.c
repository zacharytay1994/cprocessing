#include "Camera.h"

CP_Vector _position;
CP_Vector _inverse_position;
float _rotation;
float _inverse_rotation;
CP_Matrix _transform;

CP_Matrix Camera_GetCameraTransform()
{
	return _transform;
}

void Camera_Update(const float dt)
{
	// update camera transform
	//_transform = CP_Matrix_Multiply(CP_Matrix_Rotate(_rotation), CP_Matrix_Translate(_position));
	_transform = CP_Matrix_Translate(_inverse_position);
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
	_inverse_rotation = -deg;
}

CP_Vector Camera_ScreenToWorld(const float x, const float y)
{
	// returns C^r*C^t*_position
	return CP_Vector_Set(x+_position.x,y+_position.y);
}
