/*
 * joystick.h
 *
 *  Created on: Apr 21, 2025
 *      Author: furkandawn
 */
#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * The joystick i use generates analog input which then converted to digital between 0 and 4095.
 */
#define JOYSTICK_THRESHOLD_LOW 1100
#define JOYSTICK_THRESHOLD_HIGH 3200

typedef enum
{
	DIRECTION_NONE,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
} joystick_direction_t;

typedef struct
{
	uint16_t x;
	uint16_t y;
} joystick_data_t;

joystick_data_t joystick_read(void);
joystick_direction_t joystick_direction(void);
bool joystick_is_pressed(void);
bool button_is_pressed(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_JOYSTICK_H_ */
