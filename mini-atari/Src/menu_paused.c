/*
 * menu_paused.c
 *
 *  Created on: May 2, 2025
 *      Author: furkandawn
 */


#include "menu_paused.h"

// includes start
// include ssd1306 OLED Display
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include "joystick.h"
#include "menu_logic.h"
#include <stdint.h>

// includes end

static const char *menu_paused_items[PAUSED_COUNT] =
{
		"Continue",
		"Exit"
};

static menu_paused_action_t current_menu_paused_action = PAUSED_CONTINUE;

static void draw_menu_paused(uint8_t current_menu_paused_action)
{
	ssd1306_Fill(Black); // Clear the OLED display

	ssd1306_SetCursor(16, 5);
	ssd1306_WriteString("PAUSED", Font_16x24, White);

	for (uint8_t i = 0; i < PAUSED_COUNT; i++)
	{
		if (i == current_menu_paused_action)
		{ // Highlight "Continue - Exit"
			ssd1306_SetCursor(33 + (i * 68), 30);
			ssd1306_WriteString("|", Font_6x8, White);
			ssd1306_SetCursor(33 + (i * 68), 38);
			ssd1306_WriteString("V", Font_6x8, White);
		}
		ssd1306_SetCursor(10 + ( i * 80), 50);
		ssd1306_WriteString(menu_paused_items[i], Font_6x8, White);
	}

	ssd1306_UpdateScreen(); // Refresh screen
}

static void navigate_menu_paused(void)
{
	navigate_menu_right_left(&current_menu_paused_action, PAUSED_COUNT, draw_menu_paused);
}

static void handle_current_menu_paused_action(void)
{
	switch (current_menu_paused_action)
	{
	case PAUSED_CONTINUE:
	{
		current_menu_state = MENU_PLAYING;
		break;
	}
	case PAUSED_EXIT:
	{
		current_menu_state = MENU_GAMEOVER;
		current_menu_paused_action = PAUSED_CONTINUE;
		break;
	}
	default: break;
	}
}

static void handle_menu_paused(void)
{
	navigate_menu_paused();
	if (joystick_is_pressed()) handle_current_menu_paused_action();
}

void game_pause(void)
{
	HAL_Delay(500);
	current_menu_state = MENU_PAUSED;
	while (current_menu_state == MENU_PAUSED)
	{
		handle_menu_paused();
	}
}
