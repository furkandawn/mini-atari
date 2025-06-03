/*
 * game_breakout.c
 *
 *  Created on: May 30, 2025
 *      Author: furkandawn
 */


#include "game_breakout.h"


// ===== Includes ===== //
// include display
#include "display_interface.h"

// include mini-atari libraries
#include "game_runtime.h"
#include "menu_paused.h"
#include "joystick.h"

// include other
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"	//	For HAL_Delay()
#include <stdlib.h>			//	For rand()
#include <stdio.h>			// for snprintf()

// ======= Macros/Constants ===== //
#define GAME_GRID 3

// BLOCK MACROS
#define ROW_SIZE 5
#define COL_SIZE 12
#define BRICK_WIDTH (GAME_GRID * 2)
#define BRICK_HEIGHT GAME_GRID
#define BRICK_GAP GAME_GRID
#define BRICK_START_X ((DISPLAY_WIDTH - (COL_SIZE * (BRICK_WIDTH + BRICK_GAP) - BRICK_GAP)) / 2)
#define BRICK_START_Y GAME_GRID

// BALL MACROS
#define BALL_SIZE GAME_GRID
#define BALL_X_MIN BALL_SIZE
#define BALL_X_MAX ((DISPLAY_WIDTH - BALL_SIZE) - (DISPLAY_WIDTH % BALL_SIZE))
#define BALL_Y_MIN 0
#define BALL_Y_MAX ((PADDLE_Y - BALL_SIZE) - (PADDLE_Y % BALL_SIZE))
#define BALL_SPEED_X GAME_GRID
#define BALL_SPEED_Y GAME_GRID

// PADDLE MACROS
#define PADDLE_Y ((DISPLAY_HEIGHT - (GAME_GRID * 2)) - (DISPLAY_HEIGHT % GAME_GRID))
#define PADDLE_LENGTH (6 * GAME_GRID)
#define PADDLE_X_MIN 0
#define PADDLE_X_MAX ((DISPLAY_WIDTH - PADDLE_LENGTH) - (DISPLAY_WIDTH % GAME_GRID))

// ===== Static File-Private Variables ===== //
static uint8_t breakout_bricks[ROW_SIZE][COL_SIZE];

// ===== Public Global Variables ===== //
// ----- //

// ===== Static Function Declarations ===== //
static void breakout_respawn(game_breakout_t *game);
static void breakout_init(game_breakout_t *game);
static void setup_level(game_breakout_t *game);
static bool are_bricks_cleared(void);
static void breakout_draw_bricks(void);
static void breakout_draw(game_breakout_t *game);
static void breakout_move_paddle(game_breakout_t *game);
static void breakout_collision(game_breakout_t *game);
static void breakout_move_ball(game_breakout_t *game);
static void breakout_update(game_breakout_t *game);

// ===== Public API Function Definitions ===== //
void game_breakout(game_breakout_t *game)
{
	breakout_init(game);

	while (!game_over && current_menu_state == MENU_PLAYING)
	{
		breakout_update(game);
	}

	current_menu_state = MENU_GAMEOVER;
}

// ===== Static Function Definitions ===== //
static void breakout_respawn(game_breakout_t *game)
{
	uint8_t paddle_mid_x = ((DISPLAY_WIDTH - PADDLE_LENGTH) / 2) - ((DISPLAY_WIDTH % GAME_GRID));
	game->paddle.prev_x = game->paddle.x = paddle_mid_x;
	game->ball.x =(((DISPLAY_WIDTH - BALL_SIZE) / 2) - (DISPLAY_WIDTH % BALL_SIZE));
	game->ball.y = (PADDLE_Y - (BALL_SIZE * 4));

	game->ball.dx = BALL_SPEED_X;
	game->ball.dy = BALL_SPEED_Y;

	breakout_draw_bricks();
}

static void breakout_init(game_breakout_t *game)
{
	setup_level(game);

	breakout_respawn(game);
}

