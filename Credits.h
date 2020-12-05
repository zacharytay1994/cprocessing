/*!
@file       Credits.h
@author     Noel Ho Sing Nam (s.ho)
@course     CSD1400
@section    A
@brief      Contains the data and functions for rendering the credits
*//*__________________________________________________________________________
_*/
#pragma once
#include <cprocessing.h>
#include "Scene.h"

#define BLACK		CP_Color_Create(0,   0,   0,   255)
#define WHITE		CP_Color_Create(255, 255, 255, 255)
#define GREEN_TINT	CP_Color_Create(230, 255, 230, 255)

char copyright[255];
char title1[127];
char subtitle1[127];
char line1[127];
char line2[127];
char line3[127];
char line4[127];
char subtitle2[127];
char line5[127];
char line6[127];
char subtitle3[127];
char line7[127];
int Credits_WindowWidth;
int Credits_WindowHeight;
float time_buffer;
CP_Font custom_font;

void Credits_Init();
void Credits_Update(const float dt);
void Credits_Render();
void Credits_Exit();