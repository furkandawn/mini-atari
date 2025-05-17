/*
 * oled_utils.c
 *
 *  Created on: May 4, 2025
 *      Author: furkandawn
 */


#include "oled_utils.h"


// ----->> includes start

// include ssd1306 OLED Display libraries
// included in header file

// include mini-atari libraries


// include other
#include <string.h>
#include <stdint.h>

// includes end <<-----

uint8_t avoid_highlight = 255; // use this in functions to avoid highlighting & navigation.


/* oled fonts */

/*
 * SSD1306 Fonts are accessed with this function
 * see oled_font_t and SSD1306_Font_t
 */
static const SSD1306_Font_t *map_font(oled_font_t oled_font)
{
	switch(oled_font)
	{
	case oled_font_6x8: return &Font_6x8;
	case oled_font_7x10: return &Font_7x10;
	case oled_font_11x18: return &Font_11x18;
	case oled_font_16x15: return &Font_16x15;
	case oled_font_16x24: return &Font_16x24;
	case oled_font_16x26: return &Font_16x26;
	default: return &Font_6x8;
	}
}

/* oled color */

/*
 * SSD1306 colors are accessed with this function
 * see oled_color_t and SSD1306_COLOR
 */
static const SSD1306_COLOR map_color(oled_color_t oled_color)
{
	switch (oled_color)
	{
	case oled_color_black: return Black;
	case oled_color_white: return White;
	default: return Black;
	}
}

/* oled general purpose functions */

/*
 * Init OLED screen
 */
void oled_init(void)
{
	ssd1306_Init();
}
/*
 * Sets the cursor at requested position
 */
void oled_set_cursor(uint8_t x, uint8_t y)
{
	ssd1306_SetCursor(x, y);
}

/*
 * Clears the screen by filling it black
 */
void oled_clear(void)
{
	ssd1306_Fill(Black);
}

/*
 * Pushes the current buffer to the OLED screen
 */
void oled_update(void)
{
	ssd1306_UpdateScreen();
}

/*
 * Draws an unfilled rectangle
 */
void oled_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, oled_color_t oled_color)
{
	ssd1306_DrawRectangle(x1, y1, x2, y2, map_color(oled_color));
}

/*
 * Draws a filled rectangle
 */
void oled_fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, oled_color_t oled_color)
{
	ssd1306_FillRectangle(x1, y1, x2, y2, map_color(oled_color));
}

/*
 * Draws a filled square
 */
void oled_fill_square(uint8_t x, uint8_t y, uint8_t size, oled_color_t oled_color)
{
	ssd1306_FillSquare(x, y, size, map_color(oled_color));
}

/*
 * Writes a string at requested position
 */
char oled_write_string(const char* str, oled_font_t oled_font, oled_color_t oled_color)
{
	return ssd1306_WriteString(str, *map_font(oled_font), map_color(oled_color));
}

/*
 * Draws the string at the center of screen
 */
void oled_write_centered_string(char* str, oled_font_t oled_font, oled_color_t oled_color)
{
	uint16_t string_pixel_width = strlen(str) * map_font(oled_font)->width;
	uint8_t x = (SSD1306_WIDTH - string_pixel_width) / 2;

	uint16_t string_pixel_height = map_font(oled_font)->height;
	uint8_t y = (SSD1306_HEIGHT - string_pixel_height) / 2;

	oled_set_cursor(x, y);
	oled_write_string(str, oled_font, oled_color);
}


/* Horizontal Centered Functions */

/*
 * Draws the string horizontal centered
 */
void oled_write_horizontal_string(char* str, oled_font_t oled_font, uint8_t y, oled_color_t oled_color)
{
	uint16_t string_pixel_width = strlen(str) * map_font(oled_font)->width;

	uint8_t x = (SSD1306_WIDTH - string_pixel_width) / 2;

	oled_set_cursor(x, y);
	oled_write_string(str, oled_font, oled_color);
}

/*
 * Draws and highlights horizontal menu
 */
void oled_draw_horizontal_menu(const char* items[], oled_font_t oled_font, uint8_t y, oled_color_t oled_color, uint8_t *current_index, uint8_t item_count)
{
	uint16_t section_width = SSD1306_WIDTH / item_count;

	for (uint8_t i = 0; i < item_count; i++)
	{
		uint8_t word_len = strlen(items[i]);
		uint8_t word_pixel_width = word_len * map_font(oled_font)->width;

		uint8_t center_x = i * section_width + section_width / 2;
		uint8_t word_start_x = center_x - word_pixel_width / 2;

		// highlight if applicable. insert "&avoid_highlight" to avoid
		if (i == *current_index)
		{
			oled_set_cursor(center_x, y - map_font(oled_font)->height - 2);
			oled_write_string("V", oled_font, oled_color);

			// background white, string black.
			oled_fill_rectangle(word_start_x - 2, y - 1, word_start_x + word_pixel_width + 1, y + map_font(oled_font)->height, oled_color);

			// draw word(s)
			oled_set_cursor(word_start_x, y);
			oled_write_string(items[i], oled_font, oled_color_black);
		}
		else
		{
			// draw word(s)
			oled_set_cursor(word_start_x, y);
			oled_write_string(items[i], oled_font, oled_color);
		}
	}
}

/* Vertical Centered Functions */

/*
 * Draws and highlights vertical menu
 */
void oled_draw_vertical_menu(const char* items[], oled_font_t oled_font, uint8_t start_y, oled_color_t oled_color, uint8_t *current_index, uint8_t item_count)
{
	uint16_t line_height = map_font(oled_font)->height + 2;

	for (uint8_t i = 0; i < item_count; i++)
	{
		uint8_t y = start_y + i * line_height;
		uint8_t string_pixel_width = strlen(items[i]) * map_font(oled_font)->width;
		uint8_t x = (SSD1306_WIDTH - string_pixel_width) / 2;

		// highlight if applicable. insert "&avoid_highlight" to avoid
		if (i == *current_index)
		{
			// define highlight symbol ">" & "<" positions
			uint8_t left_highlight_x = x - 2 * map_font(oled_font)->width;
			uint8_t right_highlight_x = x + string_pixel_width + map_font(oled_font)->width;

			// background white, string black.
			oled_fill_rectangle(x - 2, y - 1, x + string_pixel_width + 1, y + map_font(oled_font)->height, oled_color);

			oled_set_cursor(left_highlight_x, y);
			oled_write_string(">", oled_font, oled_color);
			oled_set_cursor(right_highlight_x, y);
			oled_write_string("<", oled_font, oled_color);

			// draw word(s)
			oled_set_cursor(x, y);
			oled_write_string(items[i], oled_font, oled_color_black);
		}
		else
		{
			// draw word(s)
			oled_set_cursor(x, y);
			oled_write_string(items[i], oled_font, oled_color);
		}
	}
}
