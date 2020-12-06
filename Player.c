#include "Player.h"
#include "Camera.h"
#include "Inventory.h"
#include "Particles.h"
#include "LightStage.h"
#include "GameGUI.h"

#include <stdio.h>
#include <math.h>

#define PLAYER_DUST_TIMER 0.2f
#define PLAYER_GROUND_LEVEL 1200

PlayerData Player_players[PLAYER_MAX_PLAYERS] = { 0 };
int Player_players_size = 0;

int Player_initialized = 0;

// temp
int Player_health;
int Player_max_health = 10;
CP_Image Player_heart;
CP_Image Player_blackheart;
float Player_heart_offset_x = 50.0f;
float Player_heart_offset_y = 50.0f;
float Player_heart_spacing = 60.0f;

// Weapon code
int Player_weapon;
CP_Vector Player_weapon_position;
float Player_weapon_width = 100.0f;
float Player_weapon_height = 100.0f;
CP_Vector Player_weapon_offset;
int Player_weapon_out = 0;
float Player_max_weapon_strength = 30.0f;
float Player_weapon_strength = 0.0f;
int Player_weapon_down = 0;

Player_Projectile Player_projectiles[PLAYER_MAX_PROJECTILES];
int Player_projectiles_size = 0;
int Player_projectiles_init = 0;
int Player_projectiles_resource;
CP_Image Player_projectile_arc_resource;
int Player_projectile_fire = 0;

float Player_dust_timer = PLAYER_DUST_TIMER;

int Player_weapon_charge = -1;
CP_Image Player_weapon_charge_bar;
CP_Image Player_weapon_charge_bar2;
int Player_weapon_max_charge = 16;
float Player_weapon_charge_gap = 10.0f;
float Player_weapon_charge_height = 20.0f;
float Player_weapon_charge_width = 5.0f;
float Player_weapon_angle = 0.0f;
float Player_weapon_charge_interval = 0.06f;
float Player_weapon_charge_timer = 0.0f;
int	  Player_weapon_charge_min_bar = 6;
float Player_weapon_charge_bar_increment = 0.1f;

