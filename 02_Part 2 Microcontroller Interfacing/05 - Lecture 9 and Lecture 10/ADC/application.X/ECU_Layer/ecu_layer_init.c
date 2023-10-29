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
    .lcd_data_pins[3].logic = GPIO_LOW
};

dc_motor_config_t dc_motor_1 = {
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].port = PORTD_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].pin = GPIO_PIN0,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].logic = DC_MOTOR_OFF,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].port = PORTD_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].pin = GPIO_PIN1,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].logic = DC_MOTOR_OFF
};

dc_motor_config_t dc_motor_2 = {
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].port = PORTD_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].pin = GPIO_PIN2, 
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].logic = DC_MOTOR_OFF,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].port = PORTD_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].pin = GPIO_PIN3, 
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].logic = DC_MOTOR_OFF
};

void ecu_layer_initialize(void)
{
    Std_ReturnType ret = E_NOT_OK;
    
    ret = ecu_lcd_4bits_initialize(&lcd_1);
    ret = ecu_dc_motor_initialize(&dc_motor_1);
    ret = ecu_dc_motor_initialize(&dc_motor_2);  
}
