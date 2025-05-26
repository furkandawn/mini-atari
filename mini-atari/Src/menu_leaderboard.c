/*
 * menu_leaderboard.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_leaderboard.h"

// ===== Includes ===== //
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "menu_interface.h"
#include "menu_logic.h"
#include "joystick.h"
#include "game_leaderboard.h"

// include other
#include <stdio.h>
#include <stdbool.h>

// ======= Macros/Constants ===== //
// ----- //

// ===== Static File-Private Variables ===== //
static uint8_t current_leaderboard_entry_index = 0;

// ===== Public Global Variables ===== //
// ----- //

// ===== Static Function Declarations ===== //
static void navigate_menu_leaderboard(void);
static void handle_menu_leaderboard_action(void);

// ===== Public API Function Definitions ===== //
void handle_menu_leaderboard(void)
{
	navigate_menu_leaderboard();

	if (joystick_is_pressed() || button_is_pressed())
	{
		handle_menu_leaderboard_action();
	}
}

// ===== Static Function Definitions ===== //
// Draw Functions

static void draw_menu_leaderboard(uint8_t current_leaderboard_entry_index)
{
	display_clear();

	display_write_horizontal_string("<< LEADERBOARD >>", 0, display_font_7x10, display_color_white);
	display_draw_vertical_menu(menu_leaderboard_entries[current_game_type], 15, &current_leaderboard_entry_index, LEADERBOARD_TOTAL_COUNT, display_font_7x10, display_color_white);

	display_update();
}

static void draw_menu_leaderboard_selected(uint8_t current_leaderboard_entry_index)
{
	display_clear();

	const leaderboard_entry_t *entry = &current_leaderboard[current_game_type][current_leaderboard_entry_index];
	char buffer[32];

	snprintf(buffer, sizeof(buffer), "%d. %s", current_leaderboard_entry_index + 1, menu_leaderboard_entries[current_game_type][current_leaderboard_entry_index]);
	display_write_horizontal_string(buffer, 0, display_font_7x10, display_color_white);

	snprintf(buffer, sizeof(buffer), "SCORE : %d", entry->score);
	display_write_horizontal_string(buffer, DISPLAY_HEIGHT / 2 - font_height(display_font_7x10) - 2, display_font_7x10, display_color_white);

	snprintf(buffer, sizeof(buffer), "TIME : %d s", entry->time_passed);
	display_write_horizontal_string(buffer, DISPLAY_HEIGHT / 2, display_font_7x10, display_color_white);

	display_write_horizontal_string(">> press to exit <<", DISPLAY_HEIGHT - font_height(display_font_6x8), display_font_6x8, display_color_white);

	display_update();
}

static void navigate_menu_leaderboard(void)
{
	navigate_menu_up_down(&current_leaderboard_entry_index, LEADERBOARD_TOTAL_COUNT, draw_menu_leaderboard);
}

// Logic Handlers

static void handle_menu_leaderboard_action(void)
{
	if (current_leaderboard_entry_index == LEADERBOARD_TOTAL_COUNT - 1) // last index is EXIT
	{
		current_leaderboard_entry_index = 0;
		current_menu_state = MENU_SELECTED;
	}
	else
	{
		bool leaderboard_entry_focused = true;
		bool drawn_once = false;

		while (leaderboard_entry_focused)
		{
			if (!drawn_once)
			{
				draw_menu_leaderboard_selected(current_leaderboard_entry_index);
				drawn_once = true;
			}

			if (joystick_is_pressed() || button_is_pressed())
			{
				leaderboard_entry_focused = false;
				current_menu_state = MENU_LEADERBOARD;
			}
		}
	}
}
