/*
 * game_tetris.c
 *
 *  Created on: May 9, 2025
 *      Author: furkandawn
 */


#include "game_tetris.h"


// === Includes Start ===

// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "game_tetris_shapes.h"
#include "joystick.h"
#include "game_runtime.h"
#include "menu_logic.h"
#include "menu_paused.h"

// include other
#include <stdlib.h>		// needed for rand() function

// === Includes End ===

#define GAME_GRID 3		// 3x3 pixel blocks


static uint8_t board[TETRIS_BOARD_WIDTH][TETRIS_BOARD_HEIGHT] = {0};
static game_tetris_t active_tetrimino;

// whenever a tetrimino spawns, get random number
// between 0 - 7 and keep that value in this variable
static uint8_t next_tetrimino;

/* menu and outline draw functions */
static void tetris_draw_board(void)
{
	for (uint8_t i = 0; i < GAME_GRID; i++)
	{
		display_draw_rectangle(0 + i, 0 + i, (TETRIS_BOARD_WIDTH * GAME_GRID) + i, (TETRIS_BOARD_HEIGHT * GAME_GRID) + i, display_color_white);
	}
}

static void tetris_draw_stats(void)
{
	uint8_t stats_position_x = (TETRIS_BOARD_WIDTH * GAME_GRID) + GAME_GRID;
}

static void tetrimino_spawn(game_tetris_t *game)
{
	game->type = next_tetrimino;
	game->rotation = 0;
	game->x = (((TETRIS_BOARD_WIDTH / 2) * GAME_GRID) + GAME_GRID);
	game->y = 0;

	active_tetrimino = *game;
	next_tetrimino = rand() % TETRIMINO_TYPE_COUNT;
}

static void tetrimino_move(game_tetris_t *game)
{
	joystick_direction_t dir = joystick_direction();

	switch(dir)
	{
	case DIRECTION_DOWN: game->y -= GAME_GRID; break;
	case DIRECTION_LEFT: if (game->x != GAME_GRID) game->x -= GAME_GRID; break;
	case DIRECTION_RIGHT: if (game->x != ((TETRIS_BOARD_WIDTH * GAME_GRID) + GAME_GRID)) game->x += GAME_GRID; break;
	default: break;
	}

	game->y -= GAME_GRID;
}

static void tetrimino_rotation(game_tetris_t *game)
{
	if (button_is_pressed())
	{
		game->rotation = ((game->rotation + 1) % tetriminos[game->type].rotation_count);
	}
}

static void tetrimino_draw(game_tetris_t *game)
{
	for (uint8_t i = 0; i < TETRIMINO_SIZE; i++)
	{
		for (uint8_t j = 0; j < TETRIMINO_SIZE; j++)
		{
			if (tetriminos[game->type].rotation[game->rotation][i][j])
			{
				uint8_t x = (game->x + (j * GAME_GRID));
				uint8_t y = (game->y + (i * GAME_GRID));
				display_fill_square(x, y, GAME_GRID, display_color_white);
			}
		}
	}
}

static void tetris_update(game_tetris_t *game)
{
	if (joystick_is_pressed()) game_pause();
	if (game_over || current_menu_state != MENU_PLAYING) return;
}
