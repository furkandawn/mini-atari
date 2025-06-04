/*
 * game_space_invaders.c
 *
 *  Created on: May 31, 2025
 *      Author: furkandawn
 */


#include "game_space_invaders.h"

// ===== Includes ===== //
#include "game_space_invaders_config.h"
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "game_runtime.h"
#include "menu_paused.h"
#include "joystick.h"

// include other
#include <stdlib.h>			//	For rand()
#include "stm32f0xx_hal.h"

// ======= Macros/Constants ===== //
#define GAME_GRID 3 // change GAME_GRID in "game_space_invaders_config.c" as well if you change this
#define MAX_ROW_NUMBER 5
#define ROW_HEIGHT (4 + ENTITY_HEIGHT_PX)

typedef struct {
    int16_t dx;
    uint32_t last_move_ms;
    uint32_t movement_delay_ms;
} enemy_row_state_t;

// ===== Static File-Private Variables ===== //
static enemy_row_state_t enemy_rows[MAX_ROW_NUMBER];
static bool is_row_full[MAX_ROW_NUMBER] = {0};

// ===== Public Global Variables ===== //
// ----- //

// ===== Static Function Declarations ===== //
static void spcinv_init(game_spcinv_t *game);
static void setup_level(game_spcinv_t *game);
static void spawn_enemy(game_spcinv_t *game, spcinv_enemy_type_t type);
static void clear_enemy_count(game_spcinv_t *game);
static void spaceship_respawn(game_spcinv_t *game);
static void spcinv_update(game_spcinv_t *game);

// draw functions
static void draw_spaceship(game_spcinv_t *game);
static void draw_enemy(game_spcinv_t *game);
static void draw_bullets(game_spcinv_t *game);

// movement & firing
static void enemy_move(game_spcinv_t *game);
static void enemy_fire(game_spcinv_t *game);
static void spaceship_move(game_spcinv_t *game);
static void spaceship_fire(game_spcinv_t *game);

// collision
static bool is_bitmap_pixel_set_16x8(const uint8_t *bitmap, uint8_t x, uint8_t y);
static bool is_bitmap_pixel_set_8x8(const uint8_t *bitmap, uint8_t x, uint8_t y);
static void move_bullets(game_spcinv_t *game);

// ===== Public API Function Definitions ===== //
void game_spcinv(game_spcinv_t *game)
{
	spcinv_init(game);

	while (!game_over_flag && !game_win_flag && current_menu_state == MENU_PLAYING)
	{
		spcinv_update(game);
	}

	current_menu_state = MENU_GAMEOVER;
}
// ===== Static Function Definitions ===== //
static void spcinv_init(game_spcinv_t *game)
{
	game->player.x = ((DISPLAY_WIDTH - ENTITY_WIDTH_PX) / 2);
	game->player.bullet.active = false;

	setup_level(game);
}

static void setup_level(game_spcinv_t *game)
{
	game->enemy_count = 0;

	for (uint8_t i = 0; i < MAX_ROW_NUMBER; i++) {
		enemy_rows[i].dx = GAME_GRID;
		enemy_rows[i].last_move_ms = 0;
		enemy_rows[i].movement_delay_ms = 1000;
		is_row_full[i] = false;
	}

	switch(game_get_level())
	{
	case 1:
		spawn_enemy(game, SPCINV_SQUID);
		break;

	case 2:
		spawn_enemy(game, SPCINV_SQUID);
		spawn_enemy(game, SPCINV_CRAB);
		break;

	case 3:
		spawn_enemy(game, SPCINV_SQUID);
		spawn_enemy(game, SPCINV_OCTOPUS);
		break;

	case 4:
		spawn_enemy(game, SPCINV_OCTOPUS);
		spawn_enemy(game, SPCINV_CRAB);
		spawn_enemy(game, SPCINV_SQUID);
		break;

	case 5:
		spawn_enemy(game, SPCINV_UFO);
		spawn_enemy(game, SPCINV_OCTOPUS);
		spawn_enemy(game, SPCINV_SQUID);
		break;

	default:
		game_win_flag = true;
		break;
	}
}

static void spawn_enemy(game_spcinv_t *game, spcinv_enemy_type_t type)
{
	uint8_t empty_row_index = 0;
	const spcinv_enemy_t *cfg = &enemy_config[type];

	for (uint8_t i = 0; i < MAX_ROW_NUMBER; i++)
	{
		if (!is_row_full[i])
		{
			is_row_full[i] = true;
			empty_row_index = i;
			break;
		}
	}

	enemy_rows[empty_row_index].movement_delay_ms = cfg->movement_delay_ms;
	enemy_rows[empty_row_index].dx = cfg->dx;

	for (uint8_t i = 0; i < cfg->horde_number; i++)
	{
		if (game->enemy_count >= SPCINV_MAX_ENEMIES) break;

		spcinv_enemy_t *e = &game->enemy[game->enemy_count++];

		uint8_t spawn_x = (i * ENTITY_WIDTH_PX);
		uint8_t spawn_y = (empty_row_index * ROW_HEIGHT);

		*e = *cfg;
		e->x = spawn_x;
		e->y = spawn_y;
		e->last_bullet_ms = HAL_GetTick();
		e->bullet.active = false;
		e->bullet.dy = ENTITY_BULLET_SPEED;
	}
}

