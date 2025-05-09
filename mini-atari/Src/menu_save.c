/*
 * menu_save.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_save.h"

// includes start
// include ssd1306 OLED Display
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "oled_utils.h"

#include "joystick.h"
#include "menu_logic.h"
#include "game_level.h"
#include "menu_leaderboard.h"
#include "oled_utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// includes end

typedef enum
{
	A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ALPHABET_COUNT
} menu_save_alphabet_t;

static const char *menu_save_items[] =
{
		"Save",
		"Bksp",
		"Exit"
};

char current_menu_save_name[MAX_NAME_LENGTH] =
{
		'\0'
};

static char *current_letter_ptr = current_menu_save_name;

static menu_save_alphabet_t current_menu_save_letter = A;
static menu_save_action_t current_menu_save_action = SAVE_BKSP;

static bool is_letter_focused = true;

static void draw_menu_save_actions(uint8_t current_menu_save_action)
{
	ssd1306_Fill(Black);

	oled_draw_horizontal_menu(menu_save_items, Font_6x8, 55, White, &current_menu_save_action, SAVE_COUNT);

	// draw the upper side
	ssd1306_DrawRectangle(14, 0, 114, 18, White);
	oled_draw_horizontal_string(current_menu_save_name, Font_7x10, 5, White);

	char left = ((current_menu_save_letter + ALPHABET_COUNT - 1) % ALPHABET_COUNT)  + 'A';
	char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
	char right = ((current_menu_save_letter + 1) % ALPHABET_COUNT)  + 'A';

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%c  <     >  %c", left, right);
	oled_draw_horizontal_string(buffer, Font_6x8, 32, White);

	char mid_buffer[2] = { mid, '\0'};
	oled_draw_horizontal_string(mid_buffer, Font_16x24, 25, White);
	// end of drawing upper side

	ssd1306_UpdateScreen(); // Refresh OLED screen
}

static void draw_menu_save_alphabet(uint8_t current_menu_save_letter)
{
	ssd1306_Fill(Black);

	ssd1306_DrawRectangle(14, 0, 114, 18, White);
	oled_draw_horizontal_string(current_menu_save_name, Font_7x10, 5, White);

	char left = ((current_menu_save_letter + ALPHABET_COUNT - 1) % ALPHABET_COUNT)  + 'A';
	char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
	char right = ((current_menu_save_letter + 1) % ALPHABET_COUNT)  + 'A';

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%c  <     >  %c", left, right);
	oled_draw_horizontal_string(buffer, Font_6x8, 32, White);

	char mid_buffer[2] = { mid, '\0'};
	oled_draw_horizontal_string(mid_buffer, Font_16x24, 25, White);

	// draw bottom side
	oled_draw_horizontal_menu(menu_save_items, Font_6x8, 55, White, &avoid_highlight, SAVE_COUNT);
	// end of drawing bottom side

	ssd1306_UpdateScreen(); // Refresh OLED screen
}

static void navigate_menu_save_actions(void)
{
	navigate_menu_right_left(&current_menu_save_action, SAVE_COUNT, draw_menu_save_actions);
}

static void navigate_menu_save_alphabet(void)
{
	navigate_menu_right_left_loop(&current_menu_save_letter, ALPHABET_COUNT, draw_menu_save_alphabet);
}

/*
 *  Up Above contains print and navigation of menu save screen
 *  -----------------------------------------------------------------------------
 *  Down Below is the logic of menu save screen
 */

static void handle_selected_letter_input(void)
{
	if ((current_letter_ptr - current_menu_save_name) < MAX_NAME_LENGTH - 1) // avoid overflow
	{
		char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
		*current_letter_ptr = mid; // pointer & array arithmetics
		current_letter_ptr++;
		*current_letter_ptr = '\0'; // last character MUST be null
		ssd1306_UpdateScreen();
	}
}

static void handle_menu_save_bksp(void) // bksp stands for backspace
{
	if (current_letter_ptr > current_menu_save_name) // check if we have a letter to delete and not smt else
	{
		current_letter_ptr--;
		*current_letter_ptr = '\0'; // last character MUST be null
		ssd1306_UpdateScreen();
	}
}

static void handle_current_menu_save_action(void)
{
	switch (current_menu_save_action)
	{
		case SAVE_SAVE:
		{
			// leaderboard direct flash write function
			current_menu_state = MENU_MAIN;
			break;
		}
		case SAVE_BKSP:
		{
			handle_menu_save_bksp();
			break;
		}
		case SAVE_EXIT:
		{
			current_menu_state = MENU_MAIN;
			break;
		}
		default:
		{
			break;
		}
	}
}

void handle_menu_save(void)
{
	joystick_direction_t direction = joystick_direction();
	if (direction == DIRECTION_UP) is_letter_focused = true;
	if (direction == DIRECTION_DOWN) is_letter_focused = false;

	if (is_letter_focused)
	{
		navigate_menu_save_alphabet();
		if (joystick_is_pressed()) handle_selected_letter_input();
	}
	else
	{
		navigate_menu_save_actions();
		if (joystick_is_pressed()) handle_current_menu_save_action();
	}
}
