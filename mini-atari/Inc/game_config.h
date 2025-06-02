/*
 * game_config.h
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_CONFIG_H_
#define INC_GAME_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include "menu_interface.h"

typedef struct
{
	uint16_t initial_delay_ms;
	uint16_t min_delay_ms;
	uint8_t delay_decrement;
	uint8_t score_per_action;
	uint8_t score_for_level;
	uint8_t max_level;
	bool level_up_animation;
	uint8_t life;
} game_config_t;

extern const game_config_t game_configs[GAME_COUNT];

#endif /* INC_GAME_CONFIG_H_ */
