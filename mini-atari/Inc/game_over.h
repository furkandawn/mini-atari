/*
 * game_over.h
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_OVER_H_
#define INC_GAME_OVER_H_

#include <stdint.h>

typedef enum
{
	GAMEOVER_PLAY_AGAIN = 0,
	GAMEOVER_MAIN_MENU,
	GAMEOVER_SAVE,
	GAMEOVER_COUNT
} game_over_action_t;

extern game_over_action_t current_gameover_action;

void game_over(void);
void game_over_draw_animated(void);
void game_over_menu_navigate(void);

#endif /* INC_GAME_OVER_H_ */