void Player_Initialize()
{
	Player_projectile_arc_resource = CP_Image_Load("./Sprites/ground.png");

	Player_health = Player_max_health;
	Player_heart = CP_Image_Load("demo_heart.png");
	Player_blackheart = CP_Image_Load("demo_blackheart.png");
	Player_weapon_charge_bar = CP_Image_Load("./Sprites/charge.png");
	Player_weapon_charge_bar2 = CP_Image_Load("./Sprites/charge2.png");

	// initialize powerup
	speed_multiplier = 1.0f;
	for (int i = 0; i < 99; i++)
	{
		powerup_timer[i] = 0.0f;
		powerup_type[i] = 0;
	}

	// initialize weapon
	Player_weapon_offset = (CP_Vector){ 0.0f,-100.0f };
	Player_weapon_position = (CP_Vector){ 0.0f,0.0f };
	Player_weapon = Sprite_AddSprite(Player_weapon_position, Player_weapon_width, Player_weapon_height, "./Sprites/weapon.png", 2, 3, 6, 20, 0);
	Sprite_SetRepeat(Player_weapon, 0);
	Sprite_SetVisible(Player_weapon, 0);

	// Adding player 2 into the scene
	/*Player_AddPlayer(position, 10.0f, 22.5f, 55.0f, 0.0f, 60.0f, 60.0f,
		"player_idle.png", 3, 4, 10, 10,
		"player_run.png", 3, 3, 8, 15,
		"player_jump.png", 3, 4, 10, 30,
		KEY_W,KEY_S,KEY_A,KEY_D);*/
	// initialize player at the center of the screen
	CP_Vector position = CP_Vector_Set(200.0f, -800.0f);

	// Adding a player into the scene
	Player_AddPlayer(position,
		PLAYER1_MASS, PLAYER1_BOUNDING_WIDTH, PLAYER1_BOUNDING_HEIGHT, PLAYER1_FRICTION, PLAYER1_SPRITE_WIDTH, PLAYER1_SPRITE_HEIGHT,
		"./Sprites/p_idle.png", 2, 3, 6, 8,
		"./Sprites/p_run.png", 2, 3, 6, 12,
		"./Sprites/p_jump.png", 2, 3, 6, 25,
		KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
	Player_SetCameraFocus(0);
	Player_projectiles_init = 1;
	Player_projectiles_resource = Sprite_AddSprite(CP_Vector_Set(-1000,-1000), 50.0f, 50.0f, "./Sprites/projectile1.png", 2, 3, 6, 20, 1);
	Player_initialized = 1;
}

void Player_AddPlayer(const CP_Vector pos, const float mass, const float bwidth, const float bheight, const float friction,
	const float swidth, const float sheight, 
	const char* idle, const int icol, const int irow, const int iframes, const int ifps,
	const char* run, const int rcol, const int rrow, const int rframes, const int rfps, 
	const char* jump, const int jcol, const int jrow, const int jframes, const int jfps,
	const CP_KEY up, const CP_KEY down, const CP_KEY left, const CP_KEY right)
{
	if (Player_players_size < PLAYER_MAX_PLAYERS) {
			Player_players[Player_players_size] = (PlayerData) { 
			(CP_Vector) { pos.x,pos.y },
			PhyObj_AddAABox( pos.x, pos.y, mass, bwidth/2.0f, bheight/2.0f, friction )->super._id,
			Sprite_AddSprite(pos, swidth, sheight, idle, icol, irow, iframes, ifps, 0),
			Sprite_AddSprite(pos, swidth, sheight, run, rcol, rrow, rframes, rfps, 0),
			Sprite_AddSprite(pos, swidth, sheight, jump, jcol, jrow, jframes, jfps, 0),
			0,
			0,
			0.0f,
			Player_Anim_Idle,
			0,
			up,
			down,
			left,
			right
		};
		Sprite_SetVisible(Player_players[Player_players_size]._sprite_run, 0);
		Sprite_SetVisible(Player_players[Player_players_size]._sprite_jump, 0);
		Sprite_SetRepeat(Player_players[Player_players_size]._sprite_jump, 0);
		++Player_players_size;
	}
}

void Player_Update(const float dt)
{
	if (CP_Input_KeyDown(KEY_J)) {
		Player_Lose_Health(1);
	}
	if (!Player_initialized && CP_Input_KeyReleased(KEY_I)) {
		// initialize player at the center of the screen
		CP_Vector position = CP_Vector_Set(200.0f, -800.0f);

		// Adding a player into the scene
		Player_AddPlayer(position,
			PLAYER1_MASS, PLAYER1_BOUNDING_WIDTH, PLAYER1_BOUNDING_HEIGHT, PLAYER1_FRICTION, PLAYER1_SPRITE_WIDTH, PLAYER1_SPRITE_HEIGHT,
			"player_idle.png", 3, 4, 10, 10,
			"player_run.png", 3, 3, 8, 15,
			"player_jump.png", 3, 4, 10, 30,
			KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
		Player_SetCameraFocus(0);
		Player_initialized = 1;
	}
	for (int i = 0; i < Player_players_size; ++i) {
		Player_players[i]._grounded = 0;
		Player_SyncPositionWithBoundingShape();
		PhyObjBoundingShape* shape = PhyObj_GetShape(Player_players[i]._box);
		Player_players[i]._grounded = shape->_grounded;
		// flag airborne is grounded more then set time
		if (!Player_players[i]._grounded) {
			if ((Player_players[i]._airborne_counter += dt) > 0.1f) {
				Player_players[i]._airborne = 1;
			}
		}
		else {
			Player_players[i]._airborne_counter = 0.0f;
			Player_players[i]._airborne = 0;
		}
	}
	Player_Input(dt);

	// temp render player ui
	/*int j = 0;
	for (j = 0; j < Player_health; j++) {
		CP_Image_Draw(Player_heart, Player_heart_offset_x + j * Player_heart_spacing, Player_heart_offset_y, 60.0f, 60.0f, 255);
	}
	for (j; j < Player_max_health; j++)
	{
		CP_Image_Draw(Player_blackheart, Player_heart_offset_x + j * Player_heart_spacing, Player_heart_offset_y, 60.0f, 60.0f, 255);
	}*/
	Player_WeaponUpdate(dt);
}

void Player_Render()
{
	// nothing to render yet as sprite.c handles rendering of sprites,
	// even player sprites
	Player_RenderProjectiles();
}

void Player_Input(const float dt)
{
	for (int i = 0; i < Player_players_size; ++i) {
		PhyObjBoundingShape* shape = PhyObj_GetShape(Player_players[i]._box);
		if (CP_Input_KeyDown(KEY_O)) {
			shape->_position = CP_Vector_Set(200.0f, 20.0f);
		}
		int some_input = 0;
		// detect key press
		if (Player_players[i]._airborne) {
			Player_SwitchAnimationState(i,Player_Anim_Jump);
			some_input = 1;
		}
		if (CP_Input_KeyDown(Player_players[i]._right) || CP_Input_KeyDown(KEY_D)) {
			// check if horizontal velocity has not exceeded velocity limit
			if (shape->_velocity.x < PLAYER_MAX_HVELOCITY) {
				PhyObj_AddVelocity(shape, CP_Vector_Set(PLAYER_APPLIED_HVELOCITY * dt * speed_multiplier, 0.0f));
			}
			// if not in air, switch animation
			if (Player_players[i]._grounded) {
				Player_SwitchAnimationState(i,Player_Anim_Run);
				some_input = !some_input;
			}
			Player_FlipAnimationsH(i,0);
			if (Player_dust_timer > 0.0f) {
				Player_dust_timer -= dt;
			}
			else {
				CP_Vector temp_pos = CP_Vector_Add(Player_players[i]._position, (CP_Vector) { 0.0f, 30.0f });
				Particle_EmitOut(PT_Dust, temp_pos, 30.0f, 30.0f, 50.0f, -30.0f, 50.0f, -50.0f, 0.5f, 0.2f, -50.0f, -80.0f, 0.2f, 0.1f, 50.0f, 5, 5, 0);
				Player_dust_timer = PLAYER_DUST_TIMER;
			}
		}
		if (CP_Input_KeyDown(Player_players[i]._left) || CP_Input_KeyDown(KEY_A)) {
			if (shape->_velocity.x > -PLAYER_MAX_HVELOCITY) {
				PhyObj_AddVelocity(shape, CP_Vector_Set(-PLAYER_APPLIED_HVELOCITY * dt * speed_multiplier, 0.0f));
			}
			if (Player_players[i]._grounded) {
				Player_SwitchAnimationState(i,Player_Anim_Run);
				some_input = !some_input;
			}
			Player_FlipAnimationsH(i,1);
			if (Player_dust_timer > 0.0f) {
				Player_dust_timer -= dt;
			}
			else {
				CP_Vector temp_pos = CP_Vector_Add(Player_players[i]._position, (CP_Vector) { 0.0f, 30.0f });
				Particle_EmitOut(PT_Dust, temp_pos, 30.0f, 30.0f, 50.0f, -30.0f, 50.0f, -50.0f, 0.5f, 0.2f, -50.0f, -80.0f, 0.2f, 0.1f, 50.0f, 5, 5, 0);
				Player_dust_timer = PLAYER_DUST_TIMER;
			}
		}
		if (CP_Input_KeyDown(Player_players[i]._up) || CP_Input_KeyDown(KEY_W)) {
			if (Player_players[i]._grounded && shape->_velocity.y > -PLAYER_MAX_VVELOCITY) {
				PhyObj_ApplyImpulse(shape, CP_Vector_Set(0.0f, -PLAYER_APPLIED_VVELOCITY));
			}
		}
		if (!some_input) {
			Player_SwitchAnimationState(i,Player_Anim_Idle);
			Player_Deccelerate(dt);
		}
		if (CP_Input_KeyDown(KEY_L)) {
			PhyObj_SetVisible(Player_players[i]._box, 1);
		}
		else {
			PhyObj_SetVisible(Player_players[i]._box, 0);
		}

		if (CP_Input_KeyDown(KEY_TAB))
		{
			Inventory_Open();
		}
		else
		{
			Inventory_Close();
		}

		if (CP_Input_MouseDown(0)) {
			if (!Player_weapon_down) {
				Player_weapon_down = 1;
				Player_weapon_charge = 0;
			}
			Player_ShowWeapon();
		}
		else {
			if (Player_weapon_down) {
				Player_weapon_charge = -1;
				Player_weapon_down = 0;
			}
			Player_HideWeapon(dt);
		}
	}
}

void Player_SyncPositionWithBoundingShape()
{
	for (int i = 0; i < Player_players_size; ++i) {
		Player_players[i]._position = PhyObj_GetShape(Player_players[i]._box)->_position;
		// sync sprite position with bounding box
		Sprite_SetPosition(Player_players[i]._sprite_idle, Player_players[i]._position);
		Sprite_SetPosition(Player_players[i]._sprite_run, Player_players[i]._position);
		Sprite_SetPosition(Player_players[i]._sprite_jump, Player_players[i]._position);
	}
}

void Player_SwitchAnimationState(const int id, PlayerAnimationState state)
{
	if (id < Player_players_size) {
		if (Player_players[id]._current_animation != state) {
			switch (Player_players[id]._current_animation) {
			case Player_Anim_Idle:
				Sprite_SetVisible(Player_players[id]._sprite_idle, 0);
				Sprite_Reset(Player_players[id]._sprite_idle);
				break;
			case Player_Anim_Run:
				Sprite_SetVisible(Player_players[id]._sprite_run, 0);
				Sprite_Reset(Player_players[id]._sprite_run);
				break;
			case Player_Anim_Jump:
				Sprite_SetVisible(Player_players[id]._sprite_jump, 0);
				Sprite_Reset(Player_players[id]._sprite_jump);
				break;
			}
			switch (state) {
			case Player_Anim_Idle:
				Sprite_SetVisible(Player_players[id]._sprite_idle, 1);
				break;
			case Player_Anim_Run:
				Sprite_SetVisible(Player_players[id]._sprite_run, 1);
				break;
			case Player_Anim_Jump:
				Sprite_SetVisible(Player_players[id]._sprite_jump, 1);
				break;
			}
			Player_players[id]._current_animation = state;
		}
	}
}

void Player_FlipAnimationsH(const int id, const int flip)
{
	if (id < Player_players_size) {
		if (Player_players[id]._animation_flipped != flip) {
			Player_players[id]._animation_flipped = flip;
			Sprite_SetFlip(Player_players[id]._sprite_idle, flip);
			Sprite_SetFlip(Player_players[id]._sprite_run, flip);
			Sprite_SetFlip(Player_players[id]._sprite_jump, flip);
		}
	}
}

void Player_Deccelerate(const float dt)
{
	for (int i = 0; i < Player_players_size; ++i) {
		PhyObjBoundingShape* shape = PhyObj_GetShape(Player_players[i]._box);
		// deccelerate player horizontally if above a certain threshold
		if (shape->_velocity.x > PLAYER_DECCELERATION_THRES) {
			PhyObj_AddVelocity(shape, CP_Vector_Set(-PLAYER_DECCELERATION * dt, 0.0f));
		}
		else if (shape->_velocity.x < -PLAYER_DECCELERATION_THRES) {
			PhyObj_AddVelocity(shape, CP_Vector_Set(PLAYER_DECCELERATION * dt, 0.0f));
		}
		else {
			shape->_velocity.x = 0.0f;
		}
	}
}

CP_Vector Player_GetPosition(const int id)
{
	if (id < Player_players_size) {
		return Player_players[id]._position;
	}
	return CP_Vector_Set(0.0f, 0.0f);
}

CP_Vector* Player_GetPosition_P(const int id)
{
	if (id < Player_players_size) {
		return &(Player_players[id]._position);
	}
	return NULL;
}

void Player_SetCameraFocus(const int id)
{
	if (id < PLAYER_MAX_PLAYERS) {
		Camera_BindToPosition(Player_GetPosition_P(id)); // magic number 0 here means player 1, index 0 in players array
	}
}

void Player_temp()
{
	PhyObjBoundingShape* shape = PhyObj_GetShape(Player_players[0]._box);
	shape->_position = CP_Vector_Set(200.0f, 20.0f);
}

int Player_GetProjectilesSize()
{
	return Player_projectiles_size;
}

int Player_GetProjectileID(const int id)
{
	if (id < Player_projectiles_size) {
		return Player_projectiles[id]._id;
	}
	return -1;
}

int Player_ProjectileDead(const int id)
{
	if (id < Player_projectiles_size) {
		return Player_projectiles[id]._dead;
	}
	return -1;
}

int Player_GetProjectileLight(const int id)
{
	if (id < Player_projectiles_size) {
		return Player_projectiles[id]._light_id;
	}
	return -1;
}

void Player_SetProjectileDead(const int id, const int b)
{
	if (id < Player_projectiles_size) {
		Player_projectiles[id]._dead = b;
	}
}

void Player_RenderProjectiles()
{
	for (int i = 0; i < Player_projectiles_size; ++i) {
		if (!Player_projectiles[i]._dead) {
			Sprite_RenderSprite(CP_System_GetDt(), Player_projectiles[i]._id);
		}
	}
}

void Player_ShowWeapon()
{
	if (Player_initialized) {
		Sprite_SetAlpha(Player_weapon, 255.0f);
		if (!Player_weapon_out) {
			Sprite_Reset(Player_weapon);
			Sprite_SetVisible(Player_weapon, 1);
			Player_weapon_out = 1;
			//Player_weapon_charge = 0;
		}
		//Particle_EmitOut(PT_Star, Player_weapon_position, 50.0f, 100.0f, -30.0f, -30.0f, 150.0f, -150.0f, 0.8f, 0.3f, -50.0f, -80.0f, 0.04f, 0.02f, 120.0f, 10, 5);
	}
}

void Player_HideWeapon(const float dt)
{
	if (Player_initialized && Player_weapon_out) {
		float alpha = Sprite_GetAlpha(Player_weapon);
		if (alpha > 0.0f) {
			Sprite_SetAlpha(Player_weapon, alpha - 1000.0f * dt);
		}
		else {
			Sprite_SetVisible(Player_weapon, 0);
			Player_weapon_out = 0;
		}
	}
}

void Player_WeaponUpdate(const float dt)
{
	if (Player_initialized) {
		CP_Vector weapon_position = CP_Vector_Add(Player_players[0]._position, Player_weapon_offset);
		Player_weapon_position = weapon_position;
		Sprite_SetPosition(Player_weapon, weapon_position);

		// rotate weapon to mouse
		CP_Vector mouse_position = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		CP_Vector mouse_vector = CP_Vector_Subtract(mouse_position, weapon_position);
		float angle = CP_Vector_Angle((CP_Vector) { 1.0f, 0.0f }, mouse_vector);
		if (mouse_vector.y < 0.0f) {
			angle *= -1;
		}
		if (mouse_vector.x < 0.0f) {
			Sprite_SetFlip(Player_weapon, 1);
			angle -= 180;
		}
		else {
			Sprite_SetFlip(Player_weapon, 0);
		}
		Player_weapon_angle = angle;
		Sprite_SetRotation(Player_weapon, angle);
		Player_ProjectileUpdate(dt);
	}
}

void Player_SpawnProjectile(const float dt)
{
	CP_Vector position = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
	CP_Vector start_position = Player_weapon_position;
	// calculate direction
	CP_Vector direction = CP_Vector_Normalize(CP_Vector_Subtract(position, start_position));
	direction = CP_Vector_Scale(direction, Player_weapon_strength);
	CP_Vector spawn_position = CP_Vector_Add(start_position, CP_Vector_Scale(direction, 5.0f));
	if (Player_projectile_fire) {
		if (!Player_projectiles_init) {
			Player_projectiles_resource = Sprite_AddSprite(spawn_position, 50.0f, 50.0f, "./Sprites/projectile1.png", 2, 3, 6, 20, 1);
			Particle_EmitOut(PT_Star, spawn_position, 50.0f, 100.0f, -30.0f, -30.0f, 150.0f, -150.0f, 0.8f, 0.3f, -50.0f, -80.0f, 0.04f, 0.02f, 120.0f, 10, 5, 0);
			LightStage_AddLight(spawn_position, 500.0f, 1000.0f, 600.0f, 0, 100);
			Player_projectiles_init = 1;
			if (Player_projectiles_size < PLAYER_MAX_PROJECTILES) {
				Player_projectiles[Player_projectiles_size++] = (Player_Projectile){ Player_projectiles_resource, 0, direction,
				LightStage_AddLight(spawn_position, 100.0f, 1600.0f, -1.0f, 0, 100) };
			}
		}
		else {
			if (Player_projectiles_size < PLAYER_MAX_PROJECTILES) {
				LightStage_AddLight(spawn_position, 500.0f, 1000.0f, 600.0f, 0, 100);
				Player_projectiles[Player_projectiles_size++] = (Player_Projectile){
					Sprite_AddSpriteRepeatAuto(spawn_position, 50.0f, 50.0f, Player_projectiles_resource), 0, direction,
					LightStage_AddLight(spawn_position, 100.0f, 600.0f, -1.0f, 0, 150) };
				Particle_EmitOut(PT_Star, spawn_position, 50.0f, 100.0f, -30.0f, -30.0f, 150.0f, -150.0f, 0.8f, 0.3f, -50.0f, -80.0f, 0.04f, 0.02f, 120.0f, 10, 5, 0);
			}
		}
		Camera_Shake(5.0f);
		Player_projectile_fire = 0;
	}
	//Player_RenderProjectileArc(spawn_position, direction, 10, 9.81f, dt);
	if (Player_weapon_charge >= 0) {
		Player_RenderProjectileCharge(CP_Vector_Add(start_position,CP_Vector_Scale(CP_Vector_Normalize(direction),30.0f)), direction, dt);
	}
}

void Player_ProjectileUpdate(const float dt)
{
	// check for spawning
	if (CP_Input_MouseClicked()) {
		Player_projectile_fire = 1;
	}
	//CP_Vector position = Camera_ScreenToWorld(CP_Input_GetMouseX(), CP_Input_GetMouseY());
	Player_SpawnProjectile(dt);
	 // update bomb velocity
	for (int i = 0; i < Player_projectiles_size; i++) {
		if (Player_projectiles[i]._dead) { continue; }
		Player_projectiles[i]._velocity.y += 9.81f * dt;
		// apply velocity
		CP_Vector pos = Sprite_GetPosition(Player_projectiles[i]._id);
		CP_Vector new_pos = CP_Vector_Add(pos, Player_projectiles[i]._velocity);
		Sprite_SetPosition(Player_projectiles[i]._id, new_pos);
		LightStage_SetPosition(Player_projectiles[i]._light_id, new_pos);
		// check if below ground lvl destroy self and emit particles
		if (new_pos.y > PLAYER_GROUND_LEVEL) {
			Player_projectiles[i]._dead = 1;
			LightStage_DeactivateLight(Player_projectiles[i]._light_id);
			LightStage_AddLight(new_pos, 300.0f, 200.0f, 200.0f, 0, 100);
			Particle_EmitOut(PT_Star, pos, 50.0f, 100.0f, -30.0f, -30.0f, 150.0f, -150.0f, 0.8f, 0.3f, -50.0f, -80.0f, 0.04f, 0.02f, 120.0f, 10, 5, 0);
		}
	}
}

void Player_RenderProjectileArc(const CP_Vector position, const CP_Vector direction, const int lineDensity, const float fallOff, const float dt)
{
	if (Player_initialized) {
		float arc_dt = dt;
		arc_dt = 0.02f;
		float frames = 20.0f;
		CP_Vector velocity = CP_Vector_Set(0.0f, 0.0f);
		CP_Vector pos = position;
		for (int i = 0; i < 10; ++i) {
			CP_Vector store = velocity;
			velocity = CP_Vector_Add(velocity, CP_Vector_Scale(direction, frames * arc_dt * i));
			CP_Graphics_DrawLine(pos.x + store.x, pos.y + store.y, pos.x + store.x + velocity.x, pos.y + store.y + velocity.y);
			velocity.y += fallOff * i * frames * arc_dt;
			pos = CP_Vector_Add(pos, velocity);
			pos = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), pos);
			CP_Image_Draw(Player_projectile_arc_resource, pos.x, pos.y, 30.0f, 30.0f, 255);
		}
	}
}

