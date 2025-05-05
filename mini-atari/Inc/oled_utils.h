/*
 * oled_utils.h
 *
 *  Created on: May 4, 2025
 *      Author: furkandawn
 */

#include <stdint.h>
#include "ssd1306.h"

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

void join_menu_items(const char* items[], uint8_t item_count, char* output_buffer, size_t buffer_size);
void oled_draw_h_centered(const char* str, SSD1306_Font_t Font, uint8_t y, SSD1306_COLOR color);
void oled_draw_h_section(const char* str, SSD1306_Font_t Font, uint8_t y, SSD1306_COLOR color, uint8_t max_items);
void oled_draw_h_highlight(const char* str, SSD1306_Font_t Font, uint8_t y, SSD1306_COLOR color, uint8_t *current_index, uint8_t max_items);

#endif /* INC_OLED_UTILS_H_ */
