/*
 * game_level.h
 *
 *  Created on: Apr 27, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_LEVEL_H_
#define INC_GAME_LEVEL_H_

#include <stdint.h>

void game_level_reset(void);
void game_score_increase(void);
uint8_t game_get_score(void);
uint8_t game_get_level(void);
uint8_t game_get_delay_ms(void);

#endif /* INC_GAME_LEVEL_H_ */
