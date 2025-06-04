/*
 * game_snake.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "game_snake.h"

// ===== Includes ===== //
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "game_runtime.h"
#include "game_ui.h"
#include "menu_paused.h"
#include "joystick.h"

// include other
#include "stm32f0xx_hal.h"	//	For HAL_Delay()
#include <stdlib.h>			//	For rand()

// ======= Macros/Constants ===== //
#define GAME_GRID 4		// 4x4 pixel blocks

// ===== Static File-Private Variables ===== //
// ----- //

// ===== Public Global Variables ===== //
// ----- //

// ===== Static Function Declarations ===== //
static void snake_move(game_snake_t *game);
static void snake_spawn_food(game_snake_t *game);
static void snake_init(game_snake_t *game);
static void snake_update(game_snake_t *game);
static void snake_draw(const game_snake_t *game);

// ===== Public API Function Definitions ===== //
void snake_game(game_snake_t *game)
{
	snake_init(game);

	while(!game_over_flag && !game_win_flag && current_menu_state == MENU_PLAYING)
	{
		snake_update(game);
	}

	current_menu_state = MENU_GAMEOVER;
}

// ===== Static Function Definitions ===== //
// Draw Functions

static void snake_draw(const game_snake_t *game)
{
	display_clear();

	display_draw_rectangle(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, display_color_white);

	// draw food
	display_fill_square(game->food.x, game->food.y, GAME_GRID, display_color_white);

	// draw snake
	uint8_t i = 0;
	for (i = 0; i < game->length; i++) // draw whole body
	{
		display_fill_square(game->segments[i].x, game->segments[i].y, GAME_GRID, display_color_white);
	}

	display_update();
}

// Game Logic

static void snake_init(game_snake_t *game)
{
	game->length = 3;
	game->direction = DIRECTION_RIGHT;

	for (int i = 0; i < game->length; i++)
	{
		game->segments[i].x = (DISPLAY_WIDTH / 2) - (i * GAME_GRID);
		game->segments[i].y = (DISPLAY_HEIGHT / 2);
	}

	snake_spawn_food(game);
}

static void snake_move(game_snake_t *game)
{
	// gets snake direction
	joystick_direction_t input = joystick_direction();
	if (input != DIRECTION_NONE)
	{
		if ((input == DIRECTION_UP && game->direction != DIRECTION_DOWN) ||
			(input == DIRECTION_DOWN && game->direction != DIRECTION_UP) ||
			(input == DIRECTION_LEFT && game->direction != DIRECTION_RIGHT) ||
			(input == DIRECTION_RIGHT && game->direction != DIRECTION_LEFT))
		{
			game->direction = input;
		}
	}

	// moves the body
	for (int i = game->length; i > 0; i--)
	{
		game->segments[i] = game->segments[i - 1];
	}

	// moves the head
	switch(game->direction)
	{
	case DIRECTION_UP : game->segments[0].y -= GAME_GRID; break;
	case DIRECTION_DOWN : game->segments[0].y += GAME_GRID; break;
	case DIRECTION_LEFT : game->segments[0].x -= GAME_GRID; break;
	case DIRECTION_RIGHT : game->segments[0].x += GAME_GRID; break;
	default: break;
	}
}


static void snake_spawn_food(game_snake_t *game)
{
	bool valid = false;

	while (!valid)
	{
		// we use ...* GAME_GRID to avoid food spawning on locations that snake cannot pass through
		uint8_t x = (rand() % (DISPLAY_WIDTH / GAME_GRID)) * GAME_GRID;
		uint8_t y = (rand() % (DISPLAY_HEIGHT / GAME_GRID)) * GAME_GRID;

		valid = true;

		// check if food generated on the snake
		for (uint8_t i = 0; i < game->length; i++)
		{
			if (game->segments[i].x == x && game->segments[i].y == y)
			{
				valid = false;	// food generated on snake, repeat the process.
				break;	// break out of for loop
			}
		}

		// food is not generated on snake, thus complete the function
		if (valid)
		{
			game->food.x = x;
			game->food.y = y;
		}
	}
}

static void snake_update(game_snake_t *game)
{
	if (is_joystick_pressed() || is_button_pressed()) game_pause();
	if (game_over_flag || game_win_flag || current_menu_state != MENU_PLAYING) return;

	snake_draw(game);
	snake_move(game);

	// checks if snake eats the bait
	point_t head = game->segments[0];
	if (head.x == game->food.x && head.y == game->food.y)
	{
		game_update_progress();
		if (game->length < SNAKE_MAX_LENGTH)
		{
			game->segments[game->length] = game->segments[game->length - 1]; // creates another piece on the tail
			game->length++;
		}
		snake_spawn_food(game);
	}

	// snake wall-collision
	if (head.x >= DISPLAY_WIDTH || head.y >= DISPLAY_HEIGHT || head.x < 0 || head.y < 0)
	{
		game_lose_life();
		return;
	}

	// self-collision
	for (int i = 1; i < game->length; i++)
	{
		if (head.x == game->segments[i].x && head.y == game->segments[i].y)
		{
			game_lose_life();
			break;
		}
	}

	HAL_Delay(game_get_delay_ms());
}
