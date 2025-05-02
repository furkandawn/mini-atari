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
#include "joystick.h"
#include "game_over.h"
#include <stdbool.h>
#include <stdio.h>

// include games
#include "game_snake.h"

static const char *main_menu_items[GAME_COUNT] =
{
		"Snake Game",
		"Game2",
		"Game3",
		"Game4",
		"Game5"
};

static const char *selected_menu_items[ACTION_COUNT] =
{
		"Start Game",
		"Leaderboard",
		"Main Menu"
};

static const char *paused_menu_items[PAUSED_COUNT] =
{
		"Continue",
		"Exit"
};

static game_type_t current_game_type = GAME_SNAKE;
static action_type_t current_menu_action = ACTION_START;
menu_state_t current_menu_state = MENU_MAIN;
static paused_action_t current_paused_action = PAUSED_CONTINUE;

static void handle_menu_paused(void);

static void game_menu_draw(uint8_t current_game_type)
{
	ssd1306_Fill(Black); // Clear the OLED display

    // Display game list
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString(">>> GAME MENU <<<", Font_7x10, White);

    for (uint8_t i = 0; i < GAME_COUNT; i++)
    {
        if (i == current_game_type)
        { // Highlight selected game
            ssd1306_SetCursor(0, (i * 10) + 15);
            ssd1306_WriteString("->", Font_6x8, White);
            ssd1306_SetCursor(20, (i * 10) + 15);
        }
        else
        {
            ssd1306_SetCursor(30, (i * 10) + 15);
        }

        ssd1306_WriteString(main_menu_items[i], Font_6x8, White);
    }

    ssd1306_UpdateScreen();  // Refresh OLED screen
}

static void game_menu_selected_draw(uint8_t current_menu_action)
{
	ssd1306_Fill(Black); // Clear the OLED display

	// Display action list
	char buffer[32];
	snprintf(buffer, sizeof(buffer), ">>> %s <<<", main_menu_items[current_game_type]);
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString(buffer, Font_7x10, White);

    for (uint8_t i = 0; i < ACTION_COUNT; i++)
    {
        if (i == current_menu_action)
        { // Highlight selected item
            ssd1306_SetCursor(0, (i * 10) + 15);
            ssd1306_WriteString("->", Font_6x8, White);
            ssd1306_SetCursor(20, (i * 10) + 15);
        }
        else
        {
        	ssd1306_SetCursor(30, (i * 10) + 15);
        }

        ssd1306_WriteString(selected_menu_items[i], Font_6x8, White);
    }

    ssd1306_UpdateScreen(); // Refresh OLED screen
}

void game_menu_paused_draw(uint8_t current_paused_action)
{
	ssd1306_Fill(Black); // Clear the OLED display

	ssd1306_SetCursor(16, 5);
	ssd1306_WriteString("PAUSED", Font_16x24, White);

	for (uint8_t i = 0; i < PAUSED_COUNT; i++)
	    {
	        if (i == current_paused_action)
	        { // Highlight "Continue - Exit"
	            ssd1306_SetCursor(33 + (i * 68), 30);
	            ssd1306_WriteString("|", Font_6x8, White);
	            ssd1306_SetCursor(33 + (i * 68), 38);
	            ssd1306_WriteString("V", Font_6x8, White);
	        }
	        ssd1306_SetCursor(10 + ( i * 80), 50);
	        ssd1306_WriteString(paused_menu_items[i], Font_6x8, White);
	    }

	ssd1306_UpdateScreen(); // Refresh screen
}

void navigate_menu_up_down(uint8_t *index, uint8_t maxItems, void (*draw_func)(uint8_t))
{

	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_UP && *index != 0)
	{
		(*index)--;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_DOWN && *index < maxItems - 1)
	{
		(*index)++;
		HAL_Delay(100);
	}

	draw_func(*index);
}

