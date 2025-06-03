/*
 * game_bitmap.h
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_BITMAP_H_
#define INC_GAME_BITMAP_H_

#define BITMAP_HEART_WIDTH	32
#define BITMAP_HEART_HEIGHT	32
#define BITMAP_HEART_BUFFER_SIZE (BITMAP_HEART_WIDTH * BITMAP_HEART_HEIGHT / 8)

#include "menu_interface.h"
#include "display_config.h"

extern const unsigned char game_startup_bitmap[GAME_COUNT][DISPLAY_BUFFER_SIZE];
extern const unsigned char game_heart_bitmap[BITMAP_HEART_BUFFER_SIZE];
extern const unsigned char game_broken_heart_bitmap[BITMAP_HEART_BUFFER_SIZE];

#endif /* INC_GAME_BITMAP_H_ */
