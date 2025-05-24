/*
 * display_ssd1306.c
 *
 *  Created on: May 23, 2025
 *      Author: furkandawn
 */


#include "display_ssd1306.h"

#include "display_interface.h"

#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306

/*
 * SSD1306 Fonts are accessed with this function
 * see display_font_t and SSD1306_Font_t
 */
const SSD1306_Font_t *display_ssd1306_map_font(display_font_t font)
{
	switch(font)
	{
	case display_font_6x8:		return &Font_6x8;
	case display_font_7x10:		return &Font_7x10;
	case display_font_11x18:	return &Font_11x18;
	//case display_font_16x15:	return &Font_16x15;	disabled to free up space
	case display_font_16x24:	return &Font_16x24;
	//case display_font_16x26:	return &Font_16x26;	disabled to free up space
	default: return &Font_6x8;
	}
}

/*
 * SSD1306 colors are accessed with this function
 * see display_color_t and SSD1306_COLOR
 */
const SSD1306_COLOR display_ssd1306_map_color(display_color_t color)
{
	switch (color)
	{
	case display_color_black:	return Black;
	case display_color_white:	return White;
	default: return Black;
	}
}

#endif /* DISPLAY_TYPE == DISPLAY_TYPE_SSD1306 */
