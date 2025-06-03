/*
 * game_space_invaders.c
 *
 *  Created on: May 31, 2025
 *      Author: furkandawn
 */


#include "game_space_invaders.h"

// ===== Includes ===== //
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
#define GAME_GRID 3						// not supported for now
#define ENTITY_WIDTH_PX 16
#define ENTITY_HEIGHT_PX 8
#define ENTITY_BITMAP_SIZE (ENTITY_WIDTH_PX * ENTITY_HEIGHT_PX / 8)
#define ROW_HEIGHT (4 + ENTITY_HEIGHT_PX)
#define ENTITY_X_MIN GAME_GRID
#define ENTITY_X_MAX (DISPLAY_WIDTH - ENTITY_WIDTH_PX - GAME_GRID)
#define MAX_ROW_NUMBER 5
#define ENTITY_BULLET_SPEED GAME_GRID

// ----- Spaceship configuration ----- //
#define SPACESHIP_Y (DISPLAY_HEIGHT - (2 * ENTITY_HEIGHT_PX))
#define SPACESHIP_SPEED GAME_GRID

// ----- Enemy configuration ----- //
typedef struct {
    int16_t dx;
    uint32_t last_move_ms;
    uint32_t movement_delay_ms;
} enemy_row_state_t;

static const spcinv_enemy_t enemy_config[SPCINV_ENEMY_TYPE_COUNT] =
{
		[SPCINV_SQUID] =
		{
				.x = 0,
				.y = 0,
				.dx = GAME_GRID,
				.points = 10,
				.life = 1,
				.alive = true,
				.horde_number = 7,
				.movement_delay_ms = 1000,
				.last_move_ms = 0,
				.bullet_delay_ms = 7000,
				.last_bullet_ms = 2000,
				.type = SPCINV_SQUID
		},

		[SPCINV_CRAB] =
		{
				.x = 0,
				.y = 0,
				.dx = GAME_GRID,
				.points = 30,
				.life = 2,
				.alive = true,
				.horde_number = 5,
				.movement_delay_ms = 700,
				.last_move_ms = 0,
				.bullet_delay_ms = 6000,
				.last_bullet_ms = 2000,
				.type = SPCINV_CRAB
		},

		[SPCINV_OCTOPUS] =
		{
				.x = 0,
				.y = 0,
				.dx = GAME_GRID,
				.points = 50,
				.life = 3,
				.alive = true,
				.horde_number = 5,
				.movement_delay_ms = 400,
				.last_move_ms = 0,
				.bullet_delay_ms = 5000,
				.last_bullet_ms = 2000,
				.type = SPCINV_OCTOPUS
		},

		[SPCINV_UFO] =
		{
				.x = ((DISPLAY_WIDTH - ENTITY_WIDTH_PX) / 2),
				.y = 0,
				.dx = (GAME_GRID * 2),
				.points = 9999,
				.life = 10,
				.alive = true,
				.horde_number = 1,
				.movement_delay_ms = 200,
				.last_move_ms = 0,
				.bullet_delay_ms = 1500,
				.last_bullet_ms = 2000,
				.type = SPCINV_UFO
		}
};

// ----- Bitmap Size Definitions ----- //
static const uint8_t spaceship_bitmap[ENTITY_BITMAP_SIZE] =
{
		0x00, 0x08,	// first row
		0x00, 0x08,
		0x00, 0x1C,
		0x00, 0x36,
		0x00, 0x7F,
		0x00, 0x49,
		0x00, 0x1C,
		0x00, 0x14	// last row
};

