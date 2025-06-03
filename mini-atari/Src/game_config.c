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
			.level_up_animation = true,
			.life = 1
		},

		[GAME_TETRIS] =
		{
			.initial_delay_ms = 450,
			.min_delay_ms = 250,
			.delay_decrement = 50,
			.score_per_action = 10,
			.score_for_level = 50,
			.max_level = 5,
			.level_up_animation = false,
			.life = 1
		},

		[GAME_PONG] =
		{
			.initial_delay_ms = 20,
			.min_delay_ms = 20,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_for_level = 0,
			.max_level = 1,
			.level_up_animation = false,
			.life = 1
		},

		[GAME_BREAKOUT] =
		{
			.initial_delay_ms = 1,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 10,
			.score_for_level = 0,
			.max_level = 0,
			.level_up_animation = false,
			.life = 3
		},

		[GAME_SPCINV] =
		{
			.initial_delay_ms = 1,
			.min_delay_ms = 0,
			.delay_decrement = 0,
			.score_per_action = 0,
			.score_for_level = 0,
			.max_level = 0,
			.level_up_animation = false,
			.life = 3
		}
};
