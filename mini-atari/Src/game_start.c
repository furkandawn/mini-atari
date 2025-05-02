/*
 * game_start.c
 *
 *  Created on: Apr 25, 2025
 *      Author: furkandawn
 */


#include "game_start.h"

// include ssd1306 OLED Display
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <stdio.h>
#include "game_level.h"


void game_start(void)
{
	game_level_reset();

	ssd1306_Fill(Black);
	ssd1306_UpdateScreen();
	HAL_Delay(200);

	for (uint8_t i = COUNTDOWN; i > 0; i--)
	{
		char buffer[4];
		sprintf(buffer, "%d", i);
		ssd1306_SetCursor(54, 24);
		ssd1306_WriteString(buffer, Font_16x24, White);
		ssd1306_UpdateScreen();
		ssd1306_Fill(Black);
		HAL_Delay(500);
	}
}
