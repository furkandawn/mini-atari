/*
 * menu_gameover.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_gameover.h"

// includes start
// include ssd1306 OLED Display
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include "joystick.h"
#include "menu_logic.h"
#include "menu_main.h"
#include "menu_selected.h"
#include "game_start.h"
#include "game_level.h"

#include <stdio.h>
#include <stdbool.h>

// includes end

static const char *menu_gameover_items[GAMEOVER_COUNT] =
{
		"Play Again",
		"Main Menu",
		"Save Score"
};

game_over_action_t current_menu_gameover_action = GAMEOVER_PLAY_AGAIN;
bool animation_shown = false;

static void draw_animated_menu_gameover(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(9, 23);
	ssd1306_WriteString("GAME OVER!", Font_11x18, White);
	ssd1306_UpdateScreen();
	HAL_Delay(2000);

	ssd1306_Fill(Black);
	ssd1306_SetCursor(8, 5);
	ssd1306_WriteString(">> YOUR STATS <<", Font_7x10, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("SCORE : ", Font_6x8, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%d", game_get_score());
	ssd1306_SetCursor(48, 20);
	ssd1306_WriteString(buffer, Font_6x8, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1500);
}

static void draw_menu_gameover(uint8_t current_menu_gameover_action)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(8, 5);
	ssd1306_WriteString(">> YOUR STATS <<", Font_7x10, White);

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("SCORE : ", Font_6x8, White);
	char buffer[8];
	snprintf(buffer, sizeof(buffer), "%d", game_get_score());
	ssd1306_SetCursor(48, 20);
	ssd1306_WriteString(buffer, Font_6x8, White);

	for (uint8_t i = 0; i < GAMEOVER_COUNT; i++)
	{
		if (i == current_menu_gameover_action)
		{ // Highlight selected item
			ssd1306_SetCursor(15, (i * 10) + 35);
			ssd1306_WriteString("->", Font_6x8, White);
			ssd1306_SetCursor(30, (i * 10) + 35);
		}
		else
		{
			ssd1306_SetCursor(40, (i * 10) + 35);
		}

		ssd1306_WriteString(menu_gameover_items[i], Font_6x8, White);
	}

	ssd1306_UpdateScreen(); // Refresh OLED screen
}

static void navigate_menu_gameover(void)
{
	navigate_menu_up_down(&current_menu_gameover_action, GAMEOVER_COUNT, draw_menu_gameover);
}

/*
 *  Up Above contains print and navigation of game over screen
 *  -----------------------------------------------------------------------------
 *  Down Below is the logic of game over screen
 */

static void handle_current_menu_gameover_action(void)
{
	switch (current_menu_gameover_action)
	{
		case GAMEOVER_PLAY_AGAIN:
		{
			current_menu_state = MENU_PLAYING;
			game_init();
			break;
		}
		case GAMEOVER_MAIN_MENU:
		{
			current_menu_state = MENU_MAIN;
			current_menu_main_item = GAME_SNAKE;
			current_menu_selected_action = ACTION_START;
			current_menu_gameover_action = GAMEOVER_PLAY_AGAIN;
			break;
		}
		case GAMEOVER_SAVE:
		{
			current_menu_state = MENU_SAVE;
			break;
		}
		default:
		{
		break;
		}
	}
}

void handle_menu_gameover(void)
{
	if (!animation_shown)
	{
		draw_animated_menu_gameover();
		animation_shown = true;
	}
	navigate_menu_gameover();
	if(joystick_is_pressed()) handle_current_menu_gameover_action();
}
