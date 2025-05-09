/*
 * menu_main.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_main.h"

// includes start
// include ssd1306 OLED Display
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include "oled_utils.h"
#include "joystick.h"
#include "menu_logic.h"
#include <stdint.h>

// includes end

const char *menu_main_items[GAME_COUNT] =
{
		"Snake Game",
		"Game2",
		"Game3",
		"Game4",
		"Game5"
};

menu_main_item_t current_menu_main_item = GAME_SNAKE;

static void draw_menu_main(uint8_t current_game_type)
{
	ssd1306_Fill(Black); // Clear the OLED display

    // Display game list
	oled_draw_horizontal_string(">>> GAME MENU <<<", Font_7x10, 0, White);

	oled_draw_vertical_menu(menu_main_items, Font_6x8, 15, White, &current_game_type, GAME_COUNT);

    ssd1306_UpdateScreen();  // Refresh OLED screen
}

static void navigate_menu_main(void)
{
	navigate_menu_up_down(&current_menu_main_item, GAME_COUNT, draw_menu_main);
}

void handle_menu_main(void)
{
	navigate_menu_main();
	if (joystick_is_pressed()) current_menu_state = MENU_SELECTED;
}
