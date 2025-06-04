/*
 * game_pong.c
 *
 *  Created on: May 26, 2025
 *      Author: furkandawn
 */

#include "game_pong.h"

// ===== Includes ===== //
// include display
#include "display_interface.h"

// include mini-atari libraries
#include "game_runtime.h"
#include "menu_paused.h"
#include "joystick.h"

// include other
#include "stm32f0xx_hal.h"	//	For HAL_Delay()
#include <stdlib.h>			//	For rand()
#include <stdio.h>			// for snprintf()

// ======= Macros/Constants ===== //
#define GAME_GRID 4
#define BORDER_SIZE 1
#define PADDLE_BLOCKS 3
#define PADDLE_LENGTH (PADDLE_BLOCKS * GAME_GRID)
#define PADDLE_DISTANCE (3 * GAME_GRID)				// x amount blocks of distance from closest side
#define PADDLE_Y_MIN GAME_GRID
#define PADDLE_Y_MAX (DISPLAY_HEIGHT - GAME_GRID - PADDLE_LENGTH)
#define PLAYER_X PADDLE_DISTANCE
#define CPU_X (DISPLAY_WIDTH - PADDLE_DISTANCE - GAME_GRID)
#define BALL_SIZE GAME_GRID
#define BALL_Y_MIN GAME_GRID
#define BALL_Y_MAX (DISPLAY_HEIGHT - GAME_GRID - BALL_SIZE)
#define BALL_SPEED_X (GAME_GRID * 3/2)
#define BALL_SPEED_Y GAME_GRID

// ===== Static File-Private Variables ===== //
static uint8_t cpu_paddle_speed;	// speed of cpu paddle

// ===== Public Global Variables ===== //

// ===== Static Function Declarations ===== //
static void pong_init(game_pong_t *game);
static void pong_restart(game_pong_t *game);
static void pong_draw_borders(void);
static void pong_draw(game_pong_t *game);
static void pong_move_ball(game_pong_t *game);
static void pong_set_cpu_paddle_speed(game_pong_t *game);
static void pong_move_paddle(game_pong_t *game);
static bool pong_check_score(game_pong_t *game);
static uint16_t pong_calculate_score(game_pong_t *game);
static void pong_update(game_pong_t *game);

// ===== Public API Function Definitions ===== //
void game_pong(game_pong_t *game)
{
	pong_init(game);

	while(!game_over_flag && !game_win_flag && current_menu_state == MENU_PLAYING)
	{
		pong_update(game);
	}

	game_runtime.score = pong_calculate_score(game);
	current_menu_state = MENU_GAMEOVER;
}

// ===== Static Function Definitions ===== //

static void pong_init(game_pong_t *game)
{
	game->player.score = game->cpu.score = 0;
	pong_restart(game);
}

static void pong_restart(game_pong_t *game)
{
	uint8_t paddle_center_y = ((DISPLAY_HEIGHT - PADDLE_LENGTH) / 2);
	paddle_center_y -= (paddle_center_y % GAME_GRID);

	game->player.prev_y = game->player.y = game->cpu.prev_y = game->cpu.y = paddle_center_y;

	game->ball.old_prev_x = game->ball.prev_x = game->ball.x = (DISPLAY_WIDTH / 2) - ((DISPLAY_WIDTH / 2) % GAME_GRID);
	game->ball.old_prev_y = game->ball.prev_y = game->ball.y = (DISPLAY_HEIGHT / 2) - ((DISPLAY_HEIGHT / 2) % GAME_GRID);

	game->ball.dx = (rand() % 2) ? -BALL_SPEED_X : +BALL_SPEED_X;
	game->ball.dy = (rand() % 2) ? +BALL_SPEED_X : -BALL_SPEED_X;

	pong_draw_borders();
}

static void pong_draw_borders(void)
{
	display_clear();

	// Draw top-bottom walls / borders
	display_fill_rectangle(0, 0, DISPLAY_WIDTH, BORDER_SIZE, display_color_white);
	display_fill_rectangle(0, DISPLAY_HEIGHT - BORDER_SIZE, DISPLAY_WIDTH, DISPLAY_HEIGHT, display_color_white);

	display_update();
}

static void pong_draw(game_pong_t *game)
{
	// Erase old player paddle
	display_fill_rectangle(PLAYER_X, game->player.prev_y, PLAYER_X + GAME_GRID, game->player.prev_y + PADDLE_LENGTH, display_color_black);

	// Draw new player paddle
	display_fill_rectangle(PLAYER_X, game->player.y, PLAYER_X + GAME_GRID, game->player.y + PADDLE_LENGTH, display_color_white);

	// Erase old cpu paddle
	display_fill_rectangle(CPU_X, game->cpu.prev_y, CPU_X + GAME_GRID, game->cpu.prev_y + PADDLE_LENGTH, display_color_black);

	// Draw cpu paddle
	display_fill_rectangle(CPU_X, game->cpu.y, CPU_X + GAME_GRID, game->cpu.y + PADDLE_LENGTH, display_color_white);

	// Erase old ball
	display_fill_square(game->ball.prev_x, game->ball.prev_y, GAME_GRID, display_color_black);

	// Draw ball
	display_fill_square(game->ball.x, game->ball.y, GAME_GRID, display_color_white);

	// Draw ball tail effect
	display_fill_square(game->ball.prev_x, game->ball.prev_y, GAME_GRID / 2, display_color_white);
	display_fill_square(game->ball.old_prev_x, game->ball.old_prev_y, GAME_GRID / 4, display_color_white);

	display_update();

	// Erase ball tail effect
	display_fill_square(game->ball.prev_x, game->ball.prev_y, GAME_GRID / 2, display_color_black);
	display_fill_square(game->ball.old_prev_x, game->ball.old_prev_y, GAME_GRID / 4, display_color_black);
}

