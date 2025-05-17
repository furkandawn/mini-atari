/*
 * menu_paused.h
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_PAUSED_H_
#define INC_MENU_PAUSED_H_

#define PAUSED_COUNT 2 // enum count

typedef enum
{
	PAUSED_CONTINUE = 0,
	PAUSED_EXIT
} menu_paused_action_t;

void game_pause(void);

#endif /* INC_MENU_PAUSED_H_ */
