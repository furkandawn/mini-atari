/*
 * display_utils.h
 *
 *  Created on: May 23, 2025
 *      Author: furkandawn
 */

#ifndef INC_DISPLAY_UTILS_H_
#define INC_DISPLAY_UTILS_H_

#include "display_interface.h"
#include <stdint.h>

extern uint8_t avoid_highlight;

void display_init(void);
void display_clear(void);
void display_update(void);
void display_set_cursor(uint8_t x, uint8_t y);
uint8_t font_width(display_font_t font);
uint8_t font_height(display_font_t font);

char display_write_string(const char* str, display_font_t font, display_color_t color);
void display_draw_bitmap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t w, uint8_t h, display_color_t color);
void display_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, display_color_t color);
void display_fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, display_color_t color);
void display_fill_square(uint8_t x, uint8_t y, uint8_t size, display_color_t color);

void display_write_centered_string(char* str, display_font_t font, display_color_t color);
void display_write_horizontal_string(char* str, uint8_t y, display_font_t font, display_color_t color);
void display_draw_horizontal_menu(const char* items[],  uint8_t y, uint8_t *current_index, uint8_t item_count, display_font_t font, display_color_t color);
void display_draw_vertical_menu(const char* items[], uint8_t start_y, uint8_t *current_index, uint8_t item_count, display_font_t font, display_color_t color);

#endif /* INC_DISPLAY_UTILS_H_ */
