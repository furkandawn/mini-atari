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

#define SPCINV_MAX_ENEMIES 30

typedef enum
{
	SPCINV_SQUID = 0,
	SPCINV_CRAB,
	SPCINV_OCTOPUS,
	SPCINV_UFO,
	SPCINV_ENEMY_TYPE_COUNT
} spcinv_enemy_type_t;

typedef struct
{
	int16_t x, y;
	int8_t dy;
	bool active;
} spcinv_bullet_t;

typedef struct
{
	int16_t x;
	spcinv_bullet_t bullet;
} spcinv_player_t;

typedef struct
{
	int16_t x, y;
	int8_t dx;
	uint16_t points;
	uint8_t life;
	bool alive;
	uint8_t horde_number;
	uint32_t movement_delay_ms;
	uint32_t last_move_ms;
	uint32_t bullet_delay_ms;
	uint32_t last_bullet_ms;
	spcinv_enemy_type_t type;
	spcinv_bullet_t bullet;
} spcinv_enemy_t;

typedef struct
{
	spcinv_player_t player;
	spcinv_enemy_t enemy[SPCINV_MAX_ENEMIES];
	uint8_t enemy_count;
} game_spcinv_t;

void game_spcinv(game_spcinv_t *game);

#endif /* INC_GAME_SPACE_INVADERS_H_ */
