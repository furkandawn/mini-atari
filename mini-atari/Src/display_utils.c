/*
 * display_utils.c
 *
 *  Created on: May 23, 2025
 *      Author: furkandawn
 */


#include "display_utils.h"

#include "display_config.h"
#include <string.h>

// implement your own display backend with the same structure in display_ssd1306.h/c
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
#include "display_ssd1306.h"
#endif

uint8_t avoid_highlight = 255; // use this in functions to avoid highlighting & navigation.

/*
 * Get font width/height
 */
uint8_t font_width(display_font_t font)
{
	switch(font)
	{
	case display_font_6x8:		return 6;
	case display_font_7x10:		return 7;
	case display_font_11x18:	return 11;
	case display_font_16x15:	return 16;
	case display_font_16x24:	return 16;
	case display_font_16x26:	return 16;
	default: return 6;
	}
}

uint8_t font_height(display_font_t font)
{
	switch(font)
	{
	case display_font_6x8:		return 8;
	case display_font_7x10:		return 10;
	case display_font_11x18:	return 18;
	case display_font_16x15:	return 15;
	case display_font_16x24:	return 24;
	case display_font_16x26:	return 26;
	default: return 8;
	}
}

/* General purpose functions */

/*
 * Init Display screen
 */
void display_init(void)
{
	backend_display_init();
}

/*
 * Clears the display by filling it black
 */
void display_clear(void)
{
	backend_display_clear();
}

/*
 * Pushes the current buffer to the display
 */
void display_update(void)
{
	backend_display_update();
}

/*
 * Sets the cursor at requested position
 */
void display_set_cursor(uint8_t x, uint8_t y)
{
	backend_display_set_cursor(x, y);
}

/*
 * Writes a string at current cursor position
 */
char display_write_string(const char* str, display_font_t font, display_color_t color)
{
	return backend_display_write_string(str, font, color);
}

/*
 * Draws an unfilled rectangle
 */
void display_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, display_color_t color)
{
	backend_display_draw_rectangle(x1, y1, x2, y2, color);
}

/*
 * Draws a filled rectangle
 */
void display_fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, display_color_t color)
{
	backend_display_fill_rectangle(x1, y1, x2, y2, color);
}

/*
 * Draws a filled square
 */
void display_fill_square(uint8_t x, uint8_t y, uint8_t size, display_color_t color)
{
	backend_display_fill_square(x, y, size, color);
}

/*
 * Draws a bitmap
 */
void display_draw_bitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, display_color_t color)
{
	backend_display_draw_bitmap(x, y, bitmap, w, h, color);
}


/* Centered String & Menu Functions */

/*
 * Draws the string at the center of screen
 */
void display_write_centered_string(char* str, display_font_t font, display_color_t color)
{
	uint16_t string_pixel_width = strlen(str) * font_width(font);
	uint8_t x = (DISPLAY_WIDTH - string_pixel_width) / 2;

	uint16_t string_pixel_height = font_height(font);
	uint8_t y = (DISPLAY_HEIGHT - string_pixel_height) / 2;

	display_set_cursor(x, y);
	display_write_string(str, font, color);
}

/*
 * Draws the string horizontal centered
 */
void display_write_horizontal_string(char* str, uint8_t y, display_font_t font, display_color_t color)
{
	uint16_t string_pixel_width = strlen(str) * font_width(font);

	uint8_t x = (DISPLAY_WIDTH - string_pixel_width) / 2;

	display_set_cursor(x, y);
	display_write_string(str, font, color);
}

/*
 * Draws and highlights horizontal menu
 */
void display_draw_horizontal_menu(const char* items[],  uint8_t y, uint8_t *current_index, uint8_t item_count, display_font_t font, display_color_t color)
{
	const uint8_t fw = font_width(font);
	const uint8_t fh = font_height(font);
	uint16_t section_width = DISPLAY_WIDTH / item_count;

	for (uint8_t i = 0; i < item_count; i++)
	{
		uint8_t word_len = strlen(items[i]);
		uint8_t word_pixel_width = word_len * fw;

		uint8_t center_x = i * section_width + section_width / 2;
		uint8_t word_start_x = center_x - word_pixel_width / 2;

		// highlight if applicable. insert "&avoid_highlight" to avoid
		if (i == *current_index)
		{
			display_set_cursor(center_x, y - fh - 2);
			display_write_string("V", font, color);

			// background white, string black.
			display_fill_rectangle(word_start_x - 2, y - 1, word_start_x + word_pixel_width + 1, y + fh, color);

			// draw word(s)
			display_set_cursor(word_start_x, y);
			display_write_string(items[i], font, display_color_black);
		}
		else
		{
			// draw word(s)
			display_set_cursor(word_start_x, y);
			display_write_string(items[i], font, color);
		}
	}
}

/* Vertical Centered Functions */

/*
 * Draws and highlights vertical menu
 */
void display_draw_vertical_menu(const char* items[], uint8_t start_y, uint8_t *current_index, uint8_t item_count, display_font_t font, display_color_t color)
{
	const uint8_t fw = font_width(font);
	const uint8_t fh = font_height(font);
	uint16_t line_height = fh + 2;

	for (uint8_t i = 0; i < item_count; i++)
	{
		uint8_t y = start_y + i * line_height;
		uint8_t string_pixel_width = strlen(items[i]) * fw;
		uint8_t x = (DISPLAY_WIDTH - string_pixel_width) / 2;

		// highlight if applicable. insert "&avoid_highlight" to avoid
		if (i == *current_index)
		{
			// define highlight symbol ">" & "<" positions
			uint8_t left_highlight_x = x - 2 * fw;
			uint8_t right_highlight_x = x + string_pixel_width + fw;

			// background white, string black.
			display_fill_rectangle(x - 2, y - 1, x + string_pixel_width + 1, y + fh, color);

			display_set_cursor(left_highlight_x, y);
			display_write_string(">", font, color);
			display_set_cursor(right_highlight_x, y);
			display_write_string("<", font, color);

			// draw word(s)
			display_set_cursor(x, y);
			display_write_string(items[i], font, display_color_black);
		}
		else
		{
			// draw word(s)
			display_set_cursor(x, y);
			display_write_string(items[i], font, color);
		}
	}
}
