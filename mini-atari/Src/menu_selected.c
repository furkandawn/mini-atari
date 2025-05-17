/*
 * menu_selected.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_selected.h"

// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "game_init.h"
#include "joystick.h"
#include "menu_main.h"
#include "menu_logic.h"

// include other
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

// includes end <<-----

static void to_upper_string(char *str);

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
	oled_clear(); // Clears the OLED display

	// Display game name
	char buffer[32];
	snprintf(buffer, sizeof(buffer), ">_ %s _<", menu_main_items[current_game_type]);
	to_upper_string(buffer);
	oled_write_horizontal_string(buffer, oled_font_7x10, 0, oled_color_white);

	// Display actions
	oled_draw_vertical_menu(menu_selected_items, oled_font_7x10, 20, oled_color_white, &current_menu_selected_action, ACTION_COUNT);

    oled_update(); // Refreshes the OLED screen
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
		if(current_game_type == GAME_SNAKE) {current_menu_state = MENU_PLAYING;
		game_init();}
		break;
	}
	case ACTION_LEADERBOARD:
	{
		current_menu_state = MENU_LEADERBOARD;
		break;
	}
	case ACTION_BACK:
	{
		current_menu_state = MENU_MAIN;
		current_menu_selected_action = ACTION_START;
		current_game_type = GAME_SNAKE;
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


// to make game name(s) uppercase
static void to_upper_string(char *str)
{
	while (*str)
	{
		*str = toupper((unsigned char)*str);
		str++;
	}
}
