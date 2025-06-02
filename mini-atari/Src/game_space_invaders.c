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

// ======= Macros/Constants ===== //
#define GAME_GRID 2						// pixel size per game grid cell
#define ENTITY_WIDTH_PX 16
#define ENTITY_HEIGHT_PX 8
#define ENTITY_BITMAP_SIZE (ENTITY_WIDTH_PX * ENTITY_HEIGHT_PX / 8)

// ----- Enemy Types ----- //
typedef enum
{
	SQUID = 0,
	CRAB,
	OCTOPUS,
	UFO,
	ENEMY_TYPE_COUNT
} enemy_type_t;

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

static const uint8_t enemy_bitmap[ENEMY_TYPE_COUNT][ENTITY_BITMAP_SIZE] =
{
		[SQUID] =
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

		[CRAB] =
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

		[OCTOPUS] =
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

		[UFO] =
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

// ===== Public Global Variables ===== //

// ===== Static Function Declarations ===== //

// ===== Public API Function Definitions ===== //
void game_spcinv(game_spcinv_t *game)
{
	while (!game_over && current_menu_state == MENU_PLAYING)
	{

	}

	current_menu_state = MENU_GAMEOVER;
}
// ===== Static Function Definitions ===== //
static void spcinv_init(game_spcinv_t *game)
{

}
