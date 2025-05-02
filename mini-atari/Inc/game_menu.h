/*
 * game_menu.h
 *
 *  Created on: Apr 21, 2025
 *      Author: furkandawn
 *      https://github.com/furkandawn
 */
#ifndef INC_GAME_MENU_H_
#define INC_GAME_MENU_H_

#include <stdint.h>

typedef enum
{
	GAME_SNAKE = 0,
	GAME_2,
	GAME_3,
	GAME_4,
	GAME_5,
	GAME_COUNT,
} game_type_t;

typedef enum
{
	ACTION_START = 0,
	ACTION_LEADERBOARD,
	ACTION_BACK,
	ACTION_COUNT
} action_type_t;

typedef enum
{
	MENU_MAIN = 0,
	MENU_SELECTED,
	MENU_LEADERBOARD,
	MENU_PLAYING,
	MENU_GAMEOVER,
	MENU_SAVE,
	MENU_PAUSED
} menu_state_t;

typedef enum
{
	PAUSED_CONTINUE = 0,
	PAUSED_EXIT,
	PAUSED_COUNT
} paused_action_t;

extern menu_state_t current_menu_state;
void pause_game(void);

void navigate_menu_up_down(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));
void navigate_menu_right_left(uint8_t *index, uint8_t maxItems, void (*draw_func)(uint8_t));
void game_menu (void);

#endif /* INC_GAME_MENU_H_ */
