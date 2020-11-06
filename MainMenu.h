#pragma once

#include <cprocessing.h>

#include "GameState.h"
#include "Button.h"

struct Button button_start;
struct Button button_credits;
struct Button button_exit;

void MainMenu_Initialize();

void MainMenu_Update(const float dt);

void MainMenu_Render();

void MainMenu_Exit();