/*
 * menu_save.h
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_SAVE_H_
#define INC_MENU_SAVE_H_

#include <stdint.h>

#define SAVE_COUNT 3 // enum count


typedef enum
{
	SAVE_SAVE = 0,
	SAVE_BKSP,
	SAVE_EXIT
} menu_save_action_t;

uint8_t get_name_length(void);
void handle_menu_save(void);

#endif /* INC_MENU_SAVE_H_ */
