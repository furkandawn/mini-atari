/*
 * menu_main.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_main.h"

// === Includes Start ===

// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "joystick.h"
#include "menu_logic.h"

// include other
#include <stdint.h>

// === Includes End ===

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
	display_clear(); // Clear the OLED display

    // Display game list
	display_write_horizontal_string(">>> GAME MENU <<<", 0, display_font_7x10, display_color_white);

	display_draw_vertical_menu(menu_main_items, 15, &current_game_type, GAME_COUNT, display_font_6x8, display_color_white);

    display_update();  // Refresh OLED screen
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