void Player_RenderProjectileCharge(const CP_Vector position, const CP_Vector direction, const float dt)
{
	if (Player_initialized) {
		if (Player_weapon_charge_timer > 0.0f) {
			Player_weapon_charge_timer -= dt;
		}
		else if (Player_weapon_charge < Player_weapon_max_charge) {
			Player_weapon_charge++;
			Player_weapon_charge_timer = Player_weapon_charge_interval;
			Player_weapon_strength = ((float)Player_weapon_charge/(float)Player_weapon_max_charge) * Player_max_weapon_strength;
		}

		CP_Vector offset = CP_Vector_Set(0.0f, 0.0f);
		CP_Vector pos;
		for (int i = 0; i < Player_weapon_charge; ++i) {
			offset = CP_Vector_Set(0.0f, 0.0f);
			offset = CP_Vector_Add(offset, CP_Vector_Scale(CP_Vector_Normalize(direction), (i+1) * Player_weapon_charge_gap));
			pos = CP_Vector_Add(position, offset);
			pos = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), pos);
			// if above certain limit use charge bar2
			if (i > Player_weapon_charge_min_bar-1) {
				CP_Image_DrawAdvanced(Player_weapon_charge_bar2, pos.x, pos.y, Player_weapon_charge_width, (1 + Player_weapon_charge_bar_increment * i) * Player_weapon_charge_height, 255, Player_weapon_angle);
			}
			else {
				CP_Image_DrawAdvanced(Player_weapon_charge_bar, pos.x, pos.y, Player_weapon_charge_width, (1 + Player_weapon_charge_bar_increment * i) * Player_weapon_charge_height, 255, Player_weapon_angle);
			}
		}
	}
}

