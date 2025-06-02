/*
 * game_space_invaders.h
 *
 *  Created on: May 31, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_SPACE_INVADERS_H_
#define INC_GAME_SPACE_INVADERS_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t x;
	uint8_t bullet_delay_ms;
} spcinv_player_t;

typedef struct
{
	uint8_t x, y;
	uint8_t speed_delay_ms;
	uint8_t bullet_delay_ms;
	uint8_t hit_point;
	bool alive;
} spcinv_enemy_t;

typedef struct
{
	spcinv_player_t player;
	spcinv_enemy_t enemy;
} game_spcinv_t;

void game_spcinv(game_spcinv_t *game);

#endif /* INC_GAME_SPACE_INVADERS_H_ */
