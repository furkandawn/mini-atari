/*
 * game_breakout.h
 *
 *  Created on: May 30, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_BREAKOUT_H_
#define INC_GAME_BREAKOUT_H_

#include "stdint.h"

typedef struct
{
	uint8_t x, y;
	int8_t dx, dy;
} breakout_ball_t;

typedef struct
{
	uint8_t x;
	uint8_t prev_x;
} breakout_paddle_t;

typedef struct
{
	breakout_ball_t ball;
	breakout_paddle_t paddle;
} game_breakout_t;

void game_breakout(game_breakout_t *game);

#endif /* INC_GAME_BREAKOUT_H_ */
