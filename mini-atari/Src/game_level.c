/*
 * game_level.c
 *
 *  Created on: Apr 27, 2025
 *      Author: furkandawn
 */


#include "game_level.h"

#define SCORE_THRESHOLD_FOR_LEVEL_UP 5
#define INITIAL_DELAY_MS 120
#define DELAY_CHANGE_PER_LEVEL_MS 20
#define MINIMUM_DELAY_MS 40

static uint8_t current_score;
static uint8_t current_level;
static uint8_t current_delay_ms = INITIAL_DELAY_MS;

void game_level_reset(void)
{
	current_score = 0;
	current_level = 1;
	current_delay_ms = INITIAL_DELAY_MS;
}

void game_score_increase(void)
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

uint8_t game_get_score(void)
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
