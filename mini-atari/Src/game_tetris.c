/*
 * game_tetris.c
 *
 *  Created on: May 9, 2025
 *      Author: furkandawn
 */


#include "game_tetris.h"


// === Includes Start === //

// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "game_tetris_shapes.h"
#include "joystick.h"
#include "game_runtime.h"
#include "menu_logic.h"
#include "menu_paused.h"

// include other
#include "stm32f0xx_hal.h"
#include <stdlib.h>		// needed for rand() function
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// === Includes End === //

#define GAME_GRID 3		// 3x3 pixel blocks
#define GAME_OFFSET GAME_GRID
#define GAME_START_POSITION_X 0
#define INPUT_DELAY_MS 50

static uint8_t board[TETRIS_BOARD_HEIGHT][TETRIS_BOARD_WIDTH] = {0};
static uint8_t next_tetrimino_index;

// game input delay
static uint32_t last_update;
static uint32_t last_input;

static void tetris_init(game_tetris_t *game);
static void tetris_update(game_tetris_t *game);
static bool tetrimino_can_move(game_tetris_t *game, uint8_t dx, uint8_t dy);


void game_tetris(game_tetris_t *game)
{
	tetris_init(game);

	while(!game_over && current_menu_state == MENU_PLAYING)
	{
		tetris_update(game);
	}

	HAL_Delay(500);
	current_menu_state = MENU_GAMEOVER;
}

/* menu and outline draw functions */
static void tetris_draw_board(void)
{
	for (uint8_t i = 0; i < GAME_GRID; i++)
	{
		// border
		display_draw_rectangle(GAME_START_POSITION_X + i, GAME_START_POSITION_X + i, ((TETRIS_BOARD_WIDTH * GAME_GRID) + GAME_OFFSET) + i, (TETRIS_BOARD_HEIGHT * GAME_GRID) + i, display_color_white);
	}

	for (uint8_t y = 0; y < TETRIS_BOARD_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < TETRIS_BOARD_WIDTH; x++)
		{
			if (board[y][x]) display_fill_square(((x * GAME_GRID) + GAME_OFFSET), (y * GAME_GRID), GAME_GRID, display_color_white);
		}
	}
}

static void draw_next_tetrimino(uint8_t x, uint8_t y)
{
	for (uint8_t i = 0; i < TETRIMINO_SIZE; i++)
	{
		for (uint8_t j = 0; j < TETRIMINO_SIZE; j++)
		{
			if (!tetriminos[next_tetrimino_index].rotation[0][i][j]) continue;

			uint8_t draw_x = (x + (j * GAME_GRID));
			uint8_t draw_y = (y + (i * GAME_GRID));
			display_fill_square(draw_x, draw_y, GAME_GRID, display_color_white);
		}
	}
}

static void tetris_draw_stats(void)
{
	uint8_t x = ((TETRIS_BOARD_WIDTH * GAME_GRID) + (GAME_GRID * 2));
	uint8_t y = GAME_GRID;
	char buffer[16];

	snprintf(buffer, sizeof(buffer), "SCORE %d", game_get_score());
	display_set_cursor(x, y);
	display_write_string(buffer, display_font_7x10, display_color_white);

	y += font_height(display_font_7x10) * 2;
	display_set_cursor(x, y);
	display_write_string("NEXT", display_font_7x10, display_color_white);

	y += font_height(display_font_7x10);
	draw_next_tetrimino(x, y);

	y += font_height(display_font_7x10) * 2;
	snprintf(buffer, sizeof(buffer), "TIME %d", game_get_continous_time());
	display_set_cursor(x, y);
	display_write_string(buffer, display_font_7x10, display_color_white);

}

static void tetrimino_draw(game_tetris_t *game)
{
	for (uint8_t i = 0; i < TETRIMINO_SIZE; i++)
	{
		for (uint8_t j = 0; j < TETRIMINO_SIZE; j++)
		{
			if (!tetriminos[game->type].rotation[game->rotation][i][j]) continue;

			uint8_t x = (game->x + (j * GAME_GRID) + GAME_OFFSET);
			uint8_t y = (game->y + (i * GAME_GRID));
			display_fill_square(x, y, GAME_GRID, display_color_white);
		}
	}
}

static void tetris_draw(game_tetris_t *game)
{
	display_clear();

	tetris_draw_board();
	tetris_draw_stats();
	tetrimino_draw(game);

	display_update();
}

static void tetrimino_spawn(game_tetris_t *game)
{
	game->type = next_tetrimino_index;
	game->rotation = 0;
	game->x = ((TETRIS_BOARD_WIDTH / 2) * GAME_GRID);
	game->y = GAME_GRID;

	next_tetrimino_index = rand() % TETRIMINO_TYPE_COUNT;

	if (!tetrimino_can_move(game, 0, 0)) game_over = true;
}