static void pong_move_ball(game_pong_t *game)
{
	bool ball_bounce_from_paddle = false;

	// keep old positions
	game->ball.old_prev_x = game->ball.prev_x;
	game->ball.old_prev_y = game->ball.prev_y;
	game->ball.prev_x = game->ball.x;
	game->ball.prev_y = game->ball.y;

	// bounce the ball from borders
	if (game->ball.y <= BALL_Y_MIN || game->ball.y >= BALL_Y_MAX)
	{
		game->ball.dy *= -1;
		if (game->ball.y <= BALL_Y_MIN) game->ball.y = BALL_Y_MIN;
		else if (game->ball.y >= BALL_Y_MAX) game->ball.y = BALL_Y_MAX;
	}

	// bounce the ball from player paddle
	if (game->ball.x >= (PLAYER_X - GAME_GRID / 2) && game->ball.x <= PLAYER_X + GAME_GRID + (GAME_GRID / 2))
	{
		if (game->ball.y >= game->player.y && game->ball.y < game->player.y + PADDLE_LENGTH)
		{
			ball_bounce_from_paddle = true;
			if (game->ball.x <= PLAYER_X + GAME_GRID)
			{
				game->ball.x = PLAYER_X + GAME_GRID;

			}
		}
	}

	// bounce the ball from cpu
	if (game->ball.x <= CPU_X + GAME_GRID + (GAME_GRID / 2) && game->ball.x >= CPU_X - (GAME_GRID / 2))
	{
		if (game->ball.y >= game->cpu.y && game->ball.y < game->cpu.y + PADDLE_LENGTH)
		{
			ball_bounce_from_paddle = true;
			if (game->ball.x >= CPU_X) game->ball.x = CPU_X;
		}
	}

	if (ball_bounce_from_paddle)
	{
		// RNG for dy
		game->ball.dy *= (rand() % 4) ? +1 : -1;
		game->ball.dx *= -1;
	}

	// move the ball
	game->ball.x += game->ball.dx;
	game->ball.y += game->ball.dy;
}

static void pong_set_cpu_paddle_speed(game_pong_t *game)
{
	cpu_paddle_speed = (2 * GAME_GRID);
	if (game_get_continous_time() % 7 == 0)
	{
		cpu_paddle_speed = 0;
	}
}

static void pong_move_paddle(game_pong_t *game)
{
	// move player paddle
	joystick_direction_t direction = joystick_direction();

	switch(direction)
	{
	case DIRECTION_DOWN:
		if(game->player.y + PADDLE_LENGTH < DISPLAY_HEIGHT - GAME_GRID)
		{
			game->player.prev_y = game->player.y;
			game->player.y += GAME_GRID;
		}
		break;

	case DIRECTION_UP:
		if(game->player.y > GAME_GRID)
		{
			game->player.prev_y = game->player.y;
			game->player.y -= GAME_GRID;
		}
		break;

	default:
		break;
	}

	// move cpu paddle
	uint8_t cpu_center_y = (game->cpu.y + (PADDLE_LENGTH / 2));
	pong_set_cpu_paddle_speed(game);

	if (game->ball.y > (game->cpu.y + PADDLE_LENGTH / 2))
	{
		game->cpu.prev_y = game->cpu.y;
		game->cpu.y += cpu_paddle_speed;
		if (game->cpu.y > PADDLE_Y_MAX) game->cpu.y = PADDLE_Y_MAX;
	}
	else if (game->ball.y < cpu_center_y && game->cpu.y > GAME_GRID)
	{
		game->cpu.prev_y = game->cpu.y;
		game->cpu.y -= cpu_paddle_speed;
		if (game->cpu.y < PADDLE_Y_MIN) game->cpu.y = PADDLE_Y_MIN;
	}

}

static bool pong_check_score(game_pong_t *game)
{
	bool score_flag = false;
	// check player score
	if (game->ball.x >= DISPLAY_WIDTH - GAME_GRID)
	{
		game->player.score++;
		score_flag = true;
		display_clear();
		display_write_centered_string("PLAYER SCORE!", display_font_7x10, display_color_white);
		display_update();
		HAL_Delay(1000);
	}
	// check cpu score
	else if (game->ball.x <= GAME_GRID)
	{
		game->cpu.score++;
		score_flag = true;
		display_clear();
		display_write_centered_string("CPU SCORE!", display_font_7x10, display_color_white);
		display_update();
		HAL_Delay(1000);
	}

	if (score_flag)
	{
		display_clear();

		char buffer[64];
		snprintf(buffer, sizeof(buffer), "PLAYER: %d | CPU: %d", game->player.score, game->cpu.score);
		display_write_centered_string(buffer, display_font_6x8, display_color_white);

		display_update();
		HAL_Delay(2000);
		return true;
	}

	return false;
}

static uint16_t pong_calculate_score(game_pong_t *game)
{
	if (game->player.score > game->cpu.score) return (game->player.score - game->cpu.score);
	else return 0;
}

static void pong_update(game_pong_t *game)
{
	if (is_joystick_pressed() || is_button_pressed())
	{
		game_pause();

		if (game_over_flag || game_win_flag || current_menu_state != MENU_PLAYING) return;

		pong_draw_borders();
	}

	pong_draw(game);
	pong_move_paddle(game);
	pong_move_ball(game);

	if(pong_check_score(game))
	{
		pong_restart(game);
	}

	HAL_Delay(game_runtime.delay_ms);
}
