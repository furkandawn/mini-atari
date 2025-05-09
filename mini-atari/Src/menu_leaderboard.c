/*
 * menu_leaderboard.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_leaderboard.h"

// includes start
// include ssd1306 OLED Display
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include "joystick.h"
#include "menu_logic.h"
#include "menu_save.h"

#include <stdbool.h>
#include <stdio.h>

// includes end

leaderboard_entry_t leaderboard[LEADERBOARD_TOP_COUNT] =
{
		{"First", 0},
		{"Second", 0},
		{"Third", 0}
};
/*
static void draw_menu_leaderboard(void)
{
	ssd1306_Fill(Black);

	ssd1306_SetCursor(30, 0);
	ssd1306_WriteString("LEADERBOARD", Font_7x10, White);

	char buffer[32];
	for (uint8_t i = 0; i < LEADERBOARD_TOP_COUNT; i++)
	{
		snprintf(buffer, sizeof(buffer), "%d. %s --> %d", i + 1, leaderboard[i].name, leaderboard[i].score);
		ssd1306_SetCursor(5, 20 + (i * 10));
		ssd1306_WriteString(buffer, Font_6x8, White);
	}

	ssd1306_SetCursor(20, 45);
	ssd1306_WriteString("Press to Exit", Font_6x8, White);

	ssd1306_UpdateScreen();
}

static void set_leaderboard(uint8_t index)
{
	strncpy(leaderboard[index].name, current_menu_save_name, sizeof(leaderboard[index].name));
	// leaderboard[index].score
}
*/
