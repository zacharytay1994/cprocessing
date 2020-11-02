#include "TestScene1.h"
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>

void TestScene1_Init()
{
	printf("Scene1 Initialized\n");
}

void TestScene1_Update(const float dt)
{
	if (CP_Input_KeyDown(KEY_J)) {
		printf("Scene1 updating\n");
	}
}

void TestScene1_Exit()
{
	printf("Scene1 exited\n");
}
