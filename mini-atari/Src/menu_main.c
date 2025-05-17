/*
 * menu_main.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_main.h"

// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "joystick.h"
#include "menu_logic.h"

// include other
#include <stdint.h>

// includes end <<-----

const char *menu_main_items[GAME_COUNT] =
{
		"Snake Game",
		"Tetris Game",
		"Game3",
		"Game4",
		"Game5"
};

game_type_t current_game_type = GAME_SNAKE;

static void draw_menu_main(uint8_t current_game_type)
{
	oled_clear(); // Clear the OLED display

    // Display game list
	oled_write_horizontal_string(">>> GAME MENU <<<", oled_font_7x10, 0, oled_color_white);

	oled_draw_vertical_menu(menu_main_items, oled_font_6x8, 15, oled_color_white, &current_game_type, GAME_COUNT);

    oled_update();  // Refresh OLED screen
}

static void navigate_menu_main(void)
{
	navigate_menu_up_down(&current_game_type, GAME_COUNT, draw_menu_main);
}

void handle_menu_main(void)
{
	navigate_menu_main();
	if (joystick_is_pressed()) current_menu_state = MENU_SELECTED;
}
