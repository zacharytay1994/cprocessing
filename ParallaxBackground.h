//---------------------------------------------------------
// file:	ParallaxBackground.h
// author:	Jia Min
// email:	j.jiamin@digipen.edu
//
// brief:	Renders parallax background with for the scene
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright  2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include "CProcessing/inc/cprocessing.h"

void PB_Initialize();
void PB_Update(const float dt);
void PB_Exit();