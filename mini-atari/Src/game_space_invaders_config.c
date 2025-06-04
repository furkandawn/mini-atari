/*
 * game_space_invaders_config.c
 *
 *  Created on: Jun 4, 2025
 *      Author: furkandawn
 */


#include "game_space_invaders_config.h"
#include "display_config.h"

#define GAME_GRID 3 // change GAME_GRID in "game_space_invaders.c" as well if you change this

const spcinv_enemy_t enemy_config[SPCINV_ENEMY_TYPE_COUNT] =
{
		[SPCINV_SQUID] =
		{
				.x = 0,
				.y = 0,
				.dx = GAME_GRID,
				.points = 10,
				.life = 1,
				.alive = true,
				.horde_number = 7,
				.movement_delay_ms = 1000,
				.last_move_ms = 0,
				.bullet_delay_ms = 7000,
				.last_bullet_ms = 0,
				.type = SPCINV_SQUID
		},

		[SPCINV_CRAB] =
		{
				.x = 0,
				.y = 0,
				.dx = GAME_GRID,
				.points = 30,
				.life = 2,
				.alive = true,
				.horde_number = 5,
				.movement_delay_ms = 700,
				.last_move_ms = 0,
				.bullet_delay_ms = 6000,
				.last_bullet_ms = 0,
				.type = SPCINV_CRAB
		},

		[SPCINV_OCTOPUS] =
		{
				.x = 0,
				.y = 0,
				.dx = GAME_GRID,
				.points = 50,
				.life = 3,
				.alive = true,
				.horde_number = 5,
				.movement_delay_ms = 400,
				.last_move_ms = 0,
				.bullet_delay_ms = 5000,
				.last_bullet_ms = 0,
				.type = SPCINV_OCTOPUS
		},

		[SPCINV_UFO] =
		{
				.x = ((DISPLAY_WIDTH - ENTITY_WIDTH_PX) / 2),
				.y = 0,
				.dx = (GAME_GRID * 2),
				.points = 9999,
				.life = 10,
				.alive = true,
				.horde_number = 1,
				.movement_delay_ms = 200,
				.last_move_ms = 0,
				.bullet_delay_ms = 1500,
				.last_bullet_ms = 0,
				.type = SPCINV_UFO
		}
};

// ----- Bitmap Size Definitions ----- //
const uint8_t spaceship_bitmap[SPACESHIP_BITMAP_SIZE] =
{
		0x10,	// first row
		0xBA,
		0xEE,
		0xEE,
		0xFE,
		0xFE,
		0x7C,
		0x28	// last row
};

const uint8_t enemy_bitmap[SPCINV_ENEMY_TYPE_COUNT][ENTITY_BITMAP_SIZE] =
{
		[SPCINV_SQUID] =
		{
				0x18, 0x00, 	// first row
				0x3C, 0x00,
				0x7E, 0x00,
				0xDB, 0x00,
				0xFF, 0x00,
				0x24, 0x00,
				0x5A, 0x00,
				0xA5, 0x00 	// last row
		},

		[SPCINV_CRAB] =
		{
				0x20, 0x80,	// first row
				0x11, 0x00,
				0x3F, 0x80,
				0x6E, 0xC0,
				0xFF, 0xE0,
				0xBF, 0xA0,
				0xA0, 0xA0,
				0x1B, 0x00	// last row
		},

		[SPCINV_OCTOPUS] =
		{
				0x0F, 0x00,	// first row
				0x7F, 0xE0,
				0xFF, 0xF0,
				0xE6, 0x70,
				0xFF, 0xF0,
				0x19, 0x80,
				0x36, 0xC0,
				0xC0, 0x30	// last row
		},

		[SPCINV_UFO] =
		{
				0x07, 0xE0,	// first row
				0x1F, 0xF8,
				0x3F, 0xFC,
				0x6D, 0xB6,
				0xFF, 0xFF,
				0x39, 0x9C,
				0x10, 0x08,
				0x00, 0x00	// last row
		}
};