static void setup_level(game_breakout_t *game)
{
	uint8_t fill_row[ROW_SIZE] = {0};
	uint8_t skip_col[COL_SIZE] = {0};
	bool set_level = false;

	switch(game_get_level())
	{
	case 1:
		fill_row[2] = 1;
		fill_row[1] = 1;
		for (uint8_t i = 0; i < 3; i++)
		{
			skip_col[i] = 1;
			skip_col[COL_SIZE - 1 - i] = 1;
		}
		set_level = true;
		break;

	case 2:
		fill_row[3] = 1;
		fill_row[2] = 1;
		fill_row[1] = 1;
		for (uint8_t i = 4; i < 8; i++)
		{
			skip_col[i] = 1;
		}
		set_level = true;
		break;

	case 3:
		fill_row[4] = 1;
		fill_row[3] = 1;
		fill_row[1] = 1;
		fill_row[0] = 1;
		for (uint8_t i = 5; i < 7; i++)
		{
			skip_col[i] = 1;
		}
		set_level = true;
		break;

	default:
		game_win();
		game_over = true;
		set_level = false;
		break;
	}

	if (set_level)
	{
		for (uint8_t row = 0; row < ROW_SIZE; row++)
		{
			if (!fill_row[row]) continue;
			for (uint8_t col = 0; col < COL_SIZE; col++)
			{
				if (skip_col[col]) continue;
				breakout_bricks[row][col] = 1;
			}
		}

		breakout_respawn(game);
	}
}

static bool are_bricks_cleared(void)
{
	for (uint8_t row = 0; row < ROW_SIZE; row++)
	{
		for (uint8_t col = 0; col < COL_SIZE; col++)
		{
			if (breakout_bricks[row][col]) return false;
		}
	}
	return true;
}

static void breakout_draw_bricks(void)
{
	display_clear();

	// draw border
	display_fill_rectangle(0, 0, 0, DISPLAY_HEIGHT, display_color_white);
	display_fill_rectangle(DISPLAY_WIDTH - 1, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, display_color_white);

	// draw bricks
	for (uint8_t row = 0; row < ROW_SIZE; row++)
	{
		for (uint8_t col = 0; col < COL_SIZE; col++)
		{
			if (!breakout_bricks[row][col]) continue;

			uint8_t x = (BRICK_START_X + (col * (BRICK_WIDTH + BRICK_GAP)));
			uint8_t y = (BRICK_START_Y + (row * (BRICK_HEIGHT + BRICK_GAP)));

			display_fill_rectangle(x, y, x + BRICK_WIDTH, y + BRICK_HEIGHT, display_color_white);
		}
	}

	display_update();
}

static void breakout_draw(game_breakout_t *game)
{
	// Erase old
	display_fill_square(game->ball.x - game->ball.dx, game->ball.y - game->ball.dy, BALL_SIZE, display_color_black);
	display_fill_rectangle(game->paddle.prev_x, PADDLE_Y, game->paddle.prev_x + PADDLE_LENGTH, PADDLE_Y + GAME_GRID, display_color_black);

	// Draw new
	display_fill_square(game->ball.x, game->ball.y, BALL_SIZE, display_color_white);
	display_fill_rectangle(game->paddle.x, PADDLE_Y, game->paddle.x + PADDLE_LENGTH, PADDLE_Y + GAME_GRID, display_color_white);

	display_update();
}

static void breakout_move_paddle(game_breakout_t *game)
{
	joystick_direction_t direction = joystick_direction();

	switch(direction)
	{
	case DIRECTION_RIGHT:
		if (game->paddle.x >= PADDLE_X_MIN && game->paddle.x < PADDLE_X_MAX)
		{
			game->paddle.prev_x = game->paddle.x;
			game->paddle.x += (GAME_GRID * 2);

			if (game->paddle.x > PADDLE_X_MAX) game->paddle.x = PADDLE_X_MAX;
		}
		break;

	case DIRECTION_LEFT:
		if (game->paddle.x > PADDLE_X_MIN && game->paddle.x <= PADDLE_X_MAX)
		{
			game->paddle.prev_x = game->paddle.x;
			if (game->paddle.x >= (GAME_GRID * 2))
				game->paddle.x -= (GAME_GRID * 2);
			else
				game->paddle.x = PADDLE_X_MIN;
		}
		break;

	default:
		break;
	}
}

