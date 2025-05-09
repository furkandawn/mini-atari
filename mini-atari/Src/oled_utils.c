/*
 * oled_utils.c
 *
 *  Created on: May 4, 2025
 *      Author: furkandawn
 */


#include "oled_utils.h"

#define START_X 20

// includes start

// include ssd1306 library
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <string.h>
#include <stdint.h>

// includes end

uint8_t avoid_highlight = 255;

// draw string at the center
void oled_draw_centered_string(char* str, SSD1306_Font_t Font, SSD1306_COLOR color)
{
	uint16_t string_pixel_width = strlen(str) * Font.width;
	uint8_t x = (SSD1306_WIDTH - string_pixel_width) / 2;

	uint16_t string_pixel_height = Font.height;
	uint8_t y = (SSD1306_HEIGHT - string_pixel_height) / 2;

	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font, color);
}


/*
 * Horizontal Centered Functions
 */

// draw horizontal centered string
void oled_draw_horizontal_string(char* str, SSD1306_Font_t Font, uint8_t y, SSD1306_COLOR color)
{
	uint16_t string_pixel_width = strlen(str) * Font.width;

	uint8_t x = (SSD1306_WIDTH - string_pixel_width) / 2;

	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(str, Font, color);
}

// draw and highlight horizontal menu
void oled_draw_horizontal_menu(const char* items[], SSD1306_Font_t Font, uint8_t y, SSD1306_COLOR color, uint8_t *current_index, uint8_t item_count)
{
	uint16_t section_width = SSD1306_WIDTH / item_count;

	for (uint8_t i = 0; i < item_count; i++)
	{
		uint8_t word_len = strlen(items[i]);
		uint8_t word_pixel_width = word_len * Font.width;

		uint8_t center_x = i * section_width + section_width / 2;
		uint8_t word_start_x = center_x - word_pixel_width / 2;

		// highlight if applicable. insert "&avoid_highlight" to avoid
		if (i == *current_index)
		{
			ssd1306_SetCursor(center_x, y - Font.height - 2);
			ssd1306_WriteString("V", Font, color);

			// background white, string black.
			ssd1306_FillRectangle(word_start_x - 2, y - 1, word_start_x + word_pixel_width + 1, y + Font.height, White);

			// draw word(s)
			ssd1306_SetCursor(word_start_x, y);
			ssd1306_WriteString(items[i], Font, Black);
		}
		else
		{
			// draw word(s)
			ssd1306_SetCursor(word_start_x, y);
			ssd1306_WriteString(items[i], Font, color);
		}
	}
}

/*
 * Vertical Centered Functions
 */

// draw and highlight vertical menu
void oled_draw_vertical_menu(const char* items[], SSD1306_Font_t Font, uint8_t start_y, SSD1306_COLOR color, uint8_t *current_index, uint8_t item_count)
{
	uint16_t line_height = Font.height + 2;

	for (uint8_t i = 0; i < item_count; i++)
	{
		uint8_t y = start_y + i * line_height;
		uint8_t string_pixel_width = strlen(items[i]) * Font.width;
		uint8_t x = (SSD1306_WIDTH - string_pixel_width) / 2;

		// highlight if applicable. insert "&avoid_highlight" to avoid
		if (i == *current_index)
		{
			// define highlight symbol ">" & "<" positions
			uint8_t left_highlight_x = x - 2 * Font.width;
			uint8_t right_highlight_x = x + string_pixel_width + Font.width;

			// background white, string black.
			ssd1306_FillRectangle(x - 2, y - 1, x + string_pixel_width + 1, y + Font.height, White);

			ssd1306_SetCursor(left_highlight_x, y);
			ssd1306_WriteString(">", Font, color);
			ssd1306_SetCursor(right_highlight_x, y);
			ssd1306_WriteString("<", Font, color);

			// draw word(s)
			ssd1306_SetCursor(x, y);
			ssd1306_WriteString(items[i], Font, Black);
		}
		else
		{
			// draw word(s)
			ssd1306_SetCursor(x, y);
			ssd1306_WriteString(items[i], Font, color);
		}
	}
}
