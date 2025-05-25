/*
 * game_init.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "game_init.h"

// === Includes Start ===

// include display library

// include mini-atari libraries
#include "game_runtime.h"
#include "game_ui.h"
#include "menu_main.h"
#include "menu_logic.h"

// include mini-atari games
#include "game_snake.h"
#include "game_tetris.h"

// include other

// === Includes End ===

static void game_init(void)
{
	switch(current_game_type)
	{
	case GAME_SNAKE:
	{
		game_snake_t game;
		snake_game(&game);
		break;
	}
	case GAME_TETRIS:
	{
		game_tetris_t game;
		game_tetris(&game);
		break;
	}
	/* to do: complete other games
	case GAME_3:
	{
		break;
	}
	case GAME_4:
	{
		break;
	}
	case GAME_5:
	{
		break;
	}
	*/
	default:
	{
		current_menu_state = MENU_MAIN;
		break;
	}
	}
}

void handle_menu_playing(void)
{
	game_reset_configs(current_game_type);
	game_draw_start_screen();

	game_init();
}
