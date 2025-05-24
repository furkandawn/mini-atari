/*
 * menu_paused.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_paused.h"

// === Includes Start ===

// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "joystick.h"
#include "menu_logic.h"

// include other
#include <stdint.h>
#include "stm32f0xx_hal.h"

// === Includes End ===

static const char *menu_paused_items[PAUSED_COUNT] =
{
		"Continue",
		"Exit"
};

static menu_paused_action_t current_menu_paused_action = PAUSED_CONTINUE;


// Draw functions of Pause Menu

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


// Logic handlers of pause menu

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
	if (joystick_is_pressed()) handle_current_menu_paused_action();
}

void game_pause(void)
{
	HAL_Delay(500);
	current_menu_state = MENU_PAUSED;
	while (current_menu_state == MENU_PAUSED)
	{
		handle_menu_paused();
	}
}
