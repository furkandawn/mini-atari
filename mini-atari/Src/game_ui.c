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
#include "stm32f0xx_hal.h"

// ======= Macros/Constants ===== //
#define COUNTDOWN 3

// ===== Public API Function Definitions ===== //
void game_draw_start_screen(void)
{
	// Draws bitmap
	display_clear();
	display_draw_bitmap(0, 0, game_bitmap[game_runtime.game_type], DISPLAY_WIDTH, DISPLAY_HEIGHT, display_color_white);
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
}

void game_draw_level_up_animation(void)
{
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "LEVEL %d", game_runtime.level);
	display_write_centered_string(buffer, display_font_16x24, display_color_white);

	display_update();

	HAL_Delay(1000);
}

