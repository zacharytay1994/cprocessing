#include "Enemy.h"




// Enemy Stuff - TODO (Move to Enemy class)
void CreateEnemy(int hp, CP_Vector position, CP_Vector size, float speed, int enemy_type)
{
	struct Enemy new_enemy;

	// Enemy Type Dependant variables
	char* path;
	int enem_sprite_col;
	int enem_sprite_row;
	int enem_sprite_frames;
	int enem_sprite_animate_speed;


	// Basic Enemy Variables
	new_enemy.health = hp;
	new_enemy.position = position;
	new_enemy.speed = speed;
	new_enemy.enem_Size = size;

	switch (enemy_type)
	{
	case 0:	// VitaminC
	{
		path = ("demo_enemy.png");
		enem_sprite_col = 1;
		enem_sprite_row = 1;
		enem_sprite_frames = 1;
		enem_sprite_animate_speed = 1;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };
		//enemy_sprite = CP_Image_Load(path);
		break;
	}
	case 1:	// NoOxygen
	{
		path = ("demo_enemy2.png");
		new_enemy.enem_Size.x = size.x * 2.5f;
		new_enemy.enem_Size.y = size.y * 2.5f;
		enem_sprite_col = 1;
		enem_sprite_row = 1;
		enem_sprite_frames = 1;
		enem_sprite_animate_speed = 1;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };

		break;
	}
	case 2:	// Late4Class
	{
		path = ("demo_player.png");
		new_enemy.enem_Size.x = size.x * 1.2f;
		new_enemy.enem_Size.y = size.y * 1.2f;
		enem_sprite_col = 8;
		enem_sprite_row = 1;
		enem_sprite_frames = 8;
		enem_sprite_animate_speed = 50;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };

		break;
	}
	default:	// ???
	{
		path = ("demo_enemy.png");
		enem_sprite_col = 1;
		enem_sprite_row = 1;
		enem_sprite_frames = 1;
		enem_sprite_animate_speed = 1;
		new_enemy.enem_HitboxScale = (CP_Vector){ 1,1 };

		break;
	}
	}

	// Assign Enemy Sprite
	new_enemy.ene_sprite_id = Sprite_AddSprite(
		new_enemy.position,
		-(new_enemy.enem_Size.x),
		new_enemy.enem_Size.y,
		path,
		enem_sprite_col,
		enem_sprite_row,
		enem_sprite_frames,
		enem_sprite_animate_speed,0);

	new_enemy.isAlive = 1;	//1 - alive, 0 - dead
	printf("NEWeneScaleX: %2f, NEWeneScaleY: %2f\n", (new_enemy.enem_HitboxScale.x), (new_enemy.enem_HitboxScale.y));
	// Add enemy to list
	new_enemy.ene_id = Add_Enem_toList(&new_enemy);
	//Debug
	printf("Enemy created at x: %f, y: %f\n", new_enemy.position.x, new_enemy.position.y);
}

int Add_Enem_toList(struct Enemy* add_enem)
{
	for (int i = 0; i < 127; i++)
	{
		if (enemy_list[i].isAlive == 0)
		{
			enemy_list[i] = *add_enem;
			enemy_list[i].ene_id = i;
			// Debug
			printf("Enemy added\n");
			return i;
		}
	}
	return 0;
}

void UpdateEnemy(const float dt)
{
	for (int i = 0; i < 127; ++i)
	{
		if (enemy_list[i].isAlive == 0)
		{
			// if Enemy is "dead", set sprite visible off
			Sprite_SetVisible(enemy_list[i].ene_sprite_id, 1);
			continue;
		}
		else
		{
			//Update Movement/sprite movement 
			enemy_list[i].position.x -= dt * (enemy_list[i].speed);
			Sprite_SetPosition(enemy_list[i].ene_sprite_id, enemy_list[i].position);

			//Only render alive enemies
			Sprite_RenderSprite(dt, enemy_list[i].ene_sprite_id);

			//if out of map (left boundaries only, not like enemy gonna move right...right?)
			if (CheckEnemyCollision(0.f, wind_Height, -10.f, 0.f, i) == 1)
			{
				enemy_list[i].isAlive = 0;
			}
			
		}

	}
}

int CheckEnemyCollision(float maxPos_X, float maxPos_Y, float minPos_X, float minPos_Y, int enemy_id)
{
	// Get enemy max hitbox boundery
	float maxhitBox_posX = enemy_list[enemy_id].position.x + (enemy_list[enemy_id].enem_HitboxScale.x / 2.f);
	float minhitBox_posX = enemy_list[enemy_id].position.x - (enemy_list[enemy_id].enem_HitboxScale.x / 2.f);
	// Get enemy min hitbox boundary
	float maxhitBox_posY = enemy_list[enemy_id].position.y + (enemy_list[enemy_id].enem_HitboxScale.y / 2.f);
	float minhitBox_posY = enemy_list[enemy_id].position.y - (enemy_list[enemy_id].enem_HitboxScale.y / 2.f);

	// Compares objbox with enemy hitbox
	if (maxPos_X >= minhitBox_posX &&
		maxPos_Y >= minhitBox_posY)
	{
		return 1;
	}
	else if (minPos_X <= maxhitBox_posX &&
		minPos_Y >= maxhitBox_posY)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CheckEnemyAlive(int id)
{
	if (enemy_list[id].isAlive == 1)
		return 1;
	else
		return 0;
}

void SetEnemyDie(int id)
{
	enemy_list[id].isAlive = 0;
}
// End of Enemy Stuff