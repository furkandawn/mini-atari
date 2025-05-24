/*
 * joystick.c
 *
 *  Created on: Apr 21, 2025
 *      Author: furkandawn
 */


#include "joystick.h"

#define DEBOUNCE_DELAY_MS 50
#define AXIS_MAX_VALUE 4095
// === Includes Start ===

#include "adc.h"
#include "gpio.h"

// === Includes End ===

extern ADC_HandleTypeDef hadc;

joystick_data_t joystick_read(void)
{
	joystick_data_t data;

    // Read X axis
    HAL_ADC_Start(&hadc);
    if (HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY) == HAL_OK)
    {
    	data.x = HAL_ADC_GetValue(&hadc);
    }
    else
    {
    	data.x = AXIS_MAX_VALUE / 2; // if timeout, fallback mid-value.
    }

    // Read Y axis
    HAL_ADC_Start(&hadc);
    if (HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY) == HAL_OK)
    {
    	data.y = HAL_ADC_GetValue(&hadc);
    }
    else
    {
    	data.y = AXIS_MAX_VALUE / 2; // if timeout, fallback mid-value.
    }

	return data;
}

joystick_direction_t joystick_direction(void)
{
	joystick_data_t joystick_data = joystick_read();
	joystick_data.y = AXIS_MAX_VALUE - joystick_data.y;

	if (joystick_data.x > JOYSTICK_THRESHOLD_HIGH) return DIRECTION_RIGHT;
	else if (joystick_data.x < JOYSTICK_THRESHOLD_LOW) return DIRECTION_LEFT;
	else if (joystick_data.y > JOYSTICK_THRESHOLD_HIGH) return DIRECTION_UP;
	else if (joystick_data.y < JOYSTICK_THRESHOLD_LOW) return DIRECTION_DOWN;
	else return DIRECTION_NONE;
}

bool joystick_is_pressed(void)
{
	static uint32_t last_press_time = 0;
	static bool was_pressed_last_time = false;

	bool is_currently_pressed = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET);

	if (is_currently_pressed && !was_pressed_last_time)
	{
		uint32_t current_time = HAL_GetTick();
		if (current_time - last_press_time > DEBOUNCE_DELAY_MS)
		{
			last_press_time = current_time;
			was_pressed_last_time = true;
			return true;
		}
	}
	else if(!is_currently_pressed)
	{
		was_pressed_last_time = false;
	}
	return false;
}
