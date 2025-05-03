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

static void menu_selected_draw(uint8_t current_menu_selected_action)
{
	ssd1306_Fill(Black); // Clear the OLED display

	// Display action list
	char buffer[32];
	snprintf(buffer, sizeof(buffer), ">>> %s <<<", menu_main_items[current_menu_main_item]);
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString(buffer, Font_7x10, White);

    for (uint8_t i = 0; i < ACTION_COUNT; i++)
    {
        if (i == current_menu_selected_action)
        { // Highlight selected item
            ssd1306_SetCursor(0, (i * 10) + 15);
            ssd1306_WriteString("->", Font_6x8, White);
            ssd1306_SetCursor(20, (i * 10) + 15);
        }
        else
        {
        	ssd1306_SetCursor(30, (i * 10) + 15);
        }

        ssd1306_WriteString(menu_selected_items[i], Font_6x8, White);
    }

    ssd1306_UpdateScreen(); // Refresh OLED screen
}

static void navigate_menu_selected(void)
{
	navigate_menu_up_down(&current_menu_selected_action, ACTION_COUNT, menu_selected_draw);
}

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
