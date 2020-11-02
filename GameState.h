#pragma once
enum GameState {
	SPLASH_SCREEN = 0,
	MAINMENU,
	GAME,
	CREDITS,
	END
};

extern enum GameState game_state;