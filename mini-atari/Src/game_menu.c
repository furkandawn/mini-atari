/*
 * game_menu.c
 *
 *  Created on: Apr 21, 2025
 *      Author: furkandawn
 */


#include "game_menu.h"

// include ssd1306 OLED Display
#include "ssd1306.h"
#include "ssd1306_fonts.h"

// include game logic
#include "game_start.h"
#include "menu_logic.h"
#include "menu_main.h"
#include "menu_selected.h"
#include "menu_paused.h"
#include "menu_save.h"
#include "menu_gameover.h"

void game_menu(void)
{
	switch (current_menu_state)
	{
	case MENU_MAIN: handle_menu_main(); break;
	case MENU_SELECTED: handle_menu_selected(); break;
	case MENU_SAVE: handle_menu_save(); break;
	case MENU_LEADERBOARD: return; break;//handle_menu_leaderboard(); break;
	case MENU_GAMEOVER: handle_menu_gameover(); break;
	default: break;
	}
}
