/*
 * game_leaderboard.h
 *
 *  Created on: May 12, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_LEADERBOARD_H_
#define INC_GAME_LEADERBOARD_H_

#include "menu_interface.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 11
#define LEADERBOARD_COUNT 3
#define LEADERBOARD_TOTAL_COUNT	(LEADERBOARD_COUNT + 1)  // + 1 for "EXIT" option

/*
 * Do not change this, or if you change, make it divisible by 4
 * Because the current structure is being written word by word, which is 4 bytes
 */
typedef struct
{
	char name[MAX_NAME_LENGTH];					// 11 bytes
	uint16_t score;								// 2 bytes
	uint16_t time_passed;	// seconds			// 2 bytes
	uint8_t _padding;		// padding to align to 16 bytes
} leaderboard_entry_t;

extern leaderboard_entry_t current_leaderboard[GAME_COUNT][LEADERBOARD_COUNT];
extern const char *menu_leaderboard_entries[GAME_COUNT][LEADERBOARD_TOTAL_COUNT];

void load_leaderbord_from_flash(void);
void add_leaderboard_entry(game_type_t game, const char *name, uint16_t ingame_score, uint16_t time_passed);
bool is_score_eligible(game_type_t game, uint16_t score);

#endif /* INC_GAME_LEADERBOARD_H_ */