/*
 * Outer loop is row index, needed for height, vertical movement - y index -
 * Inner loop is column index, needed for width, horizontal movement - x index -
 * dx and dy are the values you would like to move a tetrimino to specific directions (eg. "+1" for dx is 1 block to right)
 */
static bool tetrimino_can_move(game_tetris_t *game, uint8_t dx, uint8_t dy)
{
	for (uint8_t i = 0; i < TETRIMINO_SIZE; i++)
	{
		for (uint8_t j = 0; j < TETRIMINO_SIZE; j++)
		{
			if (!tetriminos[game->type].rotation[game->rotation][i][j]) continue;

		int8_t new_x = (game->x / GAME_GRID) + j + dx;
		int8_t new_y = (game->y / GAME_GRID) + i + dy;

			// Check boundaries
			if (new_x < 0 || new_x >= TETRIS_BOARD_WIDTH || new_y < 0 || new_y >= TETRIS_BOARD_HEIGHT) return false;

			// Check collision with placed blocks
			if (new_y >= 0 && board[new_y][new_x]) return false;
		}
	}
	return true;
}

static void tetrimino_move(game_tetris_t *game)	// to do: check placed tetriminos to move. Currently it can move through blocks
{
	joystick_direction_t dir = joystick_direction();

	switch(dir)
	{
	case DIRECTION_DOWN: if (tetrimino_can_move(game, 0, 1)) game->y += GAME_GRID; break;
	case DIRECTION_LEFT: if (tetrimino_can_move(game, -1, 0)) game->x -= GAME_GRID; break;
	case DIRECTION_RIGHT: if (tetrimino_can_move(game, 1, 0)) game->x += GAME_GRID; break;
	default: break;
	}
}

static void tetrimino_rotate(game_tetris_t *game)
{
	if (button_is_pressed())
	{
		uint8_t original_rotation = game->rotation;
		game->rotation = ((game->rotation + 1) % tetriminos[game->type].rotation_count);

		// revert if new rotation hits the walls
		if (!tetrimino_can_move(game, 0, 0))
		{
			game->rotation = original_rotation;
		}
	}
}

static void tetrimino_land(game_tetris_t *game)
{
	for (uint8_t i = 0; i < TETRIMINO_SIZE; i++)
	{
		for (uint8_t j = 0; j < TETRIMINO_SIZE; j++)
		{
			if (!tetriminos[game->type].rotation[game->rotation][i][j]) continue;

			uint8_t x = ((game->x / GAME_GRID) + j);
			uint8_t y = ((game->y / GAME_GRID) + i);

			if (x < TETRIS_BOARD_WIDTH && y < TETRIS_BOARD_HEIGHT) board[y][x] = 1;
		}
	}
}

static void tetris_clear_line(void)
{
	for (int8_t y = TETRIS_BOARD_HEIGHT - 1; y >= 0 ; y--)
	{
		bool line_is_full = true;

		for (uint8_t x = 0; x < TETRIS_BOARD_WIDTH; x++)
		{
			if (!board[y][x])
			{
				line_is_full = false;
				break;
			}
		}

		if (line_is_full)
		{
			// shift down the rows above
			for (uint8_t row = y; row > 0; row--)
			{
				for (uint8_t col = 0; col < TETRIS_BOARD_WIDTH; col++)
				{
					board[row][col] = board[row - 1][col];
				}
			}

			// clear the top row
			for (uint8_t col = 0; col < TETRIS_BOARD_WIDTH; col++)
			{
				board[0][col] = 0;
			}

			game_update_progress();
			y++;
		}
	}
}


static void tetris_init(game_tetris_t *game)
{
	memset(board, 0, sizeof(board));
	last_update = HAL_GetTick();
	last_input = HAL_GetTick();
	next_tetrimino_index = rand() % TETRIMINO_TYPE_COUNT;
	tetrimino_spawn(game);
}

static void tetris_update(game_tetris_t *game)
{
	uint32_t now = HAL_GetTick();

	if (joystick_is_pressed()) game_pause();
	if (game_over || current_menu_state != MENU_PLAYING) return;

	if (now - last_input >= INPUT_DELAY_MS)
	{
		tetrimino_rotate(game);
		tetrimino_move(game);
		last_input = now;
	}


	if (now - last_update >= game_get_delay_ms())
	{
		// gravity
		if (tetrimino_can_move(game, 0, 1))
		{
			game->y += GAME_GRID;
		}
		else
		{
			tetrimino_land(game);
			tetris_clear_line();
			tetrimino_spawn(game);
		}
		last_update = now;
	}

	tetris_draw(game);
}
