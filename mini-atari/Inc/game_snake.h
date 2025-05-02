/*
 * game_snake.h
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#ifndef INC_GAME_SNAKE_H_
#define INC_GAME_SNAKE_H_

#include <stdint.h>
#include <stdbool.h>
#include "joystick.h"

#define SNAKE_MAX_LENGTH 128
#define BOARD_WIDTH 128
#define BOARD_HEIGHT 64
#define BLOCK_SIZE 4  // Every snake piece is 4x4 pixel

typedef struct // x and y coordinates of a specific point
{
	uint8_t x;
	uint8_t y;
} point_t;

typedef struct
{
	point_t segments[SNAKE_MAX_LENGTH];  // segments[0] is the head of the snake
	uint8_t length;  // snake length
	joystick_direction_t direction;  // snake direction
	point_t food;  // location of bait
	bool game_over;  // current game state
} game_snake_t;

void snake_game(game_snake_t *game);

#endif /* INC_GAME_SNAKE_H_ */
