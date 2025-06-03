/*
 * menu_paused.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_paused.h"

// ===== Includes ===== //
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "menu_interface.h"
#include "menu_logic.h"
#include "joystick.h"
#include "game_runtime.h"

// include other
#include <stdint.h>
#include "stm32f0xx_hal.h"

// ======= Macros/Constants ===== //
// ----- //

// ===== Static File-Private Variables ===== //
// ----- menu_paused ----- //
typedef enum
{
	PAUSED_CONTINUE = 0,
	PAUSED_EXIT,
	PAUSED_COUNT
} paused_action_t;

static const char *menu_paused_items[PAUSED_COUNT] =
{
		"Continue",
		"Exit"
};

static paused_action_t current_menu_paused_action = PAUSED_CONTINUE;

// ===== Public Global Variables ===== //
// ----- //

// ===== Static Function Declarations ===== //
static void handle_menu_paused(void);

// ===== Public API Function Definitions ===== //
void game_pause(void)
{
	uint32_t start_paused_time = HAL_GetTick();			// start measuring how much time spent in paused state

	input_enabled = false;
	HAL_Delay(500);
	input_enabled = true;

	current_menu_state = MENU_PAUSED;

	while (current_menu_state == MENU_PAUSED)
	{
		handle_menu_paused();
	}

	paused_time += HAL_GetTick() - start_paused_time;	// get the result
}

// ===== Static Function Definitions ===== //
// Draw Functions

static void draw_menu_paused(uint8_t current_menu_paused_action)
{
	display_clear(); // Clear the OLED display

	display_write_horizontal_string("PAUSED", 5, display_font_16x24, display_color_white);
	display_draw_horizontal_menu(menu_paused_items, 50, &current_menu_paused_action, PAUSED_COUNT, display_font_7x10, display_color_white);

	display_update(); // Refresh screen
}

static void navigate_menu_paused(void)
{
	navigate_menu_right_left(&current_menu_paused_action, PAUSED_COUNT, draw_menu_paused);
}


// Logic Handlers

static void handle_current_menu_paused_action(void)
{
	switch (current_menu_paused_action)
	{
	case PAUSED_CONTINUE:
	{
		current_menu_state = MENU_PLAYING;
		break;
	}
	case PAUSED_EXIT:
	{
		current_menu_state = MENU_GAMEOVER;
		current_menu_paused_action = PAUSED_CONTINUE;
		break;
	}
	default: break;
	}
}

static void handle_menu_paused(void)
{
	navigate_menu_paused();
	if (is_joystick_pressed() || is_button_pressed()) handle_current_menu_paused_action();
}
