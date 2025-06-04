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
#define EXCLAMATION_SYMBOL "!"

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

void game_draw_remaining_lives(void)
{
	input_enabled = false;

	display_clear();

	uint8_t gap_between_hearts = 6;
	uint8_t start_x = ((DISPLAY_WIDTH - (game_runtime.life * BITMAP_HEART_WIDTH) - ((game_runtime.life - 1) * gap_between_hearts)) / 2);
	uint8_t y = ((DISPLAY_HEIGHT - BITMAP_HEART_HEIGHT) / 2);

	uint8_t x = start_x;
	for (uint8_t i = 0; i < game_runtime.life; i++)
	{
		if (i == game_runtime.life - 1)
			display_draw_bitmap(x, y, game_broken_heart_bitmap, BITMAP_HEART_HEIGHT, BITMAP_HEART_HEIGHT, display_color_white);
		else
			display_draw_bitmap(x, y, game_heart_bitmap, BITMAP_HEART_HEIGHT, BITMAP_HEART_HEIGHT, display_color_white);

		x += (BITMAP_HEART_WIDTH + gap_between_hearts);
	}

	display_update();

	HAL_Delay(1500);

	display_clear();

	x = start_x;
	for (uint8_t i = 0; i < game_runtime.life - 1; i++)
	{
		display_draw_bitmap(x, y, game_heart_bitmap, BITMAP_HEART_HEIGHT, BITMAP_HEART_HEIGHT, display_color_white);
		x += (BITMAP_HEART_WIDTH + gap_between_hearts);
	}

	display_update();

	HAL_Delay(1500);

	input_enabled = true;
}

void game_draw_you_win(void)
{
	input_enabled = false;

	char buffer[32];
	buffer[0] = '\0';

	for (uint8_t i = 0; i < 5; i++)
	{
		strcat(buffer, EXCLAMATION_SYMBOL);

		display_clear();
		display_write_centered_string(buffer, display_font_16x24, display_color_white);
		display_update();

		HAL_Delay(300);
	}

	char *you_win = "YOU WIN!";
	buffer[0] = '\0';

	for (uint8_t i = 0; i < strlen("YOU WIN!"); i++)
	{
		buffer[i] = you_win[i];
		buffer[i + 1] = '\0';
		display_clear();
		display_write_centered_string(buffer, display_font_11x18, display_color_white);
		display_update();
		HAL_Delay(30);
	}
	HAL_Delay(1000);

	input_enabled = true;
}

void game_draw_game_over(void)
{
	char *game_over = "GAME OVER!";
	char buffer[32];
	buffer[0] = '\0';

	for (uint8_t i = 0; i < strlen("GAME OVER!"); i++)
	{
		buffer[i] = game_over[i];
		buffer[i + 1] = '\0';
		display_clear();
		display_write_centered_string(buffer, display_font_11x18, display_color_white);
		display_update();
		HAL_Delay(20);
	}
	HAL_Delay(1000);
}
