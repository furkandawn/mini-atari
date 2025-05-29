/*
 * game_config.c
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */


#include "game_config.h"

// ===== Public Global Variables ===== //
const game_config_t game_configs[GAME_COUNT] =
{
		[GAME_SNAKE] =
		{
			.initial_delay_ms = 90,
			.min_delay_ms = 50,
			.delay_decrement = 10,
			.score_per_action = 1,
			.score_for_level = 10,
			.max_level = 5,
			.level_up_animation = true
		},

		[GAME_TETRIS] =
		{
			.initial_delay_ms = 300,
			.min_delay_ms = 100,
			.delay_decrement = 50,
			.score_per_action = 10,
			.score_for_level = 50,
			.max_level = 5,
			.level_up_animation = false
		},

		[GAME_3] =
		{
			.initial_delay_ms = 0,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_for_level = 0,
			.max_level = 0,
			.level_up_animation = false
		},

		[GAME_4] =
		{
			.initial_delay_ms = 0,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_for_level = 0,
			.max_level = 0,
			.level_up_animation = false
		},

		[GAME_5] =
		{
			.initial_delay_ms = 0,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_for_level = 0,
			.max_level = 0,
			.level_up_animation = false
		}
};
