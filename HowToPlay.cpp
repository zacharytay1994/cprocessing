#include "HowToPlay.h"
#include "GameGUI.h"
#include "Sprite.h"
#include "Scene.h"

#define HOWTOPLAY_MAX_IMAGES 6

#define HTP_X_KEYBOARD 0.5f
#define HTP_Y_KEYBOARD 0.5f
#define HTP_WIDTH_KEYBOARD 1.0f
#define HTP_HEIGHT_KEYBOARD 1.0f

#define HTP_X_TAB 0.5f
#define HTP_Y_TAB 0.5f
#define HTP_WIDTH_TAB 1.0f
#define HTP_HEIGHT_TAB 1.0f

#define HTP_X_UP 0.5f
#define HTP_Y_UP 0.5f
#define HTP_WIDTH_UP 1.0f
#define HTP_HEIGHT_UP 1.0f

#define HTP_X_LEFT 0.5f
#define HTP_Y_LEFT 0.5f
#define HTP_WIDTH_LEFT 1.0f
#define HTP_HEIGHT_LEFT 1.0f

#define HTP_X_RIGHT 0.5f
#define HTP_Y_RIGHT 0.5f
#define HTP_WIDTH_RIGHT 1.0f
#define HTP_HEIGHT_RIGHT 1.0f

#define HTP_X_LEFTMOUSE 0.7f
#define HTP_Y_LEFTMOUSE 0.4f
#define HTP_WIDTH_LEFTMOUSE 0.5f
#define HTP_HEIGHT_LEFTMOUSE 0.5f

#define HTP_X_RIGHTMOUSE 0.5f
#define HTP_Y_RIGHTMOUSE 0.5f
#define HTP_WIDTH_RIGHTMOUSE 1.0f
#define HTP_HEIGHT_RIGHTMOUSE 1.0f

typedef struct HowToPlayImage {
	int			_image;
	int			_visible;
	float		_alpha;
	float		_fade_in;
	float		_fade_out;
} HowToPlayImage;

// tab wasd arrowkeys left mouse right mouse
HowToPlayImage HowToPlay_images[HOWTOPLAY_MAX_IMAGES] = { 0 };
int HowToPlay_images_size = 0;

CP_Image HowToPlay_background;
CP_Image HowToPlay_keyboard;

float HTP_window_width;
float HTP_window_height;

void HowToPlay_Init()
{
	GameGUI_Init();
	HTP_window_width = (float)CP_System_GetWindowWidth();
	HTP_window_height = (float)CP_System_GetWindowHeight();
	HowToPlay_background = CP_Image_Load("./Assets/HowToPlay/howtoplay_background.png");
	// loading images
	HowToPlay_AddImage(Sprite_AddSprite((CP_Vector) { HTP_window_width*HTP_X_KEYBOARD, HTP_window_height
	*HTP_Y_KEYBOARD}, HTP_window_width*HTP_WIDTH_KEYBOARD, HTP_window_height*HTP_HEIGHT_KEYBOARD, "./Assets/HowToPlay/howtoplay_keyboard.jpg",
		2, 1, 2, 2, 1), 5.0f, 1.0f);
	HowToPlay_AddImage(Sprite_AddSprite((CP_Vector) {
		HTP_window_width* HTP_X_TAB, HTP_window_height*HTP_Y_TAB },
		HTP_window_width* HTP_WIDTH_TAB, HTP_window_height* HTP_HEIGHT_TAB, "./Assets/HowToPlay/howtoplay_tab.png",
		2, 3, 6, 10, 1), 5.0f, 1.0f);
	HowToPlay_AddImage(Sprite_AddSprite((CP_Vector) {
		HTP_window_width* HTP_X_UP, HTP_window_height*HTP_Y_UP },
		HTP_window_width* HTP_WIDTH_UP, HTP_window_height* HTP_HEIGHT_UP, "./Assets/HowToPlay/howtoplay_up.png",
		2, 1, 2, 2, 1), 5.0f, 1.0f);
	HowToPlay_AddImage(Sprite_AddSprite((CP_Vector) {
		HTP_window_width* HTP_X_LEFT, HTP_window_height*HTP_Y_LEFT },
		HTP_window_width* HTP_WIDTH_LEFT, HTP_window_height* HTP_HEIGHT_LEFT, "./Assets/HowToPlay/howtoplay_left.png",
		2, 1, 2, 2, 1), 5.0f, 1.0f);
	HowToPlay_AddImage(Sprite_AddSprite((CP_Vector) {
		HTP_window_width* HTP_X_RIGHT, HTP_window_height*HTP_Y_RIGHT },
		HTP_window_width* HTP_WIDTH_RIGHT, HTP_window_height* HTP_HEIGHT_RIGHT, "./Assets/HowToPlay/howtoplay_right.png",
		2, 1, 2, 2, 1), 5.0f, 1.0f);
	HowToPlay_AddImage(Sprite_AddSprite((CP_Vector) {
		HTP_window_width* HTP_X_LEFTMOUSE, HTP_window_height*HTP_Y_LEFTMOUSE },
		HTP_window_width* HTP_WIDTH_LEFTMOUSE, HTP_window_height* HTP_HEIGHT_LEFTMOUSE, "./Assets/HowToPlay/howtoplay_leftmouse.jpg",
		2, 3, 6, 10, 1), 2.0f, 2.0f);
	HowToPlay_AddImage(Sprite_AddSprite((CP_Vector) {
		HTP_window_width* HTP_X_RIGHTMOUSE, HTP_window_height*HTP_Y_RIGHTMOUSE }, 
		HTP_window_width* HTP_WIDTH_RIGHTMOUSE, HTP_window_height* HTP_HEIGHT_RIGHTMOUSE, "./Assets/HowToPlay/howtoplay_rightmouse.png",
		2, 1, 2, 2, 1), 5.0f, 1.0f);
	HowToPlay_SetKeyVisible(HTP_Keyboard, 1);
}

