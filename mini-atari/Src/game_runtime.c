/*
 * game_runtime.c
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */


#include "game_runtime.h"

// ===== Includes ===== //
// include mini-atari libraries
#include "game_ui.h"

// include other
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_conf.h"
#include <stdlib.h>

// ======= Macros/Constants ===== //
// ----- //

// ===== Static File-Private Variables ===== //
// in-game time spent related variables
static uint32_t start_time_ms = 0;
static uint16_t frozen_time = 0;
static uint16_t continous_time = 0;
static bool freeze_timer = false;

// ===== Public Global Variables ===== //
uint32_t paused_time;
game_runtime_t game_runtime = {0};
bool game_over = false;

// ===== Public API Function Definitions ===== //

// Initializes game configurations
void game_reset_configs(game_type_t type)
{
	// game configurations
	game_runtime.game_type = type;
	game_runtime.score = 0;
	game_runtime.level = 1;
	game_runtime.delay_ms = game_configs[type].initial_delay_ms;
	game_over = false;
	gameover_animation_shown = false;
}

// Initializes all time related variables
void game_reset_timers(void)
{
	// in-game time spent
	start_time_ms = HAL_GetTick(); // get the start time of game boot-up
	freeze_timer = false;
	frozen_time = 0;
	continous_time = 0;
	paused_time = 0;

	// RNG for game randomness
	srand(HAL_GetTick());
}

// Updates score, level and in-game delay
void game_update_progress(void)
{
	const game_config_t *cfg = &game_configs[game_runtime.game_type];
	game_runtime.score += cfg->score_per_action;

	if (game_runtime.score >= game_runtime.level * cfg->score_for_level && game_runtime.level < cfg->max_level)
	{
		game_runtime.level++;

		if (game_runtime.delay_ms - cfg->delay_decrement > cfg->min_delay_ms)
		{
			game_runtime.delay_ms -= cfg->delay_decrement;
		}
	}
}

// Getter Functions

uint16_t game_get_score(void)
{
	return game_runtime.score;
}

uint8_t game_get_level(void)
{
	return game_runtime.level;
}

uint16_t game_get_delay_ms(void)
{
	return game_runtime.delay_ms;
}

uint16_t game_get_time_spent(void)
{
	if (!freeze_timer)
	{
		frozen_time = (HAL_GetTick() - start_time_ms - paused_time) / 1000; // converts ms to seconds
		freeze_timer = true;
		return frozen_time;
	}

	return frozen_time;
}

uint16_t game_get_continous_time(void)
{
	continous_time = (HAL_GetTick() - start_time_ms - paused_time) / 1000; // converts ms to seconds
	return continous_time;
}
