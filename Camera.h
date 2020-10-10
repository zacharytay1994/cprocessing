#pragma once

#include "CProcessing/inc/cprocessing.h"

// position is the negative position, i.e. offset
extern CP_Vector _position;
extern float _rotation;
extern CP_Matrix _transform;

CP_Matrix Camera_GetCameraTransform();
void Camera_Update(const float dt);
void Camera_SetCameraX(const float x);
void Camera_SetCameraY(const float y);
void Camera_SetRotation(const float deg);
CP_Vector Camera_ScreenToWorld(const float x, const float y);