void HowToPlay_Update(const float dt)
{
	// Render background
	//GameGUI_DrawImage(HowToPlay_background, 0.5f, 0.5f, 1.0f, 1.0f, 255);
	//HowToPlay_SetKeyVisible(HTP_Keyboard, CP_Input_KeyReleased(KEY_P));
	HowToPlay_SetKeyVisible(HTP_Up, CP_Input_KeyDown(KEY_W) || CP_Input_KeyDown(KEY_UP));
	HowToPlay_SetKeyVisible(HTP_Left, CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_LEFT));
	HowToPlay_SetKeyVisible(HTP_Right, CP_Input_KeyDown(KEY_D) || CP_Input_KeyDown(KEY_RIGHT));
	HowToPlay_SetKeyVisible(HTP_Tab, CP_Input_KeyDown(KEY_TAB));
	HowToPlay_SetKeyVisible(HTP_LeftMouse, CP_Input_MouseDown(0));
	HowToPlay_SetKeyVisible(HTP_RightMouse, CP_Input_MouseDown(1));
	HowToPlay_ImageUpdate(dt);
	if (CP_Input_KeyDown(KEY_ESCAPE)) {
		Scene_ChangeScene(3);
	}
}

void HowToPlay_Exit()
{
}

void HowToPlay_AddImage(const int img, const float fadeIn, const float fadeOut)
{
	if (HowToPlay_images_size < HOWTOPLAY_MAX_IMAGES) {
		HowToPlay_images[HowToPlay_images_size++] = (HowToPlayImage){ img, 0, 255.0f, fadeIn,fadeOut };
	}
}

void HowToPlay_SetKeyVisible(const HTP_Image img, const int b)
{
	HowToPlay_images[img]._visible = b;
}

void HowToPlay_ImageUpdate(const float dt)
{
	for (int i = 0; i < HOWTOPLAY_MAX_IMAGES; ++i) {
		HowToPlayImage* img = &HowToPlay_images[i];
		if (img->_visible) {
			if (img->_alpha < 255.0f) {
				img->_alpha += img->_fade_in*dt*255.0f;
			}
		}
		else {
			if (img->_alpha > 0.0f) {
				img->_alpha -= img->_fade_out*dt*255.0f;
			}
		}
		Sprite_SetAlpha(img->_image, img->_alpha);
		Sprite_RenderSprite(dt, HowToPlay_images[i]._image);
	}
}