void Player_Add_Health(int x)
{
	Player_health += x;
	if (Player_health > Player_max_health)
	{
		Player_health = Player_max_health;
	}
}

void Player_Lose_Health(int x)
{
	Player_health -= x;
	if (Player_health < 0)
	{
		Player_health = 0;
	}
	// set healthscale on gui
	GameGUI_SetHealthScale((float)Player_health / (float)Player_max_health);
	GameGUI_SetRedHitRatio(255.0f);
	// shake screen
	Camera_Shake(6.0f);
}

void Player_Add_MaxHealth(int x)
{
	Player_max_health += x;
	Player_health += x;
}

void Player_Lose_MaxHealth(int x)
{
	Player_max_health -= x;
	if (Player_max_health < 1)
	{
		Player_max_health = 1;
	}
	if (Player_health > Player_max_health)
	{
		Player_health = Player_max_health;
	}
}

void Player_Add_Powerup(int x, float duration)
{
	for (int i = 0; i < 99; i++)
	{
		if (powerup_type[i] == 0)
		{
			powerup_type[i] = x;
			powerup_timer[i] = duration;
			switch (x)
			{
				case 1:			// SPEED UP
				{
					speed_multiplier = 5.0f;
					break;
				}
			}
		}
	}
}

void Player_Powerup_Update(const float dt)
{
	for (int i = 0; i < 99; i++)
	{
		if (powerup_type[i] == 0)
		{
			continue;
		}
		else
		{
			powerup_timer[i] -= dt;
			if (powerup_timer[i] <= 0)
			{
				switch (powerup_type[i])
				{
					case 1:			// SPEED UP
					{
						speed_multiplier = 1.0f;
						break;
					}
				}
				powerup_timer[i] = 0.0f;
				powerup_type[i] = 0;
			}
		}
	}
}