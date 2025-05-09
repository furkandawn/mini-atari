/*
 * menu_selected.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_selected.h"

// includes start
// include ssd1306 OLED Display
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include "oled_utils.h"
#include "joystick.h"
#include "menu_main.h"
#include "menu_logic.h"
#include "game_start.h"

#include <stdio.h>
#include <stdint.h>

// includes end

static const char *menu_selected_items[ACTION_COUNT] =
{
		"Start Game",
		"Leaderboard",
		"Main Menu"
};

menu_selected_action_t current_menu_selected_action = ACTION_START;


// Draw functions of selected menu

static void menu_selected_draw(uint8_t current_menu_selected_action)
{
	ssd1306_Fill(Black); // Clear the OLED display

	// Display game name
	char buffer[32];
	snprintf(buffer, sizeof(buffer), ">_> %s <_<", menu_main_items[current_menu_main_item]);
	oled_draw_horizontal_string(buffer, Font_7x10, 0, White);

	// Display actions
	oled_draw_vertical_menu(menu_selected_items, Font_7x10, 20, White, &current_menu_selected_action, ACTION_COUNT);

    ssd1306_UpdateScreen(); // Refresh OLED screen
}

static void navigate_menu_selected(void)
{
	navigate_menu_up_down(&current_menu_selected_action, ACTION_COUNT, menu_selected_draw);
}


// Logic handlers of selected menu

static void handle_current_menu_selected_action(void)
{
	switch (current_menu_selected_action)
	{
	case ACTION_START:
	{
		if(current_menu_main_item == GAME_SNAKE) {current_menu_state = MENU_PLAYING;
		game_init();}
		break;
	}
	case ACTION_LEADERBOARD:
	{
		//current_menu_state = MENU_LEADERBOARD;
		// re-write this after leaderboard done.
		break;
	}
	case ACTION_BACK:
	{
		current_menu_state = MENU_MAIN;
		current_menu_selected_action = ACTION_START;
		current_menu_main_item = GAME_SNAKE;
		break;
	}
	default:
	{
		break;
	}
	}
}

void handle_menu_selected(void)
{
	navigate_menu_selected();
	if (joystick_is_pressed()) handle_current_menu_selected_action();
}
