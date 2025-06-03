/*
 * joystick.c
 *
 *  Created on: Apr 21, 2025
 *      Author: furkandawn
 */


#include "joystick.h"


// ===== Includes ===== //
#include "adc.h"
#include "gpio.h"
extern ADC_HandleTypeDef hadc;

// ======= Macros/Constants ===== //
#define DEBOUNCE_DELAY_MS 200
#define AXIS_MAX_VALUE 4095

// ===== Static File-Private Variables ===== //
static bool joystick_pressed_flag = false;
static bool button_pressed_flag = false;

// ===== Public Global Variables ===== //
bool input_enabled = true;

// ===== Static Function Declarations ===== //
static joystick_data_t joystick_read(void);

// ===== Public API Function Definitions ===== //

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

bool is_joystick_pressed(void)
{
	if (joystick_pressed_flag)
	{
		joystick_pressed_flag = false;
		return true;
	}
	return false;
}

bool is_button_pressed(void)
{
	if (button_pressed_flag)
	{
		button_pressed_flag = false;
		return true;
	}
	return false;
}

// ===== Static Function Definitions ===== //
static joystick_data_t joystick_read(void)
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

// interrupt for joystick/button press
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (!input_enabled) return;

	static uint32_t last_joystick_press = 0;
	static uint32_t last_button_press = 0;
	uint32_t now = HAL_GetTick();

	if (GPIO_Pin == GPIO_PIN_4)
	{
		if (now - last_joystick_press > DEBOUNCE_DELAY_MS)
		{
			last_joystick_press = now;
			joystick_pressed_flag = true;
		}
	}
	else if (GPIO_Pin == GPIO_PIN_5)
	{
		if (now - last_button_press > DEBOUNCE_DELAY_MS)
		{
			last_button_press = now;
			button_pressed_flag = true;
		}
	}
}
