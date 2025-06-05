/*
 * game_leaderboard.c
 *
 *  Created on: May 12, 2025
 *      Author: furkandawn
 */


#include "game_leaderboard.h"

// ===== Includes ===== //
// include Display library
#include "display_interface.h"

// include mini-atari libraries
#include "menu_interface.h"

// include other
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_flash.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// ======= Macros/Constants ===== //
#define FLASH_PAGE_63_ADDRESS 0x0800FC00	// STM32F030R8T6 PAGE NUMBER 63 ADRESS, !!! CHECK YOUR OWN MICROCONTROLLER INFO
#define FLASH_LEADERBOARD_PAGE_SIZE 1024	// flash page size of microcontroller
#define LEADERBOARD_FLASH_ADRESS FLASH_PAGE_63_ADDRESS

// ===== Static File-Private Variables ===== //
// ----- //

// ===== Public Global Variables ===== //
// main data structure holding leaderboard entries for all games
leaderboard_entry_t current_leaderboard[GAME_COUNT][LEADERBOARD_COUNT] = {0};

// pointer array for UI rendering only
const char *menu_leaderboard_entries[GAME_COUNT][LEADERBOARD_TOTAL_COUNT] = {0};

// ===== Static Function Declarations ===== //
static void save_leaderboard_to_flash(void);
static void sort_leaderboard(leaderboard_entry_t *entry);
static void update_menu_leaderboard_entries(game_type_t game);

// ===== Public API Function Definitions ===== //
bool is_entry_empty(const leaderboard_entry_t *entry)
{
	const uint8_t *bytes = (const uint8_t *)entry;

	// no names - 0 scores
	if (entry->name[0] == 0xFF || entry->score == 0 || entry->time_passed == 0) return true;

	// since my structure is 16 bytes, i used uint8_t (255 max)
	for (uint8_t i = 0; i < sizeof(leaderboard_entry_t); i++)
	{
		if (bytes[i] != 0xFF)
		{
			return false;	// at least one byte is not 0xFF, then initialized
		}
	}
	return true;
}

bool is_score_eligible(game_type_t game, uint16_t score)
{
	bool result = score > current_leaderboard[game][LEADERBOARD_COUNT - 1].score;
	return result;
}

void load_leaderbord_from_flash(void)
{
	memcpy(current_leaderboard, (void *)LEADERBOARD_FLASH_ADRESS, sizeof(current_leaderboard));

	for (uint8_t g = 0; g < GAME_COUNT; g++)
	{
		// avoids not initialized data
		for (uint8_t i = 0; i < LEADERBOARD_COUNT; i++)
		{
			// checks if the page data is uninitialized
			if (is_entry_empty(&current_leaderboard[g][i]))
			{
				// sets uninitialized page data as 0, to avoid garbage
				memset(&current_leaderboard[g][i], 0, sizeof(leaderboard_entry_t));
			}
		}
		update_menu_leaderboard_entries(g);
	}
}

void add_leaderboard_entry(game_type_t game, const char *name, uint16_t score, uint16_t time_passed)
{
	if (!is_score_eligible(game, score)) return;

	leaderboard_entry_t new_entry;
	strncpy(new_entry.name, name, MAX_NAME_LENGTH);

	new_entry.name[MAX_NAME_LENGTH - 1] = '\0';
	new_entry.score = score;
	new_entry.time_passed = time_passed;

	leaderboard_entry_t *entries = current_leaderboard[game];

	entries[LEADERBOARD_COUNT - 1] = new_entry;

	sort_leaderboard(entries);

	// manually delete entry memset(&current_leaderboard[GAME_TETRIS][0], 0xFF, sizeof(leaderboard_entry_t));

	update_menu_leaderboard_entries(game);

	save_leaderboard_to_flash();
}

// ===== Static Function Definitions ===== //

// MANIPULATE LEADERBOARD STRUCTURE

static void sort_leaderboard(leaderboard_entry_t *entry)
{
	for (uint8_t i = 0; i < LEADERBOARD_COUNT - 1; i++)
	{
		for (uint8_t j = 0; j < LEADERBOARD_COUNT - i - 1; j++)
		{
			if (entry[j].score < entry[j + 1].score)
			{
				leaderboard_entry_t temp = entry[j];
				entry[j] = entry[j + 1];
				entry[j + 1] = temp;
			}
		}
	}
}

static void update_menu_leaderboard_entries(game_type_t game)
{
	for (uint8_t i = 0; i < LEADERBOARD_COUNT; i++)
	{
		menu_leaderboard_entries[game][i] = current_leaderboard[game][i].name;
	}
	menu_leaderboard_entries[game][LEADERBOARD_TOTAL_COUNT - 1] = "--EXIT--";
}


// FLASH MEMORY CONTROL

static void save_leaderboard_to_flash(void)
{
	HAL_FLASH_Unlock();

	// erase the page
	FLASH_EraseInitTypeDef erase =
	{
		.TypeErase = FLASH_TYPEERASE_PAGES,
		.PageAddress = LEADERBOARD_FLASH_ADRESS,
		.NbPages = 1
	};

	uint32_t page_error;
	if(HAL_FLASHEx_Erase(&erase, &page_error) != HAL_OK)
	{
		// error msg
		HAL_FLASH_Lock();
		return;
	}

	// add entries to the flash
	uint32_t flash_adress = LEADERBOARD_FLASH_ADRESS;

	for (uint8_t g = 0; g < GAME_COUNT; g++)
	{
		for (uint8_t i = 0; i < LEADERBOARD_COUNT; i++)
		{
			const uint8_t *data = (const uint8_t *)&current_leaderboard[g][i];

			for (uint8_t j = 0; j < sizeof(leaderboard_entry_t); j += 4)
			{
				uint32_t word;
				memcpy(&word, data + j, sizeof(word));

				if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_adress, word) != HAL_OK)
				{
					// error msg
					display_clear();
					display_write_centered_string("!= HAL_OK N 1", display_font_7x10, display_color_white);
					display_update();
					HAL_Delay(5000);
					HAL_FLASH_Lock();
					return;
				}

				if (flash_adress >= LEADERBOARD_FLASH_ADRESS + FLASH_LEADERBOARD_PAGE_SIZE)
				{
					// error msg
					display_clear();
					display_write_centered_string("REACHED PAGE END", display_font_7x10, display_color_white);
					display_update();
					HAL_Delay(5000);
					break;
					HAL_FLASH_Lock();
				}

				flash_adress += 4;
			}
		}
	}
	HAL_FLASH_Lock();
}
