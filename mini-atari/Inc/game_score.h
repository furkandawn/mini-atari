/*
 * game_score.h
 *
 *  Created on: Apr 27, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_SCORE_H_
#define INC_GAME_SCORE_H_

#include <stdint.h>

typedef struct
{
	uint8_t init_delay;
	uint16_t score_to_level_up;
	uint8_t delay_change_per_level;
	uint8_t minimum_delay;
} game_score_t;

void game_level_reset(void);
void game_increase_score(void);
uint16_t game_get_score(void);
uint8_t game_get_level(void);
uint8_t game_get_delay_ms(void);
uint16_t game_get_time_spent(void);

#endif /* INC_GAME_SCORE_H_ */
