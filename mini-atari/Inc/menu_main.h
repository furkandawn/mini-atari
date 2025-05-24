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
	GAME_TETRIS,
	GAME_3,
	GAME_4,
	GAME_5,
	GAME_COUNT
} game_type_t;

extern game_type_t current_game_type;
extern const char *menu_main_items[GAME_COUNT];

void handle_menu_main(void);

#endif /* INC_MENU_MAIN_H_ */
