/*
 * menu_logic.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_logic.h"

// ----->> includes start

// include OLED Display library


// include mini-atari libraries
#include "joystick.h"

// include other
#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

// includes end <<-----

menu_state_t current_menu_state = MENU_MAIN;

/*
 *    Navigate functions without loop
 */
void navigate_menu_up_down(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t))
{
	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_UP && *index != 0)
	{
		(*index)--;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_DOWN && *index < max_items - 1)
	{
		(*index)++;
		HAL_Delay(100);
	}

	draw_func(*index);
}

void navigate_menu_right_left(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t))
{
	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_LEFT && *index != 0)
	{
		(*index)--;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_RIGHT && *index < max_items - 1)
	{
		(*index)++;
		HAL_Delay(100);
	}

	draw_func(*index);
}


/*
 * looped versions of navigate functions
 * Either
 * Jump to the first element from last
 * or jump to the last element from first
 */
void navigate_menu_up_down_loop(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t))
{
	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_UP)
	{
		*index = (*index + max_items - 1) % max_items;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_DOWN)
	{
		*index = (*index + 1) % max_items;
		HAL_Delay(100);
	}

	draw_func(*index);
}

void navigate_menu_right_left_loop(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t))
{
	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_LEFT)
	{
		*index = (*index + max_items - 1) % max_items;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_RIGHT)
	{
		*index = (*index + 1) % max_items;
		HAL_Delay(100);
	}

	draw_func(*index);
}
