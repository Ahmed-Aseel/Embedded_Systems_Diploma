/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

dc_motor_config_t dc_motor_1 = {
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].port = PORTC_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].pin = GPIO_PIN0,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].logic = DC_MOTOR_OFF,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].port = PORTC_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].pin = GPIO_PIN1,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].logic = DC_MOTOR_OFF
};

dc_motor_config_t dc_motor_2 = {
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].port = PORTC_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].pin = GPIO_PIN2, 
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pins[DC_MOTOR_PIN1_INDEX].logic = DC_MOTOR_OFF,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].port = PORTC_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].pin = GPIO_PIN3, 
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pins[DC_MOTOR_PIN2_INDEX].logic = DC_MOTOR_OFF
};

Std_ReturnType ret = E_NOT_OK;

int main()
{
    application_initialize();

    while(1)
    {
        ret = ecu_dc_motor_move_right(&dc_motor_1);
        ret = ecu_dc_motor_move_right(&dc_motor_2);
        __delay_ms(2000);
        ret = ecu_dc_motor_move_left(&dc_motor_1);
        ret = ecu_dc_motor_move_left(&dc_motor_2);
        __delay_ms(2000);
        ret = ecu_dc_motor_stop(&dc_motor_1);
        ret = ecu_dc_motor_stop(&dc_motor_2);
        __delay_ms(2000);
        ret = ecu_dc_motor_move_right(&dc_motor_1);
        ret = ecu_dc_motor_move_left(&dc_motor_2);
        __delay_ms(2000);
        ret = ecu_dc_motor_stop(&dc_motor_1);
        ret = ecu_dc_motor_stop(&dc_motor_2);
        __delay_ms(2000);
        ret = ecu_dc_motor_move_left(&dc_motor_1);
        ret = ecu_dc_motor_move_right(&dc_motor_2);
        __delay_ms(2000);
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ret = ecu_dc_motor_initialize(&dc_motor_1);
    ret = ecu_dc_motor_initialize(&dc_motor_2);   
}