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
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString(">>> GAME MENU <<<", Font_7x10, White);

    for (uint8_t i = 0; i < GAME_COUNT; i++)
    {
        if (i == current_game_type)
        { // Highlight selected game
            ssd1306_SetCursor(0, (i * 10) + 15);
            ssd1306_WriteString("->", Font_6x8, White);
            ssd1306_SetCursor(20, (i * 10) + 15);
        }
        else
        {
            ssd1306_SetCursor(30, (i * 10) + 15);
        }

        ssd1306_WriteString(menu_main_items[i], Font_6x8, White);
    }

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
