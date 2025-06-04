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
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
	game_type_t game_type;
	uint16_t score;
	uint8_t level;
	uint16_t delay_ms;
	uint8_t life;
} game_runtime_t;

extern uint32_t paused_time;
extern game_runtime_t game_runtime;
extern bool game_over_flag;
extern bool game_win_flag;

void game_reset_configs(game_type_t type);
void game_reset_timers(void);
void game_update_progress(void);
void game_update_level(void);
bool game_lose_life(void);
void game_end(void);
uint16_t game_get_score(void);
uint8_t game_get_level(void);
uint16_t game_get_delay_ms(void);
uint16_t game_get_time_spent(void);
uint16_t game_get_continous_time(void);

#endif /* INC_GAME_RUNTIME_H_ */
