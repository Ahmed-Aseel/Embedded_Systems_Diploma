/* 
 * File:   ecu_layer_init.c
 * Author: Ahmed Aseel
 *
 * Created on April 22, 2023, 1:53 AM
 */

#include "ecu_layer_init.h"

lcd_4bits_config_t lcd_1 = {
    .lcd_rs.port = PORTC_INDEX,
    .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_rs.logic = GPIO_LOW,

    .lcd_en.port = PORTC_INDEX,
    .lcd_en.pin = GPIO_PIN1,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_en.logic = GPIO_LOW,

    .lcd_data_pins[0].port = PORTC_INDEX,
    .lcd_data_pins[0].pin = GPIO_PIN2,
    .lcd_data_pins[0].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[0].logic = GPIO_LOW,

    .lcd_data_pins[1].port = PORTC_INDEX,
    .lcd_data_pins[1].pin = GPIO_PIN3,
    .lcd_data_pins[1].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[1].logic = GPIO_LOW,

    .lcd_data_pins[2].port = PORTC_INDEX,
    .lcd_data_pins[2].pin = GPIO_PIN4,
    .lcd_data_pins[2].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[2].logic = GPIO_LOW,

    .lcd_data_pins[3].port = PORTC_INDEX,
    .lcd_data_pins[3].pin = GPIO_PIN5,
    .lcd_data_pins[3].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[3].logic = GPIO_LOW,    
};

lcd_8bits_config_t lcd_2 = {
    .lcd_rs.port = PORTC_INDEX,
    .lcd_rs.pin = GPIO_PIN6,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_rs.logic = GPIO_LOW,

    .lcd_en.port = PORTC_INDEX,
    .lcd_en.pin = GPIO_PIN7,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_en.logic = GPIO_LOW,

    .lcd_data_pins[0].port = PORTD_INDEX,
    .lcd_data_pins[0].pin = GPIO_PIN0,
    .lcd_data_pins[0].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[0].logic = GPIO_LOW,

    .lcd_data_pins[1].port = PORTD_INDEX,
    .lcd_data_pins[1].pin = GPIO_PIN1,
    .lcd_data_pins[1].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[1].logic = GPIO_LOW,

    .lcd_data_pins[2].port = PORTD_INDEX,
    .lcd_data_pins[2].pin = GPIO_PIN2,
    .lcd_data_pins[2].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[2].logic = GPIO_LOW,

    .lcd_data_pins[3].port = PORTD_INDEX,
    .lcd_data_pins[3].pin = GPIO_PIN3,
    .lcd_data_pins[3].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[3].logic = GPIO_LOW,    

    .lcd_data_pins[4].port = PORTD_INDEX,
    .lcd_data_pins[4].pin = GPIO_PIN4,
    .lcd_data_pins[4].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[4].logic = GPIO_LOW,

    .lcd_data_pins[5].port = PORTD_INDEX,
    .lcd_data_pins[5].pin = GPIO_PIN5,
    .lcd_data_pins[5].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[5].logic = GPIO_LOW,

    .lcd_data_pins[6].port = PORTD_INDEX,
    .lcd_data_pins[6].pin = GPIO_PIN6,
    .lcd_data_pins[6].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[6].logic = GPIO_LOW,

    .lcd_data_pins[7].port = PORTD_INDEX,
    .lcd_data_pins[7].pin = GPIO_PIN7,
    .lcd_data_pins[7].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data_pins[7].logic = GPIO_LOW, 
};

void ecu_layer_initialize(void)
{
    Std_ReturnType ret = E_NOT_OK;
    
    ret = ecu_lcd_4bits_initialize(&lcd_1);
    ret = ecu_lcd_8bits_initialize(&lcd_2);
}
