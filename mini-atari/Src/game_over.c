/*
 * game_over.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "game_over.h"
#include <stdio.h>
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "game_menu.h"
#include "game_level.h"

static const char *gameover_menu_items[GAMEOVER_COUNT] =
{
		"Play Again",
		"Main Menu",
		"Save Score"
};

game_over_action_t current_gameover_action = GAMEOVER_PLAY_AGAIN;

void game_over(void)
{
	game_over_menu_navigate();
}

void game_over_draw_animated(void)
{
	current_menu_state = MENU_GAMEOVER;
	ssd1306_Fill(Black);
	ssd1306_SetCursor(9, 23);
	ssd1306_WriteString("GAME OVER!", Font_11x18, White);
	ssd1306_UpdateScreen();
	HAL_Delay(2000);

	ssd1306_Fill(Black);
	ssd1306_SetCursor(8, 5);
	ssd1306_WriteString(">> YOUR STATS <<", Font_7x10, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("SCORE : ", Font_6x8, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%d", game_get_score());
	ssd1306_SetCursor(48, 20);
	ssd1306_WriteString(buffer, Font_6x8, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1500);
}

static void game_over_draw(uint8_t current_gameover_action)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(8, 5);
	ssd1306_WriteString(">> YOUR STATS <<", Font_7x10, White);

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("SCORE : ", Font_6x8, White);
	char buffer[8];
	snprintf(buffer, sizeof(buffer), "%d", game_get_score());
	ssd1306_SetCursor(48, 20);
	ssd1306_WriteString(buffer, Font_6x8, White);

	for (uint8_t i = 0; i < GAMEOVER_COUNT; i++)
	    {
	        if (i == current_gameover_action)
	        { // Highlight selected item
	            ssd1306_SetCursor(15, (i * 10) + 35);
	            ssd1306_WriteString("->", Font_6x8, White);
	            ssd1306_SetCursor(30, (i * 10) + 35);
	        }
	        else
	        {
	        	ssd1306_SetCursor(40, (i * 10) + 35);
	        }

	        ssd1306_WriteString(gameover_menu_items[i], Font_6x8, White);
	    }

	    ssd1306_UpdateScreen(); // Refresh OLED screen
}

void game_over_menu_navigate(void)
{
	navigate_menu_up_down(&current_gameover_action, GAMEOVER_COUNT, game_over_draw);
}
