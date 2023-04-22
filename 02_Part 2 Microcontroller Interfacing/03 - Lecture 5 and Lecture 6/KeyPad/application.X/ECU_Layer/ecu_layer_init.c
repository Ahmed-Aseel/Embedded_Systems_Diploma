/* 
 * File:   ecu_layer_init.c
 * Author: Ahmed Aseel
 *
 * Created on April 22, 2023, 1:53 AM
 */

#include "ecu_layer_init.h"

led_config_t led_1 = {
    .led_pin.port = PORTD_INDEX,
    .led_pin.pin = GPIO_PIN0,
    .led_pin.direction = GPIO_DIRECTION_OUTPUT,
    .led_pin.logic = GPIO_LOW,    
};

keypad_config_t keypad_1 = 
{
    .keypad_rows_pins[0].port = PORTC_INDEX,
    .keypad_rows_pins[0].pin = GPIO_PIN0,
    .keypad_rows_pins[0].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[0].logic = GPIO_LOW,

    .keypad_rows_pins[1].port = PORTC_INDEX,
    .keypad_rows_pins[1].pin = GPIO_PIN1,
    .keypad_rows_pins[1].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[1].logic = GPIO_LOW,

    .keypad_rows_pins[2].port = PORTC_INDEX,
    .keypad_rows_pins[2].pin = GPIO_PIN2,
    .keypad_rows_pins[2].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[2].logic = GPIO_LOW,

    .keypad_rows_pins[3].port = PORTC_INDEX,
    .keypad_rows_pins[3].pin = GPIO_PIN3,
    .keypad_rows_pins[3].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[3].logic = GPIO_LOW,    

    .keypad_columns_pins[0].port = PORTC_INDEX,
    .keypad_columns_pins[0].pin = GPIO_PIN4,
    .keypad_columns_pins[0].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[0].logic = GPIO_LOW,

    .keypad_columns_pins[1].port = PORTC_INDEX,
    .keypad_columns_pins[1].pin = GPIO_PIN5,
    .keypad_columns_pins[1].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[1].logic = GPIO_LOW,

    .keypad_columns_pins[2].port = PORTC_INDEX,
    .keypad_columns_pins[2].pin = GPIO_PIN6,
    .keypad_columns_pins[2].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[2].logic = GPIO_LOW,

    .keypad_columns_pins[3].port = PORTC_INDEX,
    .keypad_columns_pins[3].pin = GPIO_PIN7,
    .keypad_columns_pins[3].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[3].logic = GPIO_LOW,
};

void ecu_layer_initialize(void)
{
    Std_ReturnType ret = E_NOT_OK;
    ret = ecu_led_initialize(&led_1);
    ret = ecu_keypad_initialize(&keypad_1);
}
