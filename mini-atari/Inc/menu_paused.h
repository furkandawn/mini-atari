/*
 * menu_paused.h
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_PAUSED_H_
#define INC_MENU_PAUSED_H_

typedef enum
{
	PAUSED_CONTINUE = 0,
	PAUSED_EXIT,
	PAUSED_COUNT
} menu_paused_action_t;

void game_pause(void);

#endif /* INC_MENU_PAUSED_H_ */
