#pragma once

#include "CProcessing/inc/cprocessing.h"

typedef enum HowToPlayImageType {
	HTP_Keyboard,
	HTP_Tab,
	HTP_Up,
	HTP_Left,
	HTP_Right,
	HTP_LeftMouse,
	HTP_RightMouse,
	HTP_Escape
} HTP_Image;

void HowToPlay_Init();
void HowToPlay_Update(const float dt);
void HowToPlay_Exit();

void HowToPlay_AddImage(const int img, const float fadeIn, const float fadeOut);
void HowToPlay_SetKeyVisible(const HTP_Image img, const int b);
void HowToPlay_ImageUpdate(const float dt);