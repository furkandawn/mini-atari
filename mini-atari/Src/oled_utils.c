/*
 * oled_utils.c
 *
 *  Created on: May 4, 2025
 *      Author: furkandawn
 */


#include "oled_utils.h"

// includes start

// include ssd1306 library
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <string.h>
#include <stdint.h>

// includes end


// draw horizontal centered string
void oled_draw_h_centered(const char* str, SSD1306_Font_t Font, uint8_t y, SSD1306_COLOR color)
{
	uint16_t string_pixel_width = strlen(str) * Font.width;

	uint8_t x = (SSD1306_WIDTH - string_pixel_width) / 2;

	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font, color);
}

void join_menu_items(const char* items[], uint8_t item_count, char* output_buffer, size_t buffer_size)
{
	output_buffer[0] = '\0';

	for (uint8_t i = 0; i < item_count; i++)
	{
		strncat(output_buffer, items[i], buffer_size - strlen(output_buffer) - 1);
		if (i < item_count - 1)
		{
			strncat(output_buffer, " ", buffer_size - strlen(output_buffer) - 1);
		}
	}
}


// draw horizontal centered multiple strings
void oled_draw_h_section(const char* str, SSD1306_Font_t Font, uint8_t y, SSD1306_COLOR color, uint8_t word_count)
{
	uint8_t words[word_count][2];  // { (string_length, string_start_position) }

	uint8_t str_index = 0;
	for (uint8_t i = 0; i < word_count; i++)
	{
		uint8_t str_len = 0;

		while (str[str_index + str_len] != ' ' && str[str_index + str_len] != '\0')
		{
			str_len++;
		}

		// assign words and their lengths
		words[i][0] = str_len;
		words[i][1] = str_index;

		while (str[str_index + str_len] == ' ') // skip gaps
		{
			str_len++;
		}

		str_index += str_len;
	}

	uint8_t section_width = SSD1306_WIDTH / word_count;
	for (uint8_t i = 0; i < word_count; i++)
	{
		uint8_t word_len = words[i][0];
		uint8_t word_start = words[i][1];
		uint8_t word_pixel_width = words[i][0] * Font.width;

		uint8_t center_x = i * section_width + section_width / 2;
		uint8_t word_start_x = center_x - word_pixel_width / 2;

		// draw word(s)
		ssd1306_SetCursor(word_start_x, y);
		for (uint8_t c = 0; c < word_len; c++)
		{
			ssd1306_WriteChar(str[word_start + c], Font, color);
		}
	}
}

// draw and highlight horizontal menu
void oled_draw_h_highlight(const char* str, SSD1306_Font_t Font, uint8_t y, SSD1306_COLOR color, uint8_t *current_index, uint8_t max_items)
{
	uint8_t words[max_items][2];  // { (string_length, string_start_position) }

	uint8_t str_index = 0;
	for (uint8_t i = 0; i < max_items; i++)
	{
		uint8_t str_len = 0;

		while (str[str_index + str_len] != ' ' && str[str_index + str_len] != '\0')
		{
			str_len++;
		}

		// assign words and their lengths
		words[i][0] = str_len;
		words[i][1] = str_index;

		while (str[str_index + str_len] == ' ') // skip gaps
		{
			str_len++;
		}

		str_index += str_len;
	}

	uint8_t section_width = SSD1306_WIDTH / max_items;
	for (uint8_t i = 0; i < max_items; i++)
	{
		uint8_t word_len = words[i][0];
		uint8_t word_start = words[i][1];
		uint8_t word_pixel_width = words[i][0] * Font.width;

		uint8_t center_x = i * section_width + section_width / 2;
		uint8_t word_start_x = center_x - word_pixel_width / 2;

		// draw word(s)
		ssd1306_SetCursor(word_start_x, y);
		for (uint8_t c = 0; c < word_len; c++)
		{
			ssd1306_WriteChar(str[word_start + c], Font, color);
		}

		// highlight selected
		if (i == *current_index)
		{
			ssd1306_SetCursor(center_x, y - Font.height);
			ssd1306_WriteString("V", Font, color);
			ssd1306_Line(word_start_x, y + Font.height, word_start_x + word_pixel_width, y + Font.height, color);
		}
	}
}
