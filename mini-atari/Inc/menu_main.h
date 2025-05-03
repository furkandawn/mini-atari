/*
 * menu_main.h
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_MAIN_H_
#define INC_MENU_MAIN_H_

typedef enum
{
	GAME_SNAKE = 0,
	GAME_2,
	GAME_3,
	GAME_4,
	GAME_5,
	GAME_COUNT,
} menu_main_item_t;

extern menu_main_item_t current_menu_main_item;
extern const char *menu_main_items[GAME_COUNT];

void handle_menu_main(void);

#endif /* INC_MENU_MAIN_H_ */
