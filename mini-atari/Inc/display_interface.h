/*
 * display_interface.h
 *
 *  Created on: May 23, 2025
 *      Author: furkandawn
 */

/*
 * This file defines the abstract interface for all display backends.
 * Game and UI should only include and use this header.
 */

#ifndef INC_DISPLAY_INTERFACE_H_
#define INC_DISPLAY_INTERFACE_H_

#include "display_config.h"		// for DISPLAY_WIDTH, DISPLAY_HEIGHT
#include <stdint.h>

typedef enum
{
	display_color_black = 0,
	display_color_white
	// add here
} display_color_t;

typedef enum
{
	display_font_6x8,
	display_font_7x10,
	display_font_11x18,
	//display_font_16x15,	disabled to free up space
	display_font_16x24,
	//display_font_16x26	disabled to free up space
	// add here
} display_font_t;

// Basic display control
void display_init(void);
void display_clear(void);
void display_update(void);
void display_set_cursor(uint8_t x, uint8_t y);
uint8_t font_width(display_font_t display_font);
uint8_t font_height(display_font_t display_font);
char display_write_string(const char* str, display_font_t display_font, display_color_t display_color);

// Drawing primitives
void display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t w, uint8_t h, display_color_t display_color);
void display_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, display_color_t display_color);
void display_fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, display_color_t display_color);
void display_fill_square(uint8_t x, uint8_t y, uint8_t size, display_color_t display_color);

// Custom drawing helpers
extern uint8_t avoid_highlight;
void display_write_centered_string(char* str, display_font_t display_font, display_color_t display_color);
void display_write_horizontal_string(char* str, uint8_t y, display_font_t display_font, display_color_t display_color);
void display_draw_horizontal_menu(const char* items[],  uint8_t y, uint8_t *current_index, uint8_t item_count, display_font_t display_font, display_color_t display_color);
void display_draw_vertical_menu(const char* items[], uint8_t start_y, uint8_t *current_index, uint8_t item_count, display_font_t display_font, display_color_t display_color);

#endif /* INC_DISPLAY_INTERFACE_H_ */
