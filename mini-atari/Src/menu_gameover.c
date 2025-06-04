/*
 * menu_gameover.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_gameover.h"

// ===== Includes ===== //
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "menu_interface.h"
#include "menu_logic.h"
#include "game_runtime.h"
#include "joystick.h"
#include "menu_selected.h"

// include other
#include "stm32f0xx_hal.h"
#include <stdio.h>
#include <stdbool.h>

// ======= Macros/Constants ===== //
// ----- //

// ===== Static File-Private Variables ===== //
static const char *menu_gameover_items[GAMEOVER_COUNT] =
{
		"Play Again",
		"Main Menu",
		"Save Score"
};

static gameover_action_t current_gameover_action = GAMEOVER_PLAY_AGAIN;

// ===== Public Global Variables ===== //
bool stats_animation_shown = false; // to draw game over screen animated, resets every start up of a game in "game_runtime.c"

// ===== Static Function Declarations ===== //
static void draw_animated_stats(void);
static void navigate_menu_gameover(void);
static void handle_current_gameover_action(void);

// ===== Public API Function Definitions ===== //
void handle_menu_gameover(void)
{
	if (!stats_animation_shown)
	{
		game_get_time_spent();

		input_enabled = false;
		HAL_Delay(500);
		game_end();
		draw_animated_stats();
		stats_animation_shown = true;
		input_enabled = true;
	}

	navigate_menu_gameover();
	if (is_joystick_pressed() || is_button_pressed())
	{
		reset_navigate_menu_draw_flags();
		handle_current_gameover_action();
	}
}

// ===== Static Function Definitions ===== //
// Draw Functions

static void draw_animated_stats(void)
{
	char buffer[32];
	uint8_t y = 0;
	display_clear();

	display_write_horizontal_string(">---YOUR STATS---<",  y, display_font_7x10, display_color_white);
	display_update();
	HAL_Delay(1000);


	y += (font_height(display_font_7x10) * 2);
	buffer[0] = '\0';
	snprintf(buffer, sizeof(buffer), "Score:%d Time:%ds", game_get_score(), game_get_time_spent());
	display_write_horizontal_string(buffer, y, display_font_6x8, display_color_white);
	display_update();
	HAL_Delay(1000);
}

static void draw_stats(uint8_t current_gameover_action)
{
	display_clear();
	uint8_t y = 0;

	display_write_horizontal_string(">---YOUR STATS---<",  y, display_font_7x10, display_color_white);

	y += (font_height(display_font_7x10) * 2);
	char buffer[48];
	snprintf(buffer, sizeof(buffer), "Score:%d Time:%ds", game_get_score(), game_get_time_spent());
	display_write_horizontal_string(buffer, y, display_font_6x8, display_color_white);

	y += (font_height(display_font_6x8) * 2);
	display_draw_vertical_menu(menu_gameover_items,  y, &current_gameover_action, GAMEOVER_COUNT, display_font_6x8, display_color_white);

	display_update();
}

static void navigate_menu_gameover(void)
{
	navigate_menu_up_down(&current_gameover_action, GAMEOVER_COUNT, draw_stats);
}


// Logic Handlers

static void handle_current_gameover_action(void)
{
	switch (current_gameover_action)
	{
		case GAMEOVER_PLAY_AGAIN:
		{
			current_menu_state = MENU_PLAYING;
			break;
		}
		case GAMEOVER_MAIN_MENU:
		{
			current_menu_state = MENU_MAIN;
			current_game_type = GAME_SNAKE;
			current_gameover_action = GAMEOVER_PLAY_AGAIN;
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
