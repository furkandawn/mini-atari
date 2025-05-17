/*
 * game_tetris.c
 *
 *  Created on: May 9, 2025
 *      Author: furkandawn
 */


#include "game_tetris.h"


// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "game_tetris_shapes.h"

// include other
#include <stdlib.h> // needed for rand() function

// includes end <<-----

static uint8_t board[TETRIS_BOARD_WIDTH][TETRIS_BOARD_HEIGHT] = {0};
static game_tetris_t active_tetrimino;

static void tetrimino_spawn(game_tetris_t *game)
{
	game->type = rand() % TETRIMINO_TYPE_COUNT;
	game->rotation = 0;
	game->x = (TETRIS_BOARD_WIDTH - TETRIMINO_SIZE) / 2;
	game->y = 0;

	active_tetrimino = *game;
}
