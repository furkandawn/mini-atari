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

#include "joystick.h"
#include "menu_logic.h"

#include "stdbool.h"

// includes end

#define MAX_NAME_LENGTH 11

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

static char menu_save_inserted_name[MAX_NAME_LENGTH] =
{
		""
};

static char *menu_save_inserted_letter = menu_save_inserted_name;

static menu_save_alphabet_t current_menu_save_letter = A;
static menu_save_action_t current_menu_save_action = SAVE_BKSP;

static bool is_letter_focused = true;

static void draw_menu_save_actions(uint8_t current_menu_save_action)
{
	ssd1306_Fill(Black);

	for (uint8_t i = 0; i < SAVE_COUNT; i++)
	{
		if (i == current_menu_save_action)  // Highlight selected item
		{
			ssd1306_SetCursor(16 + (i * 48), 47);
			ssd1306_WriteString("V", Font_6x8, White);
			ssd1306_FillRectangle(4 + (i * 48), 62, 28 + (i * 48), 63, White);
		}

		ssd1306_SetCursor(4 + (i * 48), 55);
		ssd1306_WriteString(menu_save_items[i], Font_6x8, White);
	}

	// draw the upper side without functionality

	char left = ((current_menu_save_letter + ALPHABET_COUNT - 1) % ALPHABET_COUNT)  + 'A';
	char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
	char right = ((current_menu_save_letter + 1) % ALPHABET_COUNT)  + 'A';

	ssd1306_DrawRectangle(14, 0, 114, 18, White);
	ssd1306_SetCursor(29, 5);
	ssd1306_WriteString(menu_save_inserted_name, Font_7x10, White);

	ssd1306_SetCursor(49, 30);
	ssd1306_WriteString("<", Font_6x8, White);
	ssd1306_SetCursor(81, 30);
	ssd1306_WriteString(">", Font_6x8, White);

	ssd1306_SetCursor(28, 28);
	ssd1306_WriteChar(left, Font_6x8, White);
	ssd1306_SetCursor(60, 25);
	ssd1306_WriteChar(mid, Font_16x24, White);
	ssd1306_SetCursor(101, 28);
	ssd1306_WriteChar(right, Font_6x8, White);

	// end of drawing upper side

	ssd1306_UpdateScreen(); // Refresh OLED screen
}

static void draw_menu_save_alphabet(uint8_t current_menu_save_letter)
{
	ssd1306_Fill(Black);

	ssd1306_DrawRectangle(14, 0, 114, 18, White);
	ssd1306_SetCursor(29, 5);
	ssd1306_WriteString(menu_save_inserted_name, Font_7x10, White);

	ssd1306_SetCursor(49, 30);
	ssd1306_WriteString("<", Font_6x8, White);
	ssd1306_SetCursor(81, 30);
	ssd1306_WriteString(">", Font_6x8, White);

	char left = ((current_menu_save_letter + ALPHABET_COUNT - 1) % ALPHABET_COUNT)  + 'A';
	char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
	char right = ((current_menu_save_letter + 1) % ALPHABET_COUNT)  + 'A';
	ssd1306_SetCursor(28, 28);
	ssd1306_WriteChar(left, Font_6x8, White);
	ssd1306_SetCursor(60, 25);
	ssd1306_WriteChar(mid, Font_16x24, White);
	ssd1306_SetCursor(101, 28);
	ssd1306_WriteChar(right, Font_6x8, White);

	// draw bottom side without functionality
	ssd1306_SetCursor(4 + (0 * 48), 55);
	ssd1306_WriteString(menu_save_items[0], Font_6x8, White);
	ssd1306_SetCursor(4 + (1 * 48), 55);
	ssd1306_WriteString(menu_save_items[1], Font_6x8, White);
	ssd1306_SetCursor(4 + (2 * 48), 55);
	ssd1306_WriteString(menu_save_items[2], Font_6x8, White);

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
	if (joystick_is_pressed()) // check if we pressed the button on a selected letter
	{
		if ((menu_save_inserted_letter - menu_save_inserted_name) < MAX_NAME_LENGTH - 1) // avoid overflow
		{
			char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
			*menu_save_inserted_letter = mid; // pointer & array arithmetics
			menu_save_inserted_letter++;
			*menu_save_inserted_letter = '\0'; // last character MUST be null
			ssd1306_UpdateScreen();
		}
	}
}

static void handle_menu_save_bksp(void) // bksp stands for backspace
{
	if (menu_save_inserted_letter > menu_save_inserted_name) // check if we have a letter to delete and not smt else
	{
		menu_save_inserted_letter--;
		*menu_save_inserted_letter = '\0'; // last character MUST be null
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
