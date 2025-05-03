/*
 * menu_selected.h
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_SELECTED_H_
#define INC_MENU_SELECTED_H_

typedef enum
{
	ACTION_START = 0,
	ACTION_LEADERBOARD,
	ACTION_BACK,
	ACTION_COUNT
} menu_selected_action_t;

extern menu_selected_action_t current_menu_selected_action;

void handle_menu_selected(void);

#endif /* INC_MENU_SELECTED_H_ */
