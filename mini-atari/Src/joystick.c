/*
 * joystick.c
 *
 *  Created on: Apr 21, 2025
 *      Author: furkandawn
 */
#include "joystick.h"
#include "adc.h"
#include "gpio.h"

extern ADC_HandleTypeDef hadc;

joystick_data_t joystick_read(void)
{
	joystick_data_t data;

    // Read X axis
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
    data.x = HAL_ADC_GetValue(&hadc);

    // Read Y axis
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
    data.y = HAL_ADC_GetValue(&hadc);

	return data;
}

joystick_direction_t joystick_direction(void)
{
	joystick_data_t jd = joystick_read();
	jd.y = 4095 - jd.y;

	if (jd.x > JOYSTICK_THRESHOLD_HIGH) return DIRECTION_RIGHT;
	else if (jd.x < JOYSTICK_THRESHOLD_LOW) return DIRECTION_LEFT;
	else if (jd.y > JOYSTICK_THRESHOLD_HIGH) return DIRECTION_UP;
	else if (jd.y < JOYSTICK_THRESHOLD_LOW) return DIRECTION_DOWN;
	else return DIRECTION_NONE;
}

bool joystick_is_pressed(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET)
	{
		HAL_Delay(100);
		return true;
	}
	return false;
}