void navigate_menu_right_left(uint8_t *index, uint8_t maxItems, void (*draw_func)(uint8_t))
{

	joystick_direction_t direction = joystick_direction();

	if (direction == DIRECTION_LEFT && *index != 0)
	{
		(*index)--;
		HAL_Delay(100);
	}
	else if (direction == DIRECTION_RIGHT && *index < maxItems - 1)
	{
		(*index)++;
		HAL_Delay(100);
	}

	draw_func(*index);
}

static void game_menu_navigate(void)
{
	navigate_menu_up_down(&current_game_type, GAME_COUNT, game_menu_draw);
}

static void game_menu_selected_navigate(void)
{
	navigate_menu_up_down(&current_menu_action, ACTION_COUNT, game_menu_selected_draw);
}

static void game_menu_paused_navigate(void)
{
	navigate_menu_right_left(&current_paused_action, PAUSED_COUNT, game_menu_paused_draw);
}

static void game_init(void)
{
	switch(current_game_type)
	{
	case GAME_SNAKE:
	{
		game_snake_t game;
		snake_game(&game);
		break;
	}
	case GAME_2:
	{
		break;
	}
	case GAME_3:
	{
		break;
	}
	case GAME_4:
	{
		break;
	}
	case GAME_5:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

void pause_game(void)
{
	HAL_Delay(500);
	current_menu_state = MENU_PAUSED;
	while (current_menu_state == MENU_PAUSED)
	{
		handle_menu_paused();
	}
}

static void handle_current_menu_action(void)
{
	switch (current_menu_action)
	{
	case ACTION_START:
	{
		current_menu_state = MENU_PLAYING;
		game_init();
		break;
	}
	case ACTION_LEADERBOARD:
	{
		current_menu_state = ACTION_LEADERBOARD;
		// re-write this after leaderboard done.
		break;
	}
	case ACTION_BACK:
	{
		current_menu_state = MENU_MAIN;
		current_menu_action = ACTION_START;
		current_game_type = GAME_SNAKE;
		break;
	}
	default:
	{
		break;
	}
	}
}

static void handle_current_game_over_action(void)
{
	switch (current_gameover_action)
	{
	case GAMEOVER_PLAY_AGAIN:
	{
		current_menu_state = MENU_PLAYING;
		game_init();
		break;
	}
	case GAMEOVER_MAIN_MENU:
	{
		current_menu_state = MENU_MAIN;
		current_game_type = GAME_SNAKE;
		current_menu_action = ACTION_START;
		current_gameover_action = GAMEOVER_PLAY_AGAIN;
		break;
	}
	case GAMEOVER_SAVE:
	{
		// re-write this after leaderboard done.
		return;
		break;
	}
	default:
	{
	break;
	}
	}
}

static void handle_current_menu_paused_action(void)
{
	switch (current_paused_action)
	{
	case PAUSED_CONTINUE:
	{
		current_menu_state = MENU_PLAYING;
		break;
	}
	case PAUSED_EXIT:
	{
		current_menu_state = MENU_GAMEOVER;
		current_paused_action = PAUSED_CONTINUE;
		break;
	}
	default: break;
	}
}

static void handle_menu_main(void)
{
	game_menu_navigate();
	if (joystick_is_pressed()) current_menu_state = MENU_SELECTED;
}

static void handle_menu_selected(void)
{
	game_menu_selected_navigate();
	if (joystick_is_pressed()) handle_current_menu_action();
}

static void handle_menu_leaderboard(void)
{
	// re-write this function when leaderboard is done.
	return;
}

static void handle_menu_paused(void)
{
	game_menu_paused_navigate();
	if (joystick_is_pressed()) handle_current_menu_paused_action();
}

static void handle_menu_gameover(void)
{
	game_over_menu_navigate();
	if(joystick_is_pressed()) handle_current_game_over_action();
}

void game_menu(void)
{
	switch (current_menu_state)
	{
	case MENU_MAIN: handle_menu_main(); break;
	case MENU_SELECTED: handle_menu_selected(); break;
	case MENU_LEADERBOARD: handle_menu_leaderboard(); break;
	case MENU_GAMEOVER: handle_menu_gameover(); break;
	default: break;
	}
}
