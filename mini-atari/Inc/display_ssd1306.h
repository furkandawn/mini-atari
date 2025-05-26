/*
 * display_ssd1306.h
 *
 *  Created on: May 23, 2025
 *      Author: furkandawn
 */

#ifndef INC_DISPLAY_SSD1306_H_
#define INC_DISPLAY_SSD1306_H_

#include "display_interface.h"
#include "display_config.h"
#include <stdint.h>

#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306

#include "ssd1306.h"
#include "ssd1306_fonts.h"

#define map_font display_ssd1306_map_font
#define map_color display_ssd1306_map_color

#define backend_display_init()									ssd1306_Init()
#define backend_display_clear()									ssd1306_Fill(Black)
#define backend_display_update()								ssd1306_UpdateScreen()
#define backend_display_set_cursor(x, y)						ssd1306_SetCursor((x), (y))
#define backend_display_write_string(str, font, color)			ssd1306_WriteString((str), *map_font((font)), map_color((color)))
#define backend_display_draw_rectangle(x1, y1, x2, y2, color)	ssd1306_DrawRectangle((x1), (y1), (x2), (y2), map_color((color)))
#define backend_display_fill_rectangle(x1, y1, x2, y2, color)	ssd1306_FillRectangle((x1), (y1), (x2), (y2), map_color((color)))
#define backend_display_fill_square(x, y, size, color)			ssd1306_FillSquare((x), (y), (size), map_color((color)))
#define backend_display_draw_bitmap(x, y, bitmap, w, h, color)	ssd1306_DrawBitmap((x), (y), (bitmap), (w), (h), map_color((color)))

extern const SSD1306_Font_t *display_ssd1306_map_font(display_font_t font);
extern const SSD1306_COLOR display_ssd1306_map_color(display_color_t color);

#endif /* DISPLAY_TYPE == DISPLAY_TYPE_SSD1306 */

#endif /* INC_DISPLAY_SSD1306_H_ */
