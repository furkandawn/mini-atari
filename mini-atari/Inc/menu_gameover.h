/*
 * menu_gameover.h
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_GAMEOVER_H_
#define INC_MENU_GAMEOVER_H_

#include <stdbool.h>

typedef enum
{
	GAMEOVER_PLAY_AGAIN = 0,
	GAMEOVER_MAIN_MENU,
	GAMEOVER_SAVE,
	GAMEOVER_COUNT
} gameover_action_t;

extern gameover_action_t current_menu_gameover_action;
extern bool animation_shown;

void handle_menu_gameover(void);

#endif /* INC_MENU_GAMEOVER_H_ */
