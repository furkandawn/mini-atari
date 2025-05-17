/*
 * menu_logic.h
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_LOGIC_H_
#define INC_MENU_LOGIC_H_

#include <stdint.h>

#define BOARD_WIDTH 	128
#define BOARD_HEIGHT 	64

typedef enum
{
	MENU_MAIN = 0,
	MENU_SELECTED,
	MENU_LEADERBOARD,
	MENU_PLAYING,
	MENU_PAUSED,
	MENU_GAMEOVER,
	MENU_SAVE
} menu_state_t;

extern menu_state_t current_menu_state;

void navigate_menu_up_down(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));
void navigate_menu_right_left(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));

void navigate_menu_up_down_loop(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));
void navigate_menu_right_left_loop(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));

#endif /* INC_MENU_LOGIC_H_ */
