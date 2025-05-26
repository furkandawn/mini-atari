/*
 * menu_logic.h
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */

#ifndef INC_MENU_LOGIC_H_
#define INC_MENU_LOGIC_H_

#include <stdint.h>

void navigate_menu_up_down(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));
void navigate_menu_right_left(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));

void navigate_menu_up_down_loop(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));
void navigate_menu_right_left_loop(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t));

#endif /* INC_MENU_LOGIC_H_ */
