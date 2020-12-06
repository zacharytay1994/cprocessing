#include "GameGUI.h"

float GameGUI_screen_width;
float GameGUI_screen_height;
float GameGUI_screen_half_width;
float GameGUI_screen_half_height;

CP_Image GameGUI_player_health_frame;
CP_Image GameGUI_player_health_bar;
CP_Image GameGUI_player_health_frame2;
CP_Image GameGUI_player_health_bar2;
CP_Vector GameGUI_player_health_position;
CP_Vector GameGUI_player_health_size;
CP_Vector GameGUI_player_healthicon_position;
CP_Vector GameGUI_player_healthicon_size;
CP_Vector GameGUI_player_health_offset;
float health_ratio = 1.0f;
float health_ratio_lerp = 1.0f;

CP_Image GameGUI_red_overlay;
float GameGUI_red_overlay_ratio = 0.0f; 
float Game_GUI_red_overlay_decrement = 1000.0f;

void GameGUI_Init()
{
	GameGUI_screen_width = (float)CP_System_GetWindowWidth();
	GameGUI_screen_height = (float)CP_System_GetWindowHeight();
	GameGUI_screen_half_width = GameGUI_screen_width / 2.0f;
	GameGUI_screen_half_height = GameGUI_screen_height / 2.0f;

	// load images
	GameGUI_player_health_frame = CP_Image_Load("./Sprites/health_frame.png");
	GameGUI_player_health_frame2 = CP_Image_Load("./Sprites/health_frame2.png");
	GameGUI_player_health_bar = CP_Image_Load("./Sprites/health_bar.png");
	GameGUI_player_health_bar2 = CP_Image_Load("./Sprites/health_bar2.png");
	GameGUI_red_overlay = CP_Image_Load("./Sprites/on_hit_overlay.png");
	GameGUI_player_healthicon_position = (CP_Vector){ 0.2f,0.2f };
	GameGUI_player_healthicon_size = (CP_Vector){ 0.3f, 0.1f };
	GameGUI_player_health_position = (CP_Vector){ 0.19f,0.2f };
	GameGUI_player_health_size = (CP_Vector){ 0.1f, 0.1f };
	GameGUI_player_health_offset = (CP_Vector){ -0.05f*GameGUI_screen_width,-0.15f*GameGUI_screen_height };
}

void GameGUI_Render(const float dt)
{
	GameGUI_RenderHitOverlay(dt);
	// calculate bar scale and position
	float x_offset = ((GameGUI_player_health_size.x * GameGUI_screen_width * (1.0f - health_ratio)) / 2.0f);
	float x_offset_2 = ((GameGUI_player_health_size.x * GameGUI_screen_width * (1.0f - health_ratio_lerp)) / 2.0f);
	CP_Image_Draw(GameGUI_player_health_bar2, GameGUI_player_health_offset.x + GameGUI_player_health_position.x * GameGUI_screen_width - x_offset_2,
		GameGUI_player_health_offset.y + GameGUI_player_health_position.y * GameGUI_screen_height,
		GameGUI_player_health_size.x * GameGUI_screen_width * health_ratio_lerp, GameGUI_player_health_size.y * GameGUI_screen_height, 255);
	CP_Image_Draw(GameGUI_player_health_bar, GameGUI_player_health_offset.x + GameGUI_player_health_position.x*GameGUI_screen_width-x_offset, 
		GameGUI_player_health_offset.y + GameGUI_player_health_position.y*GameGUI_screen_height,
		GameGUI_player_health_size.x*GameGUI_screen_width*health_ratio, GameGUI_player_health_size.y*GameGUI_screen_height, 255);
	CP_Image_Draw(GameGUI_player_health_frame2, GameGUI_player_health_offset.x + GameGUI_player_health_position.x * GameGUI_screen_width,
		GameGUI_player_health_offset.y + GameGUI_player_health_position.y * GameGUI_screen_height,
		GameGUI_player_health_size.x * GameGUI_screen_width, GameGUI_player_health_size.y * GameGUI_screen_height, 255);
	CP_Image_Draw(GameGUI_player_health_frame, GameGUI_player_health_offset.x + GameGUI_player_healthicon_position.x * GameGUI_screen_width, 
		GameGUI_player_health_offset.y + GameGUI_player_healthicon_position.y * GameGUI_screen_height,
		GameGUI_player_healthicon_size.x * GameGUI_screen_width, GameGUI_player_healthicon_size.y * GameGUI_screen_height, 255);
	// lerp health scale lerp to health scale
	if (health_ratio_lerp - health_ratio > 0.0f) {
		health_ratio_lerp -= 0.2f * dt;
	}
}

void GameGUI_DrawText(CP_Color colour, const char* text, const float x, const float y, const float size, const CP_TEXT_ALIGN_HORIZONTAL h, const CP_TEXT_ALIGN_VERTICAL v)
{
	CP_Settings_Fill(colour);
	CP_Settings_TextSize(size*GameGUI_screen_width);
	CP_Settings_TextAlignment(h, v);
	CP_Font_DrawText(text, x*GameGUI_screen_width, y*GameGUI_screen_height);
}

void GameGUI_SetHealthScale(const float scale)
{
	health_ratio = scale;
	// shake screen

}

void GameGUI_RenderHitOverlay(const float dt)
{
	// decrease red overlay ratio
	if (GameGUI_red_overlay_ratio > 0.0f) {
		GameGUI_red_overlay_ratio -= Game_GUI_red_overlay_decrement * dt;
	}
	CP_Settings_BlendMode(CP_BLEND_ADD);
	CP_Image_Draw(GameGUI_red_overlay, GameGUI_screen_half_width, GameGUI_screen_half_height, GameGUI_screen_width, GameGUI_screen_height, (int)GameGUI_red_overlay_ratio);
	CP_Settings_BlendMode(CP_BLEND_ALPHA);
}

void GameGUI_SetRedHitRatio(const float value)
{
	GameGUI_red_overlay_ratio = value;
}
