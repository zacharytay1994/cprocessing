/*!
@file       Credits.h
@author     Noel Ho Sing Nam (s.ho) / Jia Min
@course     CSD1400
@section    A
@brief      Contains the data and functions for rendering the credits
*//*__________________________________________________________________________
_*/
#pragma once
#include <cprocessing.h>
#include "Scene.h"
#include "Button.h"
#include "MainMenu.h"

#define BLACK			CP_Color_Create(0,   0,   0,   255)
#define WHITE			CP_Color_Create(255, 255, 255, 255)
#define POO_BROWN		CP_Color_Create( 59,  35,  20, 255)
#define POO_LIGHT_BROWN	CP_Color_Create(195, 153, 107, 255)

CP_Vector Credit_Logo_Shadow;
CP_Image credit_logo;
CP_Image credit_logo_shadow;
CP_Image credit_background;
CP_Image credit_box;
CP_Image credit_button;

float Time_Buffer;
float Credits_WindowWidth_div16;
float Credits_WindowHeight_div16;
float Credit_Button_Height;
float Credit_Button_Width;
float GetMouseX;
float GetMouseY;


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
int Scene_Number;
CP_Font custom_font;

void Credits_Init();
void Credits_Update(const float dt);
void Credits_Render();
void Credits_Exit();