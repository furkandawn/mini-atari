/*
 * menu_save.h
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_SAVE_H_
#define INC_MENU_SAVE_H_

#include <stdint.h>

#define MAX_NAME_LENGTH 11

typedef enum
{
	SAVE_SAVE = 0,
	SAVE_BKSP,
	SAVE_EXIT,
	SAVE_COUNT
} menu_save_action_t;

extern char current_menu_save_name[MAX_NAME_LENGTH];

char get_saved_name(void);
void handle_menu_save(void);

#endif /* INC_MENU_SAVE_H_ */
