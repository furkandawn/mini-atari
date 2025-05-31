/*
 * menu_playing.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "menu_playing.h"

// ===== Includes ===== //
// include mini-atari libraries
#include "menu_interface.h"
#include "game_runtime.h"
#include "game_ui.h"
#include "game_snake.h"
#include "game_tetris.h"
#include "game_pong.h"
#include "game_breakout.h"

// ======= Macros/Constants ===== //
// ----- //

// ===== Static File-Private Variables ===== //
// ----- //

// ===== Public Global Variables ===== //
// ----- //

// ===== Static Function Declarations ===== //
static void game_init(void);

// ===== Public API Function Definitions ===== //
void handle_menu_playing(void)
{
	game_reset_configs(current_game_type);

	input_enabled = false;
	game_draw_start_screen();
	input_enabled = true;

	game_reset_timers();
	game_init();
}

// ===== Static Function Definitions ===== //

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
	case GAME_PONG:
	{
		game_pong_t game;
		game_pong(&game);
		break;
	}
	case GAME_BREAKOUT:
	{
		game_breakout_t game;
		game_breakout(&game);
		break;
	}
	/* to do: complete other games
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
