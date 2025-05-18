/*
 * game_ui.c
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */


#include "game_ui.h"

// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "game_runtime.h"
#include "menu_gameover.h"
#include "game_bitmap.h"

// include other
#include <stdio.h>

// includes end <<-----

#define COUNTDOWN 3

void game_draw_start_screen(void)
{
	// Draws bitmap
	oled_clear();
	oled_draw_bitmap(0, 0, game_bitmap[game_runtime.game_type], BITMAP_WIDTH, BITMAP_HEIGHT, oled_color_white);
	oled_update();
	HAL_Delay(2500);

	// Draws countdown
	oled_clear();
	for (uint8_t i = COUNTDOWN; i > 0; i--)
	{
		oled_clear();

		char buffer[4];
		sprintf(buffer, "%d", i);

		oled_write_centered_string(buffer, oled_font_16x26, oled_color_white);

		oled_update();
		HAL_Delay(500);
	}

	animation_shown = false;  // resets gameover animated screen flag
}

void game_draw_level_screen(void)
{
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "LEVEL %d", game_runtime.level);
	oled_write_centered_string(buffer, oled_font_16x26, oled_color_white);

	oled_update();

	HAL_Delay(1000);
}

