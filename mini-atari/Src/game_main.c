/*
 * game_main.c
 *
 *  Created on: Apr 21, 2025
 *      Author: furkandawn
 */


#include "game_main.h"

// ===== Includes ===== //
// include mini-atari libraries
#include "menu_interface.h"
#include "menu_logic.h"
#include "menu_main.h"
#include "menu_selected.h"
#include "menu_playing.h"
#include "menu_gameover.h"
#include "menu_save.h"
#include "menu_leaderboard.h"
#include "game_leaderboard.h"

// include other
#include <stdbool.h>

// ===== Static File-Private Variables ===== //
static bool leaderboard_loaded = false;

// ===== Public API Function Definitions ===== //
void game_main(void)
{
	if (!leaderboard_loaded)
	{
		load_leaderbord_from_flash();
		leaderboard_loaded = true;
	}

	switch (current_menu_state)
	{
	case MENU_MAIN: handle_menu_main(); break;
	case MENU_SELECTED: handle_menu_selected(); break;
	case MENU_PLAYING: handle_menu_playing(); break;
	case MENU_SAVE: handle_menu_save(); break;
	case MENU_LEADERBOARD: handle_menu_leaderboard(); break;
	case MENU_GAMEOVER: handle_menu_gameover(); break;
	default: break;
	}
}
