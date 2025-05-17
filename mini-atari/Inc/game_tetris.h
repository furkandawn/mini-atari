/*
 * game_tetris.h
 *
 *  Created on: May 9, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_TETRIS_H_
#define INC_GAME_TETRIS_H_

#include <stdint.h>

#define TETRIS_BLOCK_SIZE 3 	// game grid 3x3

#define TETRIS_BOARD_WIDTH 10
#define TETRIS_BOARD_HEIGHT 20

#ifndef TETRIMINO_TYPE_COUNT
#define TETRIMINO_TYPE_COUNT 7
#endif

typedef struct
{
	uint8_t x, y;		// x & y position of the active tetrimino. (top left position).
	uint8_t rotation;	// tetrimino rotated version
	uint8_t type;		// tetrimino type
} game_tetris_t ;


#endif /* INC_GAME_TETRIS_H_ */
