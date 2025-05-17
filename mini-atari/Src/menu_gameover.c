/*
 * menu_gameover.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_gameover.h"

// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "game_init.h"
#include "game_score.h"
#include "oled_utils.h"
#include "joystick.h"
#include "menu_logic.h"
#include "menu_main.h"
#include "menu_selected.h"

// include other
#include <stdio.h>
#include <stdbool.h>

// includes end <<-----

static const char *menu_gameover_items[GAMEOVER_COUNT] =
{
		"Play Again",
		"Main Menu",
		"Save Score"
};

gameover_action_t current_menu_gameover_action = GAMEOVER_PLAY_AGAIN;

bool animation_shown = false; // to draw game over screen animated, resets every start up of a game in "game_start.c"



// gameover screen draw functions

static void draw_animated_menu_gameover(void)
{
	oled_clear();

	oled_write_centered_string("GAME OVER !", oled_font_11x18, oled_color_white);
	oled_update();
	HAL_Delay(2000);

	oled_clear();

	oled_write_horizontal_string(">---YOUR STATS---<", oled_font_7x10, 0, oled_color_white);
	oled_update();
	HAL_Delay(1000);

	char buffer[48];
	snprintf(buffer, sizeof(buffer), "Score:%d Time:%ds", game_get_score(), game_get_time_spent());
	oled_write_horizontal_string(buffer, oled_font_6x8, 20, oled_color_white);
	oled_update();
	HAL_Delay(1000);
}

static void draw_menu_gameover(uint8_t current_menu_gameover_action)
{
	oled_clear();

	oled_write_horizontal_string(">---YOUR STATS---<", oled_font_7x10, 0, oled_color_white);

	char buffer[48];
	snprintf(buffer, sizeof(buffer), "Score:%d Time:%ds", game_get_score(), game_get_time_spent());
	oled_write_horizontal_string(buffer, oled_font_6x8, 20, oled_color_white);

	oled_draw_vertical_menu(menu_gameover_items, oled_font_6x8, 35, oled_color_white, &current_menu_gameover_action, GAMEOVER_COUNT);

	oled_update();
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
			current_game_type = GAME_SNAKE;
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
