/*
 * game_init.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "game_init.h"

// ----->> includes start

// include OLED Display library

// include mini-atari libraries
#include "game_runtime.h"
#include "game_ui.h"
#include "menu_main.h"

// include mini-atari games
#include "game_snake.h"

// include other

// includes end <<-----

void game_init(void)
{
	game_draw_start_screen();
	game_reset_configs(current_game_type);

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
		break;
	}
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
	default:
	{
		break;
	}
	}
}
