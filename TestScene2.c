#include "TestScene2.h"
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>

void TestScene2_Init()
{
	printf("Scene2 Initialized\n");
}

void TestScene2_Update(const float dt)
{
	if (CP_Input_KeyDown(KEY_J)) {
		printf("Scene2 updating\n");
	}
}

void TestScene2_Exit()
{
	printf("Scene2 exited\n");
}
