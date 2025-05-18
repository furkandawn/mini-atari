/*
 * game_bitmap.h
 *
 *  Created on: May 18, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_BITMAP_H_
#define INC_GAME_BITMAP_H_

#include "menu_main.h"

#define BITMAP_WIDTH	128
#define BITMAP_HEIGHT	64
#define BITMAP_SIZE		(BITMAP_WIDTH * BITMAP_HEIGHT / 8)

extern const unsigned char game_bitmap[GAME_COUNT][BITMAP_SIZE];

#endif /* INC_GAME_BITMAP_H_ */