static void clear_enemy_count(game_spcinv_t *game)
{
	uint8_t new_count = 0;

	for (uint8_t i = 0; i < game->enemy_count; i++)
	{
		spcinv_enemy_t *e = &game->enemy[i];
		if (!e->alive && !e->bullet.active) continue;

		game->enemy[new_count++] = *e;
	}

	game->enemy_count = new_count;
}

static void spaceship_respawn(game_spcinv_t *game)
{
	for (uint8_t i = 0; i < game->enemy_count; i++)
	{
		spcinv_enemy_t *e = &game->enemy[i];
		if (e->bullet.active) e->bullet.active = false;
	}

	game->player.x = ((DISPLAY_WIDTH - ENTITY_WIDTH_PX)/ 2);
	if (game->player.bullet.active) game->player.bullet.active = false;
}

static void draw_spaceship(game_spcinv_t *game)
{
	display_draw_bitmap(game->player.x, SPACESHIP_Y, spaceship_bitmap, SPACESHIP_WIDTH_PX, SPACESHIP_HEIGHT_PX, display_color_white);
}

static void draw_enemy(game_spcinv_t *game)
{
	for (uint8_t i = 0; i < game->enemy_count; i++)
	{
		const spcinv_enemy_t *e = &game->enemy[i];
		if (!e->alive) continue;

		const uint8_t *bitmap = enemy_bitmap[e->type];
		display_draw_bitmap(e->x, e->y, bitmap, ENTITY_WIDTH_PX, ENTITY_HEIGHT_PX, display_color_white);
	}
}

static void draw_bullets(game_spcinv_t *game)
{
	// draw spaceship bullet
	if (game->player.bullet.active) display_fill_square(game->player.bullet.x, game->player.bullet.y, GAME_GRID, display_color_white);

	// draw enemy bullets
	for (uint8_t i = 0; i < game->enemy_count; i++)
	{
		spcinv_enemy_t *e = &game->enemy[i];
		if (!e->bullet.active) continue;

		display_fill_square(e->bullet.x, e->bullet.y, GAME_GRID, display_color_white);
	}
}

static void spcinv_draw(game_spcinv_t *game)
{
	display_clear();

	draw_spaceship(game);
	draw_enemy(game);
	draw_bullets(game);

	display_update();
}

static void enemy_move(game_spcinv_t *game)
{
    uint32_t now = HAL_GetTick();

    // check if any enemy in a row will hit the edge
    for (uint8_t row = 0; row < MAX_ROW_NUMBER; row++)
    {
        bool edge_hit = false;

        for (uint8_t i = 0; i < game->enemy_count; i++)
        {
            spcinv_enemy_t *e = &game->enemy[i];

            if (!e->alive) continue;
            if ((e->y / ROW_HEIGHT) != row) continue;

            if ((e->x <= ENTITY_X_MIN && enemy_rows[row].dx < 0) ||
                (e->x >= ENTITY_X_MAX && enemy_rows[row].dx > 0))
            {
                edge_hit = true;
                break;
            }
        }

        if (edge_hit) enemy_rows[row].dx *= -1;
    }

    // move all enemies in each row together
    for (uint8_t row = 0; row < MAX_ROW_NUMBER; row++)
    {
        if (now - enemy_rows[row].last_move_ms < enemy_rows[row].movement_delay_ms) continue;

        for (uint8_t i = 0; i < game->enemy_count; i++)
        {
            spcinv_enemy_t *e = &game->enemy[i];

            if (!e->alive) continue;
            if ((e->y / ROW_HEIGHT) != row) continue;

            e->x += enemy_rows[row].dx;

            if (e->x < ENTITY_X_MIN) e->x = ENTITY_X_MIN;
            else if (e->x > ENTITY_X_MAX) e->x = ENTITY_X_MAX;
        }

        enemy_rows[row].last_move_ms = now;
    }
}

static void enemy_fire(game_spcinv_t *game)
{
	uint32_t now = HAL_GetTick();

	for (uint8_t i = 0; i < game->enemy_count; i++)
	{
		spcinv_enemy_t *e = &game->enemy[i];
		if (!e->alive) continue;

		if (!e->bullet.active && now - e->last_bullet_ms > e->bullet_delay_ms)
		{
			if (rand() % 3 == 0) continue;

			e->bullet.x = e->x + (ENTITY_WIDTH_PX / 2);
			e->bullet.y = e->y + ENTITY_HEIGHT_PX;
			e->bullet.active = true;
			e->last_bullet_ms = now;
		}
	}
}

