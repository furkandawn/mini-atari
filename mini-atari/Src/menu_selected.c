/*
 * menu_selected.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_selected.h"

// ===== Includes ===== //
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "joystick.h"
#include "menu_interface.h"
#include "menu_logic.h"

// include other
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

// ======= Macros/Constants ===== //
// ----- //

// ===== Static File-Private Variables ===== //
static const char *menu_selected_items[MENU_ACTION_COUNT] =
{
		"Start Game",
		"Leaderboard",
		"Main Menu"
};

static menu_action_t current_menu_action = MENU_ACTION_START;

// ===== Public Global Variables ===== //
// ----- //

// ===== Static Function Declarations ===== //
static void to_upper_string(char *str);
static void navigate_menu_selected(void);
static void handle_current_menu_action(void);

// ===== Public API Function Definitions ===== //
void handle_menu_selected(void)
{
	navigate_menu_selected();
	if (joystick_is_pressed() || button_is_pressed()) handle_current_menu_action();
}

// ===== Static Function Definitions ===== //
// Draw Functions

static void menu_selected_draw(uint8_t current_menu_action)
{
	display_clear();

	// Get current game name
	char buffer[32];
	snprintf(buffer, sizeof(buffer), ">_ %s _<", menu_main_items[current_game_type]);
	to_upper_string(buffer);

	display_write_horizontal_string(buffer, 0, display_font_7x10, display_color_white);
	display_draw_vertical_menu(menu_selected_items, (font_height(display_font_7x10) * 2), &current_menu_action, MENU_ACTION_COUNT, display_font_7x10, display_color_white);

    display_update();
}

static void navigate_menu_selected(void)
{
	navigate_menu_up_down(&current_menu_action, MENU_ACTION_COUNT, menu_selected_draw);
}


// Logic Handlers

static void handle_current_menu_action(void)
{
	switch (current_menu_action)
	{
	case MENU_ACTION_START:
	{
		current_menu_state = MENU_PLAYING;
		break;
	}
	case MENU_ACTION_LEADERBOARD:
	{
		current_menu_state = MENU_LEADERBOARD;
		break;
	}
	case MENU_ACTION_BACK:
	{
		current_menu_state = MENU_MAIN;
		current_menu_action = MENU_ACTION_START;
		break;
	}
	default:
	{
		break;
	}
	}
}

// to make game name upper case
static void to_upper_string(char *str)
{
	while (*str)
	{
		*str = toupper((unsigned char)*str);
		str++;
	}
}
