/*
 * menu_main.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_main.h"

// === Includes === //
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "joystick.h"
#include "menu_logic.h"
#include "menu_interface.h"

// ===== Static File-Private Variables ===== //
// ----- //

// ===== Public Global Variables ===== //
const char *menu_main_items[GAME_COUNT] =
{
		"Snake",
		"Tetris",
		"Pong",
		"Breakout",
		"Spc Inv"
};

game_type_t current_game_type = GAME_SNAKE;

// ===== Static Function Declarations ===== //
static void navigate_menu_main(void);

// ===== Public API Function Definitions ===== //
void handle_menu_main(void)
{
	navigate_menu_main();
	if (is_joystick_pressed() || is_button_pressed()) current_menu_state = MENU_SELECTED;
}

// ===== Static Function Definitions ===== //
// Draw Functions
static void draw_menu_main(uint8_t current_game_type)
{
	display_clear();

	display_write_horizontal_string(">>> GAME MENU <<<", 0, display_font_7x10, display_color_white);
	display_draw_vertical_menu(menu_main_items, 15, &current_game_type, GAME_COUNT, display_font_6x8, display_color_white);

    display_update();
}

static void navigate_menu_main(void)
{
	navigate_menu_up_down_loop(&current_game_type, GAME_COUNT, draw_menu_main);
}
