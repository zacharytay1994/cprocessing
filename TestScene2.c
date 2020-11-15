#include "TestScene2.h"
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>

CP_Image t2_background;

void TestScene2_Init()
{
	printf("Scene2 Initialized\n");
	t2_background = CP_Image_Load("demo_player.png");
}

void TestScene2_Update(const float dt)
{
	if (CP_Input_KeyDown(KEY_J)) {
		printf("Scene2 updating\n");
	}
	CP_Image_Draw(t2_background, 100, 100, 100, 100, 255);
}

void TestScene2_Exit()
{
	printf("Scene2 exited\n");
}