static void spaceship_move(game_spcinv_t *game)
{
	joystick_direction_t direction = joystick_direction();

	switch(direction)
	{
	case DIRECTION_LEFT:
		if (game->player.x > SPACESHIP_X_MIN) game->player.x -= SPACESHIP_SPEED;
		if (game->player.x < SPACESHIP_X_MIN) game->player.x = SPACESHIP_X_MIN;
		break;

	case DIRECTION_RIGHT:
		if (game->player.x < SPACESHIP_X_MAX) game->player.x += SPACESHIP_SPEED;
		if (game->player.x > SPACESHIP_X_MAX) game->player.x = SPACESHIP_X_MAX;
		break;

	default:
		break;
	}
}

static void spaceship_fire(game_spcinv_t *game)
{
	if (is_button_pressed() && !game->player.bullet.active)
	{
		game->player.bullet.x = (game->player.x + ((SPACESHIP_WIDTH_PX + GAME_GRID) / 2));
		game->player.bullet.y = SPACESHIP_Y;
		game->player.bullet.dy = -SPACESHIP_BULLET_SPEED;
		game->player.bullet.active = true;
	}
}

static bool is_bitmap_pixel_set_16x8(const uint8_t *bitmap, uint8_t x, uint8_t y)
{
	if (x >= 16 || y >= 8) return false;

	uint8_t byte_index = ((y * 2) + (x / 8));
	uint8_t bit_index = (7 - (x % 8));			// leftmost pixel is bit 7

	return (bitmap[byte_index] >> bit_index) & 0x01;
}

static bool is_bitmap_pixel_set_8x8(const uint8_t *bitmap, uint8_t x, uint8_t y)
{
	if (x >= 8 || y >= 8) return false;

	uint8_t byte_index = y;
	uint8_t bit_index = (7 - (x % 8));	// leftmost pixel is bit 7

	return (bitmap[byte_index] >> bit_index) & 0x01;
}

static void move_bullets(game_spcinv_t *game)
{
	// move spaceship bullet
	if (game->player.bullet.active)
	{
		game->player.bullet.y += game->player.bullet.dy;
		if (game->player.bullet.y <= 0 || game->player.bullet.y >= DISPLAY_HEIGHT) game->player.bullet.active = false;
	}


	for (uint8_t i = 0; i < game->enemy_count; i++)
	{
		spcinv_enemy_t *e = &game->enemy[i];

		// player bullet collision with enemy
		if (e->alive && game->player.bullet.active)
		{
			for (uint8_t dx = 0; dx < GAME_GRID; dx++)
			{
				for (uint8_t dy = 0; dy < GAME_GRID; dy++)
				{
					uint8_t bx = game->player.bullet.x + dx;
					uint8_t by = game->player.bullet.y + dy;

					if (bx >= e->x && bx < e->x + ENTITY_WIDTH_PX &&
						by >= e->y && by < e->y + ENTITY_HEIGHT_PX)
					{
						uint8_t rel_x = bx - e->x;
						uint8_t rel_y = by - e->y;

						if (is_bitmap_pixel_set_16x8(enemy_bitmap[e->type], rel_x, rel_y))
						{
							game->player.bullet.active = false;
							e->life--;

							if (e->life == 0)
							{
								game_runtime.score += e->points;
								e->alive = false;
							}
							goto spaceship_bullet_done; // exit both loops
						}
					}
				}
			}
			spaceship_bullet_done:;
		}

		// enemy bullet collision with spaceship
		if (e->bullet.active)
		{
			for (uint8_t dx = 0; dx < GAME_GRID; dx++)
			{
				for (uint8_t dy = 0; dy < GAME_GRID; dy++)
				{
					uint8_t bx = e->bullet.x + dx;
					uint8_t by = e->bullet.y + dy;

					if (bx >= game->player.x && bx < game->player.x + SPACESHIP_WIDTH_PX &&
						by >= SPACESHIP_Y && by < SPACESHIP_Y + SPACESHIP_HEIGHT_PX)
					{
						uint8_t rel_x = bx - game->player.x;
						uint8_t rel_y = by - SPACESHIP_Y;

						if (is_bitmap_pixel_set_8x8(spaceship_bitmap, rel_x, rel_y))
						{
							e->bullet.active = false;
							if (game_lose_life()) spaceship_respawn(game);
							goto enemy_bullet_done; // exit both loops
						}
					}
				}
			}
			enemy_bullet_done:;
		}

		// move enemy bullets
		e->bullet.y += e->bullet.dy;
		if (e->bullet.y >= DISPLAY_HEIGHT) e->bullet.active = false;
	}
}

static void spcinv_update(game_spcinv_t *game)
{
	if (is_joystick_pressed()) game_pause();
	if (game_over_flag || game_win_flag || current_menu_state != MENU_PLAYING) return;

	spcinv_draw(game);

	spaceship_move(game);
	enemy_move(game);
	spaceship_fire(game);
	enemy_fire(game);

	move_bullets(game);

	clear_enemy_count(game);

	if (game->enemy_count == 0) {
		game_update_level();
	    setup_level(game);
	}

	HAL_Delay(game_get_delay_ms());
}
