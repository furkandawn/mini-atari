/*
 * menu_leaderboard.h
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_LEADERBOARD_H_
#define INC_MENU_LEADERBOARD_H_

#include <stdint.h>

#define MAX_NAME_LENGTH 11
#define LEADERBOARD_TOP_COUNT 3

typedef struct
{
	char name[MAX_NAME_LENGTH];
	uint8_t score;
} leaderboard_entry_t;

extern leaderboard_entry_t leaderboard[LEADERBOARD_TOP_COUNT];

#endif /* INC_MENU_LEADERBOARD_H_ */
