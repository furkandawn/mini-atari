/*
 * game_ui.c
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */


#include "game_ui.h"

// ===== Includes ===== //
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "game_runtime.h"
#include "menu_gameover.h"
#include "game_bitmap.h"

// include other
#include <stdio.h>
#include <string.h>
#include "stm32f0xx_hal.h"

// ======= Macros/Constants ===== //
#define COUNTDOWN 3
#define LIFE_SYMBOL "<3 "

// ===== Public API Function Definitions ===== //
void game_draw_start_screen(void)
{
	input_enabled = false;

	// Draws bitmap
	display_clear();
	display_draw_bitmap(0, 0, game_startup_bitmap[game_runtime.game_type], DISPLAY_WIDTH, DISPLAY_HEIGHT, display_color_white);
	display_update();
	HAL_Delay(2500);

	// Draws countdown
	display_clear();
	for (uint8_t i = COUNTDOWN; i > 0; i--)
	{
		display_clear();

		char buffer[4];
		sprintf(buffer, "%d", i);

		display_write_centered_string(buffer, display_font_16x24, display_color_white);

		display_update();
		HAL_Delay(500);
	}

	input_enabled = true;
}

void game_draw_level_up_animation(void)
{
	input_enabled = false;

	char buffer[16];
	snprintf(buffer, sizeof(buffer), "LEVEL %d", game_runtime.level);
	display_write_centered_string(buffer, display_font_16x24, display_color_white);

	display_update();

	HAL_Delay(1000);

	input_enabled = true;
}

// Show remaining lives animation (full -> one less) when player loses. If no lives remaining, return and set game_over true.
void game_draw_remaining_lives(void)
{
	input_enabled = false;

	char buffer[32];
	buffer[0] = '\0';

	for (uint8_t i = 0; i < game_runtime.life; i++)
	{
		strcat(buffer, LIFE_SYMBOL);
	}

	display_clear();
	display_write_centered_string(buffer, display_font_11x18, display_color_white);
	display_update();

	HAL_Delay(1000);

	buffer[0] = '\0';

	for (uint8_t i = 0; i < game_runtime.life - 1; i++)
	{
		strcat(buffer, LIFE_SYMBOL);
	}

	display_clear();
	display_write_centered_string(buffer, display_font_11x18, display_color_white);
	display_update();

	HAL_Delay(1000);

	input_enabled = true;
}
