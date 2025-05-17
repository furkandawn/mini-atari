/*
 * menu_leaderboard.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_leaderboard.h"

// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "menu_main.h"
#include "joystick.h"
#include "menu_logic.h"
#include "game_leaderboard.h"

// include other
#include <stdio.h>
#include <stdbool.h>

// includes end <<-----

static uint8_t current_leaderboard_entry_index = 0;

static void draw_menu_leaderboard(uint8_t current_leaderboard_entry_index)
{
	oled_clear();

	oled_write_horizontal_string("<< LEADERBOARD >>", oled_font_7x10, 0, oled_color_white);
	oled_draw_vertical_menu(menu_leaderboard_entries[current_game_type], oled_font_7x10, 15, oled_color_white, &current_leaderboard_entry_index, LEADERBOARD_TOTAL_COUNT);

	oled_update();
}

static void draw_menu_leaderboard_selected(uint8_t current_leaderboard_entry_index)
{
	oled_clear();

	const leaderboard_entry_t *entry = &leaderboard[current_game_type][current_leaderboard_entry_index];
	char buffer[32];

	snprintf(buffer, sizeof(buffer), "%d. %s", current_leaderboard_entry_index + 1, menu_leaderboard_entries[current_game_type][current_leaderboard_entry_index]);
	oled_write_horizontal_string(buffer, oled_font_7x10, 0, oled_color_white);

	snprintf(buffer, sizeof(buffer), "SCORE : %d", entry->score);
	oled_write_horizontal_string(buffer, oled_font_7x10, 20, oled_color_white);

	snprintf(buffer, sizeof(buffer), "TIME : %d s", entry->time_passed);
	oled_write_horizontal_string(buffer, oled_font_7x10, 35, oled_color_white);

	oled_write_horizontal_string(">> press to exit <<", oled_font_6x8, 54, oled_color_white);

	oled_update();
}

static void navigate_menu_leaderboard(void)
{
	navigate_menu_up_down(&current_leaderboard_entry_index, LEADERBOARD_TOTAL_COUNT, draw_menu_leaderboard);
}


// LOGIC HANDLERS

void handle_menu_leaderboard(void)
{
	navigate_menu_leaderboard();

	if (joystick_is_pressed())
	{
		if (current_leaderboard_entry_index == LEADERBOARD_TOTAL_COUNT - 1)
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

				if (joystick_is_pressed())
				{
					leaderboard_entry_focused = false;
					current_menu_state = MENU_LEADERBOARD;
				}
			}
		}
	}
}
