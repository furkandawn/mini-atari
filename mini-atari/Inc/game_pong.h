/*
 * game_pong.h
 *
 *  Created on: May 26, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_PONG_H_
#define INC_GAME_PONG_H_

#include "stdint.h"

typedef struct
{
	uint8_t x, y;
	uint8_t old_x, old_y, old_old_x, old_old_y;
	int8_t dx, dy;
} pong_ball_t;

typedef struct
{
	uint8_t y;
	uint8_t old_y;
	uint8_t score;
} pong_paddle_t;

typedef struct
{
	pong_ball_t ball;
	pong_paddle_t player, cpu;
} game_pong_t;

void game_pong(game_pong_t *game);

#endif /* INC_GAME_PONG_H_ */
