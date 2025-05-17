/*
 * game_snake.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "game_snake.h"

// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "game_init.h"
#include "game_score.h"
#include "menu_logic.h"
#include "menu_paused.h"

// include other
#include <stdlib.h> // needed for rand() function

// includes end <<-----

static void snake_move(game_snake_t *game);
static void snake_spawn_food(game_snake_t *game);
static void snake_init(game_snake_t *game);
static void snake_update(game_snake_t *game);
static void snake_draw(const game_snake_t *game);

void snake_game(game_snake_t *game)
{
	snake_init(game);

	while(!game->game_over && current_menu_state == MENU_PLAYING)
	{
		snake_draw(game);
		snake_update(game);
	}

	HAL_Delay(500);
	current_menu_state = MENU_GAMEOVER;
}

static void snake_init(game_snake_t *game)
{
	game->length = 3;
	game->direction = DIRECTION_RIGHT;
	game->game_over = false;

	for (int i = 0; i < game->length; i++)
	{
		game->segments[i].x = (BOARD_WIDTH / 2) - (i * BLOCK_SIZE);
		game->segments[i].y = (BOARD_HEIGHT / 2);
	}

	snake_spawn_food(game);
}

static void snake_update(game_snake_t *game)
{
	if (joystick_is_pressed()) game_pause();
	if (game->game_over || current_menu_state != MENU_PLAYING) return;

	snake_move(game);

	// checks if snake eats the bait
	point_t head = game->segments[0];
	if (head.x == game->food.x && head.y == game->food.y)
	{
		game_increase_score();
		if (game->length < SNAKE_MAX_LENGTH)
		{
			game->segments[game->length] = game->segments[game->length - 1]; // creates another piece on the tail
			game->length++;
		}
		snake_spawn_food(game);
	}

	// snake wall-collapsion
	if (head.x >= BOARD_WIDTH || head.y >= BOARD_HEIGHT)
	{
		game->game_over = true;
		return;
	}

	// snake self-collapsion
	for (int i = 1; i < game->length; i++)
	{
		if (head.x == game->segments[i].x && head.y == game->segments[i].y)
		{
			game->game_over = true;
			break;
		}
	}

	HAL_Delay(game_get_delay_ms());
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
	case DIRECTION_UP : game->segments[0].y -= BLOCK_SIZE; break;
	case DIRECTION_DOWN : game->segments[0].y += BLOCK_SIZE; break;
	case DIRECTION_LEFT : game->segments[0].x -= BLOCK_SIZE; break;
	case DIRECTION_RIGHT : game->segments[0].x += BLOCK_SIZE; break;
	case DIRECTION_NONE : break;
	default: break;
	}
}


static void snake_spawn_food(game_snake_t *game)
{
	bool valid = false;

	while (!valid)
	{
		// we use ...* BLOCK_SIZE to avoid food spawning on locations that snake cannot pass through
		uint8_t x = (rand() % (BOARD_WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
		uint8_t y = (rand() % (BOARD_HEIGHT / BLOCK_SIZE)) * BLOCK_SIZE;

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

static void snake_draw(const game_snake_t *game)
{
	oled_clear();

	oled_draw_rectangle(0, 0, BOARD_WIDTH - 1, BOARD_HEIGHT - 1, oled_color_white);

	// draw food
	oled_fill_square(game->food.x, game->food.y, BLOCK_SIZE, oled_color_white);

	// draw snake
	uint8_t i = 0;
	for (i = 0; i < game->length; i++) // draw whole body
	{
		oled_fill_square(game->segments[i].x, game->segments[i].y, BLOCK_SIZE, oled_color_white);
	}

	oled_update();
}
