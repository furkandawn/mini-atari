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

// include other
#include <stdlib.h>		// needed for rand() function

// === Includes End ===

#define GAME_GRID 3		// 3x3 pixel blocks


static uint8_t board[TETRIS_BOARD_WIDTH][TETRIS_BOARD_HEIGHT] = {0};
static game_tetris_t active_tetrimino;
static uint8_t next_tetrimino;		// whenever a tetrimino spawns, get random number between 0 - 7
									// and keep that value in this variable
static uint8_t stats_position_x;

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
	stats_position_x = (TETRIS_BOARD_WIDTH * GAME_GRID) + GAME_GRID;
}

static void tetrimino_spawn(game_tetris_t *game)
{
	game->type = rand() % TETRIMINO_TYPE_COUNT;
	game->rotation = 0;
	game->x = (TETRIS_BOARD_WIDTH - GAME_GRID) / 2;
	game->y = 0;

	active_tetrimino = *game;
}
