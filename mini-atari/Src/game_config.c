/*
 * game_config.c
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */


#include "game_config.h"

const game_config_t game_configs[GAME_COUNT] =
{
		[GAME_SNAKE] =
		{
			.initial_delay_ms = 90,
			.min_delay_ms = 40,
			.delay_decrement = 10,
			.score_per_action = 1,
			.score_per_level = 10,
			.max_level = 5
		},

		[GAME_TETRIS] =
		{
			.initial_delay_ms = 0,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_per_level = 0,
			.max_level = 0
		},

		[GAME_3] =
		{
			.initial_delay_ms = 0,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_per_level = 0,
			.max_level = 0
		},

		[GAME_4] =
		{
			.initial_delay_ms = 0,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_per_level = 0,
			.max_level = 0
		},

		[GAME_5] =
		{
			.initial_delay_ms = 0,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_per_level = 0,
			.max_level = 0
		}
};
