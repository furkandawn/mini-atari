/*
 * game_init.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "game_init.h"

// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "game_score.h"
#include "menu_main.h"
#include "menu_gameover.h"

// include mini-atari games
#include "game_snake.h"

// include other
#include <stdio.h>

// includes end <<-----

#define COUNTDOWN 3

static void game_draw_start(void)
{
	oled_clear();
	oled_update();
	HAL_Delay(200);

	for (uint8_t i = COUNTDOWN; i > 0; i--)
	{
		oled_clear();

		char buffer[4];
		sprintf(buffer, "%d", i);

		oled_write_centered_string(buffer, oled_font_16x26, oled_color_white);

		oled_update();
		HAL_Delay(500);
	}

	animation_shown = false;
}

void game_init(void)
{
	game_draw_start();
	game_reset();

	switch(current_game_type)
	{
	case GAME_SNAKE:
	{
		game_snake_t game;
		snake_game(&game);
		break;
	}
	case GAME_TETRIS:
	{
		break;
	}
	case GAME_3:
	{
		break;
	}
	case GAME_4:
	{
		break;
	}
	case GAME_5:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}
