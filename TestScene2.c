#include "TestScene2.h"
#include "CProcessing/inc/cprocessing.h"
#include <stdio.h>

#include "Sprite.h"


int Plant_id [100];
int plant_id = 0 ;

int bean_mr, smokey_mr;

int temp;
int bool_check = 1;


void TestScene2_Init()
{
	bean_mr = Sprite_AddSpriteF((CP_Vector){0.0f,0.0f}, 300.0f, 300.0f, "./Photos/Plants-01.png", 4, 2, 7, 0.2f, 1);
	smokey_mr = Sprite_AddSpriteF((CP_Vector) {0.0f, 0.0f}, 300.0f, 300.0f, "./Photos/Smoke-04.png", 1, 3, 3, 0.2f, 1);
}

void TestScene2_Update(const float dt)
{
	float MouseX = CP_Input_GetMouseX();
	float MouseY = CP_Input_GetMouseY();

	CP_Vector cp_vector = CP_Vector_Set(CP_Input_GetMouseX(), (CP_Input_GetMouseY()-70.0f));

	if (CP_Input_MouseClicked())
	{
		bool_check = 0;
		if (plant_id > 0) 
		{
			for (int i = 0; i < plant_id; ++i)
			{
				CP_Vector plant_pos = Sprite_GetPosition(Plant_id[i]);
				float left = plant_pos.x - 100.0f;
				float right = plant_pos.x + 100.0f;
				float up = plant_pos.y - 100.0f;
				float down = plant_pos.y + 100.0f;
				printf("%f %f %f %f \n", left, right, up, down);
				printf("%f %f \n", MouseX, MouseY);
				if (!(MouseX < left || MouseX > right || MouseY < up || MouseY > down))
				{
					bool_check = 1;
				}
			}
		}
		else
		{
			temp = Sprite_AddSpriteRepeatAuto(cp_vector, 300.0f, 300.0f, bean_mr);
			Plant_id[plant_id] = temp;
			plant_id += 1;
			Sprite_OptOut(temp, 0);
			Sprite_SetRepeat(temp, 0);
		}

		if (bool_check == 0)
		{
			temp = Sprite_AddSpriteRepeatAuto(cp_vector, 300.0f, 300.0f, bean_mr);
			Plant_id[plant_id] = temp;
			plant_id += 1;
			Sprite_OptOut(temp, 0);
			Sprite_SetRepeat(temp, 0);
		}
















		/*if (plant_id == 0)
		{
			bool_check = 1;
		}
		else
		{
			for (int check_id1 = 0; check_id1 != 0; check_id1++)
			{
				CP_Vector plant_pos = Sprite_GetPosition(Plant_id[check_id1]);
				if (((plant_pos.x - 100.0f) < MouseX < (plant_pos.x + 100.0f)) && ((plant_pos.y - 100.0f) < MouseY < (plant_pos.y + 100.0f)))
				{
				}
				else
				{
					bool_check = 1;
				}
			}
		}
		
		if (bool_check == 1)
		{
			temp = Sprite_AddSpriteRepeatAuto(cp_vector, 300.0f, 300.0f, bean_mr);
			Plant_id[plant_id] = temp;
			plant_id += 1;
			Sprite_OptOut(temp, 0);
			Sprite_SetRepeat(temp, 0);
		}*/

	}
}

void TestScene2_Exit()
{
	printf("Scene2 exited\n");
}




				//cp_vector = CP_Vector_Set(plant_pos.x, plant_pos.y);
				//Sprite_AddSpriteRepeatAuto(cp_vector, 300.0f, 300.f, smokey_mr);
				//Sprite_SetRepeat(smokey_mr, 0);
				//CP_Image Plant_Img_Potion = CP_Image_Load("./Photos/Potions-01.png");
				//CP_Image_Draw(Plant_Img_Potion, MouseX, MouseY, plant_pos.x, plant_pos.y, 255);