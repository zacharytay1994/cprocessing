#include "TestScene3.h"
#include <stdio.h>

CP_Image test_logo;

void TestScene3_Init()
{
	test_logo = CP_Image_Load("demo_enemy.png");
	printf("Scene2 Initialized\n");
}

void TestScene3_Update(const float dt)
{
	if (CP_Input_KeyDown(KEY_J)) {
		Scene_ChangeScene(2);
	}
}

void TestScene3_Exit()
{
	printf("Scene2 exited\n");
}