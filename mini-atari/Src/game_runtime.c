/*
 * game_runtime.c
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */


#include "game_runtime.h"

// === Includes Start ===

// include display library

// include mini-atari libraries
#include "menu_main.h"
#include "game_config.h"
#include "game_ui.h"

// include other
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_conf.h"
#include <stdbool.h>
#include <stdlib.h>

// === Includes End ===

game_runtime_t game_runtime = {0};

// in-game time spent related variables
static uint32_t start_time_ms = 0;
static bool time_reset = false;
static uint16_t frozen_time = 0;

/* Game initialization */
void game_reset_configs(game_type_t type)
{
	// game configurations
	game_runtime.game_type = type;
	game_runtime.score = 0;
	game_runtime.level = 1;
	game_runtime.delay_ms = game_configs[type].initial_delay_ms;

	// in-game time spent
	start_time_ms = HAL_GetTick(); // Starts measuring time
	time_reset = false;
	frozen_time = 0;

	// RNG for game randomness
	srand(HAL_GetTick());
}

/* Update score, level and delay */
void game_update_progress(void)
{
	const game_config_t *cfg = &game_configs[game_runtime.game_type];
	game_runtime.score += cfg->score_per_action;

	if (game_runtime.score >= game_runtime.level * cfg->score_per_level && game_runtime.level < cfg->max_level)
	{
		game_runtime.level++;
		game_draw_level_screen();

		if (game_runtime.delay_ms > cfg->min_delay_ms)
		{
			game_runtime.delay_ms -= cfg->delay_decrement;
		}
	}
}

uint16_t game_get_score(void)
{
	return game_runtime.score;
}

uint8_t game_get_level(void)
{
	return game_runtime.level;
}

uint8_t game_get_delay_ms(void)
{
	return game_runtime.delay_ms;
}

uint16_t game_get_time_spent(void)
{
	if (!time_reset)
	{
		frozen_time = (HAL_GetTick() - start_time_ms) / 1000; // converts ms to seconds
		time_reset = true;
	}

	return frozen_time;
}