static void breakout_collision(game_breakout_t *game)
{
	uint8_t bottom_brick_y = BRICK_START_Y + (ROW_SIZE * (BRICK_HEIGHT + BRICK_GAP));
	if (game->ball.y + BALL_SIZE > bottom_brick_y) return;

    int16_t next_ball_left   = game->ball.x + game->ball.dx;
    int16_t next_ball_right  = next_ball_left + BALL_SIZE;
    int16_t next_ball_top    = game->ball.y + game->ball.dy;
    int16_t next_ball_bottom = next_ball_top + BALL_SIZE;

    uint8_t ball_left   = game->ball.x;
    uint8_t ball_right  = ball_left + BALL_SIZE;
    uint8_t ball_top    = game->ball.y;
    uint8_t ball_bottom = ball_top + BALL_SIZE;

    for (uint8_t row = 0; row < ROW_SIZE; row++)
    {
        for (uint8_t col = 0; col < COL_SIZE; col++)
        {
            if (!breakout_bricks[row][col]) continue;

            uint8_t brick_x = BRICK_START_X + (col * (BRICK_WIDTH + BRICK_GAP));
            uint8_t brick_y = BRICK_START_Y + (row * (BRICK_HEIGHT + BRICK_GAP));
            uint8_t brick_left   = brick_x;
            uint8_t brick_right  = brick_x + BRICK_WIDTH;
            uint8_t brick_top    = brick_y;
            uint8_t brick_bottom = brick_y + BRICK_HEIGHT;

            // Check collision with next position
            if (next_ball_right >= brick_left && next_ball_left <= brick_right &&
                next_ball_bottom >= brick_top && next_ball_top <= brick_bottom)
            {
                // Remove the brick and erase visually
                breakout_bricks[row][col] = 0;
                display_fill_rectangle(brick_left, brick_top, brick_right, brick_bottom, display_color_black);

                // Determine collision side
				bool hit_horizontal = (ball_right <= brick_left) || (ball_left >= brick_right);
				bool hit_vertical   = (ball_bottom <= brick_top) || (ball_top >= brick_bottom);

				if (hit_horizontal && hit_vertical)
				{
					game->ball.dx *= -1;
					game->ball.dy *= -1;
				}
				else if (hit_horizontal)
				{
					game->ball.dx *= -1;
				}
				else
				{
					game->ball.dy *= -1;
				}

                game_update_progress();
                return;
            }
        }
    }
}

static void breakout_move_ball(game_breakout_t *game)
{
	int16_t next_x = game->ball.x + game->ball.dx;
	int16_t next_y = game->ball.y + game->ball.dy;

	// ball - block collision
	breakout_collision(game);

	// left-right border hit
	if (next_x < BALL_X_MIN || next_x > BALL_X_MAX)
	{
		game->ball.dx *= -1;
		next_x = game->ball.x + game->ball.dx;
	}

	// ceiling hit
	if (next_y < BALL_Y_MIN)
	{
		game->ball.dy *= -1;
		next_y = game->ball.y + game->ball.dy;
	}

	// paddle bounce
	if (next_y + BALL_SIZE >= PADDLE_Y && next_y <= PADDLE_Y + GAME_GRID &&
		next_x + BALL_SIZE >= game->paddle.x && next_x <= game->paddle.x + PADDLE_LENGTH)
	{
		game->ball.dy *= -1;

		uint8_t paddle_center = game->paddle.x + PADDLE_LENGTH / 2;
		uint8_t ball_center = next_x + BALL_SIZE / 2;

		if (ball_center < paddle_center)
		{
			if (game->ball.dx > 0)
				game->ball.dx *= -1;
			else
				game->ball.dx *= 1;

		}
		else if (ball_center > paddle_center)
		{
			if (game->ball.dx < 0)
				game->ball.dx *= -1;
			else
				game->ball.dx *= 1;
		}
		// if exactly in the center, keep dx as is

		next_y = game->ball.y + game->ball.dy;
	}

	if (game->ball.y > BALL_Y_MAX)
	{
		if (game_lose_life()) breakout_respawn(game);
		return;
	}

	game->ball.x += game->ball.dx;
	game->ball.y += game->ball.dy;
}

static void breakout_update(game_breakout_t *game)
{
	if (is_joystick_pressed() || is_button_pressed())
	{
		game_pause();

		if (game_over || current_menu_state != MENU_PLAYING) return;

		breakout_draw_bricks();
	}

	breakout_move_paddle(game);
	breakout_move_ball(game);
	breakout_draw(game);

	if (are_bricks_cleared())
	{
		game_update_level();
		setup_level(game);
	}

	HAL_Delay(game_runtime.delay_ms);
}
