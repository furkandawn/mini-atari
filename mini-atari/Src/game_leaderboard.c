/*
 * game_leaderboard.c
 *
 *  Created on: May 12, 2025
 *      Author: furkandawn
 */


#include "game_leaderboard.h"

// ----->> includes start

// include OLED Display library


// include mini-atari libraries
#include "menu_save.h"
#include "menu_main.h"
#include "menu_leaderboard.h"

// include other
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// include stm32 flash libraries
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_flash.h"

// includes end <<-----

#define FLASH_PAGE_63_ADDRESS 0x0800FC00	// STM32F030R8T6 PAGE NUMBER 63 ADRESS, !!! CHECK YOUR OWN MICROCONTROLLER INFO
#define LEADERBOARD_FLASH_ADRESS FLASH_PAGE_63_ADDRESS

static void save_leaderboard_to_flash(void);

// main data structure holding leaderboard entries for all games
leaderboard_entry_t leaderboard[GAME_COUNT][LEADERBOARD_COUNT] =
{
		0
};

// pointer array for UI rendering only
const char *menu_leaderboard_entries[GAME_COUNT][LEADERBOARD_TOTAL_COUNT] =
{
		0
};


/* LEADERBOARD ALGORITHMS */

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
		menu_leaderboard_entries[game][i] = leaderboard[game][i].name;
	}
	menu_leaderboard_entries[game][LEADERBOARD_TOTAL_COUNT - 1] = "--EXIT--";
}


// FLASH MEMORY CONTROL

bool is_entry_empty(const leaderboard_entry_t *entry)
{
	const uint8_t *bytes = (const uint8_t *)entry;

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

static void save_leaderboard_to_flash(void)
{

	__disable_irq(); // Disable interrupts temporarily

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
		HAL_FLASH_Lock();
		__enable_irq();
		return;
	}

	// add entries to the flash
	uint32_t flash_adress = LEADERBOARD_FLASH_ADRESS;

	for (uint8_t g = 0; g < GAME_COUNT; g++)
	{
		for (uint8_t i = 0; i < LEADERBOARD_COUNT; i++)
		{
			const uint8_t *data = (const uint8_t *)&leaderboard[g][i];

			for (uint8_t j = 0; j < sizeof(leaderboard_entry_t); j += 4)
			{
				uint32_t word = *(uint32_t *)(data + j);
				if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_adress, word) != HAL_OK)
				{
					HAL_FLASH_Lock();
					__enable_irq();
					return;
				}
				flash_adress += 4;
			}
		}
	}

	HAL_FLASH_Lock();

	__enable_irq(); // enables interrupts again
}


/* PUBLIC API FUNCTIONS */

void add_leaderboard_entry(game_type_t game, const char *name, uint16_t score, uint16_t time_passed)
{
	if (!is_score_eligible(game, score)) return;

	leaderboard_entry_t new_entry;
	strncpy(new_entry.name, name, MAX_NAME_LENGTH);

	new_entry.name[MAX_NAME_LENGTH - 1] = '\0';
	new_entry.score = score;
	new_entry.time_passed = time_passed;

	leaderboard_entry_t *entries = leaderboard[game];

	entries[LEADERBOARD_COUNT - 1] = new_entry;

	sort_leaderboard(entries);

	update_menu_leaderboard_entries(game);

	save_leaderboard_to_flash();
}

void load_leaderbord_from_flash(void)
{
	memcpy(leaderboard, (void *)LEADERBOARD_FLASH_ADRESS, sizeof(leaderboard));

	for (uint8_t g = 0; g < GAME_COUNT; g++)
	{
		// avoids not initialized data
		for (uint8_t i = 0; i < LEADERBOARD_COUNT; i++)
		{
			// checks if the page data is uninitialized
			if (is_entry_empty(&leaderboard[g][i]))
			{
				// sets uninitialized page data as 0, to avoid garbage
				memset(&leaderboard[g][i], 0, sizeof(leaderboard_entry_t));
			}
		}

		update_menu_leaderboard_entries(g);
	}
}


// LOGIC HANDLERS

bool is_score_eligible(game_type_t game, uint16_t score)
{
	return score > leaderboard[game][LEADERBOARD_COUNT - 1].score;
}