static const uint8_t enemy_bitmap[SPCINV_ENEMY_TYPE_COUNT][ENTITY_BITMAP_SIZE] =
{
		[SPCINV_SQUID] =
		{
				0x00, 0x18,	// first row
				0x00, 0x3C,
				0x00, 0x7E,
				0x00, 0xDB,
				0x00, 0xFF,
				0x00, 0x24,
				0x00, 0x5A,
				0x00, 0xA5	// last row
		},

		[SPCINV_CRAB] =
		{
				0x01, 0x04,	// first row
				0x00, 0x88,
				0x01, 0xFC,
				0x03, 0x76,
				0x07, 0xFF,
				0x05, 0xFD,
				0x05, 0x05,
				0x00, 0xD8	// last row
		},

		[SPCINV_OCTOPUS] =
		{
				0x00, 0xF0,	// first row
				0x07, 0xFE,
				0x0F, 0xFF,
				0x0E, 0x67,
				0x0F, 0xFF,
				0x01, 0x98,
				0x03, 0x6C,
				0x0C, 0x03	// last row
		},

		[SPCINV_UFO] =
		{
				0x07, 0xE0,	// first row
				0x1F, 0xF8,
				0x3F, 0xFC,
				0x6D, 0xB6,
				0xFF, 0xFF,
				0x39, 0x9C,
				0x10, 0x08,
				0x00, 0x00	// last row
		}
};

// ===== Static File-Private Variables ===== //
static enemy_row_state_t enemy_rows[MAX_ROW_NUMBER];
static bool is_row_full[MAX_ROW_NUMBER] = {0};

// ===== Public Global Variables ===== //

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
static bool is_bitmap_pixel_set(const uint8_t *bitmap, uint8_t x, uint8_t y);
static void move_bullets(game_spcinv_t *game);

// ===== Public API Function Definitions ===== //
void game_spcinv(game_spcinv_t *game)
{
	spcinv_init(game);

	while (!game_over && current_menu_state == MENU_PLAYING)
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
		game_win();
		game_over = true;
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
		if (!e->alive) continue;

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
	display_draw_bitmap(game->player.x, SPACESHIP_Y, spaceship_bitmap, ENTITY_WIDTH_PX, ENTITY_HEIGHT_PX, display_color_white);
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
			if (rand() % 5 == 0) continue;

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
		if (game->player.x > ENTITY_X_MIN) game->player.x -= SPACESHIP_SPEED;
		if (game->player.x < ENTITY_X_MIN) game->player.x = ENTITY_X_MIN;
		break;

	case DIRECTION_RIGHT:
		if (game->player.x < ENTITY_X_MAX) game->player.x += SPACESHIP_SPEED;
		if (game->player.x > ENTITY_X_MAX) game->player.x = ENTITY_X_MAX;
		break;

	default:
		break;
	}
}

static void spaceship_fire(game_spcinv_t *game)
{
	if (is_button_pressed() && !game->player.bullet.active)
	{
		game->player.bullet.x = ((2 * GAME_GRID) + game->player.x + ENTITY_WIDTH_PX / 2);
		game->player.bullet.y = SPACESHIP_Y;
		game->player.bullet.dy = -ENTITY_BULLET_SPEED;
		game->player.bullet.active = true;
	}
}

static bool is_bitmap_pixel_set(const uint8_t *bitmap, uint8_t x, uint8_t y)
{
	uint8_t byte_index = ((y * 2) + (x / 8));
	uint8_t bit_index = (7 - (x % 8));

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

						if (is_bitmap_pixel_set(enemy_bitmap[e->type], rel_x, rel_y))
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

					if (bx >= game->player.x && bx < game->player.x + ENTITY_WIDTH_PX &&
						by >= SPACESHIP_Y && by < SPACESHIP_Y + ENTITY_HEIGHT_PX)
					{
						uint8_t rel_x = bx - game->player.x;
						uint8_t rel_y = by - SPACESHIP_Y;

						if (is_bitmap_pixel_set(spaceship_bitmap, rel_x, rel_y))
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
	if (game_over || current_menu_state != MENU_PLAYING) return;

	spaceship_move(game);
	enemy_move(game);
	spaceship_fire(game);
	enemy_fire(game);

	move_bullets(game);

	spcinv_draw(game);

	clear_enemy_count(game);

	if (game->enemy_count == 0) {
		game_update_level();
	    setup_level(game);
	}

	HAL_Delay(game_get_delay_ms());
}
