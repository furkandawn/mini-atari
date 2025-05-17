/*
 * menu_save.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_save.h"

// ----->> includes start

// include OLED Display library
#include "oled_utils.h"

// include mini-atari libraries
#include "menu_main.h"
#include "game_score.h"
#include "joystick.h"
#include "menu_logic.h"
#include "game_leaderboard.h"

// include other
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// includes end <<-----

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
	oled_clear(); // clears the OLED screen

	oled_draw_horizontal_menu(menu_save_items, oled_font_6x8, 55, oled_color_white, &current_menu_save_action, SAVE_COUNT);

	// draw the upper side
	uint8_t rect_x = 20;
	uint8_t rect_y = 20;
	oled_draw_rectangle(rect_x, 0, BOARD_WIDTH - rect_x, rect_y, oled_color_white);
	oled_write_horizontal_string(current_menu_save_name, oled_font_7x10, 5, oled_color_white);

	char left = ((current_menu_save_letter + ALPHABET_COUNT - 1) % ALPHABET_COUNT)  + 'A';
	char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
	char right = ((current_menu_save_letter + 1) % ALPHABET_COUNT)  + 'A';

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%c  <     >  %c", left, right);
	oled_write_horizontal_string(buffer, oled_font_6x8, 32, oled_color_white);

	char mid_buffer[2] = { mid, '\0'};
	oled_write_horizontal_string(mid_buffer, oled_font_16x24, 25, oled_color_white);
	// end of drawing upper side

	oled_update(); // Refreshes the OLED screen
}

static void draw_menu_save_alphabet(uint8_t current_menu_save_letter)
{
	oled_clear(); // clears the OLED screen

	uint8_t rect_x = 20;
	uint8_t rect_y = 20;
	oled_draw_rectangle(rect_x, 0, BOARD_WIDTH - rect_x, rect_y, oled_color_white);
	oled_write_horizontal_string(current_menu_save_name, oled_font_7x10, 5, oled_color_white);

	char left = ((current_menu_save_letter + ALPHABET_COUNT - 1) % ALPHABET_COUNT)  + 'A';
	char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
	char right = ((current_menu_save_letter + 1) % ALPHABET_COUNT)  + 'A';

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%c  <     >  %c", left, right);
	oled_write_horizontal_string(buffer, oled_font_6x8, 32, oled_color_white);

	char mid_buffer[2] = { mid, '\0'};
	oled_write_horizontal_string(mid_buffer, oled_font_16x24, 25, oled_color_white);

	// draw bottom side
	oled_draw_horizontal_menu(menu_save_items, oled_font_6x8, 55, oled_color_white, &avoid_highlight, SAVE_COUNT);
	// end of drawing bottom side

	oled_update(); // Refreshes the OLED screen
}

static void draw_not_eligible(void)
{
	oled_clear(); // clears the OLED screen

	oled_write_horizontal_string("you are not", oled_font_7x10, 0, oled_color_white);
	oled_write_horizontal_string("in top 3...", oled_font_7x10, 15, oled_color_white);

	oled_update(); // Refreshes the OLED screen

	HAL_Delay(1500);

	oled_write_horizontal_string("...YET", oled_font_16x15, 30, oled_color_white);

	oled_update();

	HAL_Delay(1000);

	oled_write_horizontal_string(">> press to exit <<", oled_font_6x8, 54, oled_color_white);

	oled_update(); // Refreshes the OLED screen
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
		oled_update(); // Refreshes the OLED screen
	}
}

static void handle_menu_save_bksp(void) // bksp stands for backspace
{
	if (current_letter_ptr > current_menu_save_name) // check if we have a letter to delete and not smt else
	{
		current_letter_ptr--;
		*current_letter_ptr = '\0'; // last character MUST be null
		oled_update(); // Refreshes the OLED screen
	}
}

static void handle_not_eligible(void)
{
	bool drawn_once = false;

	while (!joystick_is_pressed())
	{
		if (!drawn_once)
		{
			draw_not_eligible();
			drawn_once = true;
		}
	}
}

static void handle_current_menu_save_action(void)
{
	switch (current_menu_save_action)
	{
		case SAVE_SAVE:
		{
			add_leaderboard_entry(current_game_type, current_menu_save_name, game_get_score(), game_get_time_spent());
			current_menu_state = MENU_LEADERBOARD;
			break;
		}
		case SAVE_BKSP:
		{
			handle_menu_save_bksp();
			break;
		}
		case SAVE_EXIT:
		{
			current_menu_state = MENU_GAMEOVER;
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
	if (!is_score_eligible(current_game_type, game_get_score()))
	{
		handle_not_eligible();
		current_menu_state = MENU_GAMEOVER;
		return;
	}

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
