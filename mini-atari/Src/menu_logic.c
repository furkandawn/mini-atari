/*
 * menu_logic.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_logic.h"

// ===== Includes ===== //

// include display library

// include mini-atari libraries
#include "menu_interface.h"
#include "joystick.h"

// include other
#include "stm32f0xx_hal.h"
#include <stdbool.h>

// ===== Static File-Private Variables ===== //
static bool draw_flag_1 = true;
static bool draw_flag_2 = true;
static bool draw_flag_3 = true;
static bool draw_flag_4 = true;

// ===== Public Global Variables ===== //
menu_state_t current_menu_state = MENU_MAIN;

// ===== Public API Function Definitions ===== //
/*
 *    Navigate functions without loop
 */
void navigate_menu_up_down(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t))
{
	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_UP && *index != 0)
	{
		(*index)--;
		draw_flag_1 = true;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_DOWN && *index < max_items - 1)
	{
		(*index)++;
		draw_flag_1 = true;
		HAL_Delay(100);
	}

	if (draw_flag_1)
	{
		draw_func(*index);
		draw_flag_1 = false;
	}
}

void navigate_menu_right_left(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t))
{
	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_LEFT && *index != 0)
	{
		(*index)--;
		draw_flag_2 = true;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_RIGHT && *index < max_items - 1)
	{
		(*index)++;
		draw_flag_2 = true;
		HAL_Delay(100);
	}

	if (draw_flag_2)
	{
		draw_func(*index);
		draw_flag_2 = false;
	}
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
		draw_flag_3 = true;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_DOWN)
	{
		*index = (*index + 1) % max_items;
		draw_flag_3 = true;
		HAL_Delay(100);
	}

	if (draw_flag_3)
	{
		draw_func(*index);
		draw_flag_3 = false;
	}

}

void navigate_menu_right_left_loop(uint8_t *index, uint8_t max_items, void (*draw_func)(uint8_t))
{
	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_LEFT)
	{
		*index = (*index + max_items - 1) % max_items;
		draw_flag_4 = true;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_RIGHT)
	{
		*index = (*index + 1) % max_items;
		draw_flag_4 = true;
		HAL_Delay(100);
	}

	if (draw_flag_4)
	{
		draw_func(*index);
		draw_flag_4 = false;
	}
}


/*
 * these flags required to optimize display usage,
 * otherwise when these navigation functions called they call the draw functions non-stop
 */
void reset_navigate_menu_draw_flags(void)
{
	draw_flag_1 = true;
	draw_flag_2 = true;
	draw_flag_3 = true;
	draw_flag_4 = true;
}
