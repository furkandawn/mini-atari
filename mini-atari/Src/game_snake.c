/*
 * game_snake.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "game_snake.h"
#include <stdlib.h> // needed for rand() function
#include "ssd1306.h"
#include "game_menu.h"
#include "game_start.h"
#include "game_over.h"
#include "game_level.h"

static void snake_move(game_snake_t *game);
static void snake_spawn_food(game_snake_t *game);
static void snake_init(game_snake_t *game);
static void snake_update(game_snake_t *game);
static void snake_draw(const game_snake_t *game);

void snake_game(game_snake_t *game)
{
	game_start();
	snake_init(game);
	while(!game->game_over && current_menu_state == MENU_PLAYING)
	{
		HAL_Delay(game_get_delay_ms());
		snake_update(game);
		snake_draw(game);
	}
	game_over_draw_animated();
}

static void snake_init(game_snake_t *game)
{
	game->length = 3;
	game->direction = DIRECTION_RIGHT;
	game->game_over = false;

	for (int i = 0; i < game->length; i++)
	{
		game->segments[i].x = 64 - (i * BLOCK_SIZE);
		game->segments[i].y = 32;
	}

	snake_spawn_food(game);
}

static void snake_update(game_snake_t *game)
{
	if (joystick_is_pressed()) pause_game();
	if (game->game_over || current_menu_state != MENU_PLAYING) return;

	snake_move(game);

	point_t head = game->segments[0];
	if (head.x == game->food.x && head.y == game->food.y) // if snake eats the bait
	{
		game_score_increase();
		if (game->length < SNAKE_MAX_LENGTH)
		{
			game->segments[game->length] = game->segments[game->length - 1]; // create another piece on the tail
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
	for (int i = 1; i < game->length + 1; i++)
	{
		if (head.x == game->segments[i].x && head.y == game->segments[i].y)
		{
			game->game_over = true;
			break;
		}
	}
}

static void snake_move(game_snake_t *game)
{
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

	// move the body
	for (int i = game->length; i > 0; i--)
	{
		game->segments[i] = game->segments[i - 1];
	}

	// move the head
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
		uint8_t x = (rand() % (BOARD_WIDTH / BLOCK_SIZE) * BLOCK_SIZE);
		uint8_t y = (rand() % (BOARD_HEIGHT / BLOCK_SIZE) * BLOCK_SIZE);

		valid = true;

		// check if food generated on the snake
		for (uint8_t i = 0; i < game->length; i++)
		{
			if (game->segments[i].x == x && game->segments[i].y == y)
			{
				valid = false;
				break;
			}
		}

		if (valid)
		{
			game->food.x = x;
			game->food.y = y;
		}
	}
}

static void snake_draw(const game_snake_t *game)
{
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(0, 0, 127, 63, White);

	// draw food
	ssd1306_DrawSquare(game->food.x, game->food.y, BLOCK_SIZE, White);

	// draw snake
	uint8_t i = 0;
	for (i = 0; i < game->length; i++) // draw whole body
	{
		ssd1306_DrawSquare(game->segments[i].x, game->segments[i].y, BLOCK_SIZE, White);
	}

	ssd1306_UpdateScreen();
}
