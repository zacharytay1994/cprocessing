#pragma once

#include "CProcessing/inc/cprocessing.h"

// position is the negative position, i.e. offset
extern CP_Vector _position;
extern float _rotation;
extern CP_Matrix _transform;

CP_Matrix Camera_GetCameraTransform();
void Camera_Initialize();
void Camera_Update(const float dt);
void Camera_SetCameraX(const float x);
void Camera_SetCameraY(const float y);
void Camera_SetRotation(const float deg);
void Camera_BindToPosition(const CP_Vector* position);
void Camera_Bind(const int bind);
void Camera_SetLerp(const float lerp);
CP_Vector Camera_ScreenToWorld(const float x, const float y);
void Camera_SetLimit(const int limit);
void Camera_SetVerticalLimit(const CP_Vector limit);
void Camera_SetHorizontalLimit(const CP_Vector limit);

void Camera_Shake(const float amount);
void Camera_Shake_Update(const float dt);