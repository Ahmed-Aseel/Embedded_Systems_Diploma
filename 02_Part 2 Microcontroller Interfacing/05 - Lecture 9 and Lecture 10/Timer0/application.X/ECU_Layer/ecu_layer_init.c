/* 
 * File:   ecu_layer_init.c
 * Author: Ahmed Aseel
 *
 * Created on April 22, 2023, 1:53 AM
 */

#include "ecu_layer_init.h"

led_config_t led_1 = {
    .led_pin.port = PORTC_INDEX,
    .led_pin.pin = GPIO_PIN0, 
    .led_pin.direction = GPIO_DIRECTION_OUTPUT,
    .led_pin.logic = GPIO_LOW
};

led_config_t led_2 = {
    .led_pin.port = PORTC_INDEX,
    .led_pin.pin = GPIO_PIN1, 
    .led_pin.direction = GPIO_DIRECTION_OUTPUT,
    .led_pin.logic = GPIO_LOW
};

void ecu_layer_initialize(void)
{
    Std_ReturnType ret = E_NOT_OK;
    
    ret = ecu_led_initialize(&led_1); 
    ret = ecu_led_initialize(&led_2); 
}
