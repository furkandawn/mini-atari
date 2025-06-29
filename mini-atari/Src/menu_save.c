/*
 * menu_save.c
 *
 *  Created on: May 3, 2025
 *      Author: furkandawn
 */


#include "menu_save.h"

// ===== Includes ===== //
// include display library
#include "display_interface.h"

// include mini-atari libraries
#include "menu_interface.h"
#include "menu_logic.h"
#include "joystick.h"
#include "game_leaderboard.h"
#include "game_runtime.h"

// include other
#include "stm32f0xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// ======= Macros/Constants ===== //
// ----- //

// ===== Static File-Private Variables ===== //
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

static char current_name[MAX_NAME_LENGTH] =
{
		'\0'
};

static char *current_letter_ptr = current_name;

static menu_save_alphabet_t current_menu_save_letter = A;
static save_action_t current_menu_save_action = SAVE_BKSP;
static bool is_letter_focused = true;

// ===== Public Global Variables ===== //
// ----- //

// ===== Static Function Declarations ===== //
static void handle_not_eligible(void);
static void navigate_alphabet(void);
static void navigate_save_action(void);
static void handle_selected_letter_input(void);
static void handle_current_save_action(void);

// ===== Public API Function Definitions ===== //
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
		navigate_alphabet();
		if (is_joystick_pressed() || is_button_pressed())
		{
			reset_navigate_menu_draw_flags();
			handle_selected_letter_input();
		}
	}
	else
	{
		navigate_save_action();
		if (is_joystick_pressed() || is_button_pressed())
		{
			reset_navigate_menu_draw_flags();
			handle_current_save_action();
		}
	}
}

// ===== Static Function Definitions ===== //
// Draw Functions

static void draw_menu_save_actions(uint8_t current_menu_save_action)
{
	display_clear(); // clears the OLED screen

	display_draw_horizontal_menu(menu_save_items, 55, &current_menu_save_action, SAVE_COUNT, display_font_6x8, display_color_white);

	// draw the upper side
	uint8_t rect_x = 20;
	uint8_t rect_y = 20;
	display_draw_rectangle(rect_x, 0, DISPLAY_WIDTH - rect_x, rect_y, display_color_white);
	display_write_horizontal_string(current_name, 5, display_font_7x10, display_color_white);

	char left = ((current_menu_save_letter + ALPHABET_COUNT - 1) % ALPHABET_COUNT)  + 'A';
	char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
	char right = ((current_menu_save_letter + 1) % ALPHABET_COUNT)  + 'A';

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%c  <     >  %c", left, right);
	display_write_horizontal_string(buffer, 32, display_font_6x8, display_color_white);

	char mid_buffer[2] = { mid, '\0'};
	display_write_horizontal_string(mid_buffer, 25, display_font_16x24, display_color_white);
	// end of drawing upper side

	display_update(); // Refreshes the OLED screen
}

static void draw_menu_save_alphabet(uint8_t current_menu_save_letter)
{
	display_clear(); // clears the OLED screen

	uint8_t rect_x = 20;
	uint8_t rect_y = 20;
	display_draw_rectangle(rect_x, 0, DISPLAY_WIDTH - rect_x, rect_y, display_color_white);
	display_write_horizontal_string(current_name, 5, display_font_7x10, display_color_white);

	char left = ((current_menu_save_letter + ALPHABET_COUNT - 1) % ALPHABET_COUNT)  + 'A';
	char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
	char right = ((current_menu_save_letter + 1) % ALPHABET_COUNT)  + 'A';

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%c  <     >  %c", left, right);
	display_write_horizontal_string(buffer, 32, display_font_6x8, display_color_white);

	char mid_buffer[2] = { mid, '\0'};
	display_write_horizontal_string(mid_buffer, 25, display_font_16x24, display_color_white);

	// draw bottom side
	display_draw_horizontal_menu(menu_save_items, 55, &avoid_highlight, SAVE_COUNT, display_font_6x8, display_color_white);
	// end of drawing bottom side

	display_update(); // Refreshes the OLED screen
}

static void draw_not_eligible(void)
{
	input_enabled = false;

	display_clear(); // clears the OLED screen

	display_write_horizontal_string("you are not", 0, display_font_7x10, display_color_white);
	display_write_horizontal_string("in top 3...", 15, display_font_7x10, display_color_white);

	display_update(); // Refreshes the OLED screen

	HAL_Delay(1500);

	display_write_horizontal_string("...YET", 30, display_font_11x18, display_color_white);

	display_update();

	HAL_Delay(1000);

	display_write_horizontal_string(">> press to exit <<", 54, display_font_6x8, display_color_white);

	display_update(); // Refreshes the OLED screen

	input_enabled = true;
}

static void navigate_save_action(void)
{
	navigate_menu_right_left(&current_menu_save_action, SAVE_COUNT, draw_menu_save_actions);
}

static void navigate_alphabet(void)
{
	navigate_menu_right_left_loop(&current_menu_save_letter, ALPHABET_COUNT, draw_menu_save_alphabet);
}


// Logic Handlers

static void handle_selected_letter_input(void)
{
	if ((current_letter_ptr - current_name) < MAX_NAME_LENGTH - 1) // avoid overflow
	{
		char mid = ((current_menu_save_letter) % ALPHABET_COUNT) + 'A';
		*current_letter_ptr = mid; // pointer & array arithmetics
		current_letter_ptr++;
		*current_letter_ptr = '\0'; // last character MUST be null
		display_update(); // Refreshes the OLED screen
	}
}

static void handle_menu_save_bksp(void) // bksp stands for backspace
{
	if (current_letter_ptr > current_name) // check if we have a letter to delete and not smt else
	{
		current_letter_ptr--;
		*current_letter_ptr = '\0'; // last character MUST be null
		display_update(); // Refreshes the OLED screen
	}
}

static void handle_not_eligible(void)
{
	bool drawn_once = false;

	while (!is_joystick_pressed() && !is_button_pressed())
	{
		if (!drawn_once)
		{
			draw_not_eligible();
			drawn_once = true;
		}
	}
}

static void handle_current_save_action(void)
{
	switch (current_menu_save_action)
	{
		case SAVE_SAVE:
		{
			add_leaderboard_entry(current_game_type, current_name, game_get_score(), game_get_time_spent());
			current_menu_save_action = SAVE_BKSP;
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
			current_menu_save_action = SAVE_BKSP;
			current_menu_state = MENU_GAMEOVER;
			break;
		}
		default:
		{
			break;
		}
	}
}
