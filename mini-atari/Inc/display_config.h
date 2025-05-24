/*
 * display_config.h
 *
 *  Created on: May 23, 2025
 *      Author: furkandawn
 */

#ifndef INC_DISPLAY_CONFIG_H_
#define INC_DISPLAY_CONFIG_H_


// Define supported display types
#define DISPLAY_TYPE_SSD1306 1


// Define active display type
#define DISPLAY_TYPE DISPLAY_TYPE_SSD1306


// Display dimensions
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
#include "ssd1306_conf.h"
#define DISPLAY_WIDTH SSD1306_WIDTH
#define DISPLAY_HEIGHT SSD1306_HEIGHT
#endif


// Display buffer size (for monochrome displays)
#define DISPLAY_BUFFER_SIZE ((DISPLAY_WIDTH * DISPLAY_HEIGHT) / 8)



#endif /* INC_DISPLAY_CONFIG_H_ */
