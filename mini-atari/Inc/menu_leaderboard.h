/*
 * menu_leaderboard.h
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_LEADERBOARD_H_
#define INC_MENU_LEADERBOARD_H_

#ifndef LEADERBOARD_COUNT
#define LEADERBOARD_COUNT 3
#endif

#ifndef LEADERBOARD_TOTAL_COUNT
#define LEADERBOARD_TOTAL_COUNT	(LEADERBOARD_COUNT + 1)  // + 1 for "EXIT" option
#endif

void handle_menu_leaderboard(void);

#endif /* INC_MENU_LEADERBOARD_H_ */
