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

#include "oled_utils.h"
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

bool animation_shown = false; // to draw game over screen animated, resets every start up of a game in "game_start.c"



// gameover screen draw functions

static void draw_animated_menu_gameover(void)
{
	ssd1306_Fill(Black);

	ssd1306_SetCursor(9, 23);
	ssd1306_WriteString("GAME OVER!", Font_11x18, White);
	ssd1306_UpdateScreen();
	HAL_Delay(2000);

	ssd1306_Fill(Black);
	oled_draw_horizontal_string(">---YOUR STATS---<", Font_7x10, 0, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);

	ssd1306_SetCursor(0, 25);
	ssd1306_WriteString("SCORE : ", Font_6x8, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);
	char score[8];
	snprintf(score, sizeof(score), "%d", game_get_score());
	ssd1306_SetCursor(42, 25);
	ssd1306_WriteString(score, Font_6x8, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);
}

static void draw_menu_gameover(uint8_t current_menu_gameover_action)
{
	ssd1306_Fill(Black);

	oled_draw_horizontal_string(">---YOUR STATS---<", Font_7x10, 0, White);

	ssd1306_SetCursor(0, 25);
	ssd1306_WriteString("SCORE : ", Font_6x8, White);
	char score[8];
	snprintf(score, sizeof(score), "%d", game_get_score());
	ssd1306_SetCursor(42, 25);
	ssd1306_WriteString(score, Font_6x8, White);

	oled_draw_vertical_menu(menu_gameover_items, Font_6x8, 35, White, &current_menu_gameover_action, GAMEOVER_COUNT);

	ssd1306_UpdateScreen(); // Refresh OLED screen
}

static void navigate_menu_gameover(void)
{
	navigate_menu_up_down(&current_menu_gameover_action, GAMEOVER_COUNT, draw_menu_gameover);
}


// Logic handlers of gameover menu

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
