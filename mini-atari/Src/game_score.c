/*
 * game_score.c
 *
 *  Created on: Apr 27, 2025
 *      Author: furkandawn
 */


#include "game_score.h"

// ----->> includes start

// include OLED Display library


// include mini-atari libraries
#include "menu_main.h"

// include other
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_conf.h"
#include <stdbool.h>

// includes end <<-----

#define INITIAL_DELAY_MS 120
#define SCORE_THRESHOLD_FOR_LEVEL_UP 5
#define DELAY_CHANGE_PER_LEVEL_MS 20
#define MINIMUM_DELAY_MS 40

static uint16_t current_score;
static uint8_t current_level;
static uint8_t current_delay_ms = INITIAL_DELAY_MS;

// in-game time related variables
static uint32_t start_time_ms = 0;
static bool time_reset = false;
static uint16_t frozen_time = 0;

// extern game_type_t current_game_type; // to do : make every game their own delay system

void game_reset(void)
{
	current_score = 0;
	current_level = 1;
	current_delay_ms = INITIAL_DELAY_MS;

	// time
	start_time_ms = HAL_GetTick(); // Starts measuring time
	time_reset = false;
	frozen_time = 0;
}

// static game_score game_score_init; // to do : make every game their own delay system

void game_increase_score(void)
{
	current_score++;

	if (current_score % SCORE_THRESHOLD_FOR_LEVEL_UP == 0)
	{
		current_level++;

		if (current_delay_ms > MINIMUM_DELAY_MS + DELAY_CHANGE_PER_LEVEL_MS)
		{
			current_delay_ms -= DELAY_CHANGE_PER_LEVEL_MS;
		}
		else
		{
			current_delay_ms = MINIMUM_DELAY_MS;
		}
	}
}

uint16_t game_get_score(void)
{
	return current_score;
}

uint8_t game_get_level(void)
{
	return current_level;
}

uint8_t game_get_delay_ms(void)
{
	return current_delay_ms;
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
/* to do : make every game their own delay system
static void init_all_snake(game_score *game)
{
	game->init_delay = 80;
	game->score_to_level_up = 5;
	game->delay_change_per_level = 10;
	game->minimum_delay = 30;
}

static void handle_game_delays(game_score *game)
{
	switch (current_game_type)
	{
	case GAME_SNAKE:
	{
		handle_delay_snake(game);
		break;
	}
	default : break;
	}
}
*/
