/*
 * oled_utils.h
 *
 *  Created on: May 4, 2025
 *      Author: furkandawn
 */

#include <stdint.h>
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_conf.h"

#ifndef INC_OLED_UTILS_H_
#define INC_OLED_UTILS_H_

// SSD1306 OLED width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH     128
#endif

// SSD1306 OLED height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT     64
#endif

typedef enum
{
	oled_font_6x8,
	oled_font_7x10,
	oled_font_11x18,
	oled_font_16x15,
	oled_font_16x24,
	oled_font_16x26
} oled_font_t;

typedef enum
{
	oled_color_black = 0x00,
	oled_color_white = 0x01
} oled_color_t;

extern uint8_t avoid_highlight; // insert this into *current_index to avoid highlighting

void oled_init(void);
void oled_set_cursor(uint8_t x, uint8_t y);
void oled_clear(void);
void oled_update(void);
void oled_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, oled_color_t oled_color);
void oled_fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, oled_color_t oled_color);
void oled_fill_square(uint8_t x, uint8_t y, uint8_t size, oled_color_t oled_color);
char oled_write_string(const char* str, oled_font_t oled_font, oled_color_t oled_color);
void oled_write_centered_string(char* str, oled_font_t oled_font, oled_color_t oled_color);
void oled_write_horizontal_string(char* str, oled_font_t oled_font, uint8_t y, oled_color_t oled_color);
void oled_draw_horizontal_menu(const char* items[], oled_font_t oled_font, uint8_t y, oled_color_t oled_color, uint8_t *current_index, uint8_t item_count);
void oled_draw_vertical_menu(const char* items[], oled_font_t oled_font, uint8_t start_y,oled_color_t oled_color, uint8_t *current_index, uint8_t item_count);


#endif /* INC_OLED_UTILS_H_ */
