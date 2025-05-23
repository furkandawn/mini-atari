/*
 * game_tetris_shapes.h
 *
 *  Created on: May 13, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_TETRIS_SHAPES_H_
#define INC_GAME_TETRIS_SHAPES_H_

#define TETRIMINO_SIZE 4	// GRID size

#define TETRIMINO_MAX_ROTATION 4

#include <stdint.h>

typedef enum {
    TETRIMINO_I = 0,
    TETRIMINO_O,
    TETRIMINO_T,
    TETRIMINO_S,
    TETRIMINO_Z,
    TETRIMINO_J,
    TETRIMINO_L,
	TETRIMINO_TYPE_COUNT
} tetrimino_type_t;

typedef struct
{
	uint8_t rotation[TETRIMINO_MAX_ROTATION][TETRIMINO_SIZE][TETRIMINO_SIZE];  // [rotation][row][col]
	uint8_t rotation_count;
} tetrimino;

extern tetrimino_type_t current_tetrimino;

extern const tetrimino tetriminos[TETRIMINO_TYPE_COUNT];

#endif /* INC_GAME_TETRIS_SHAPES_H_ */
