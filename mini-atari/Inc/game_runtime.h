/*
 * game_runtime.h
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_RUNTIME_H_
#define INC_GAME_RUNTIME_H_

#include "game_config.h"
#include "menu_main.h"
#include <stdint.h>

typedef struct
{
	game_type_t game_type;
	uint16_t score;
	uint8_t level;
	uint16_t delay_ms;
} game_runtime_t;

extern game_runtime_t game_runtime;

void game_reset_configs(game_type_t type);
void game_update_progress(void);
uint16_t game_get_score(void);
uint8_t game_get_level(void);
uint8_t game_get_delay_ms(void);
uint16_t game_get_time_spent(void);

#endif /* INC_GAME_RUNTIME_H_ */
