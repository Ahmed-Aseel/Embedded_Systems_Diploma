/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

Std_ReturnType ret = E_NOT_OK;
uint8 keypad_val = 0;

int main()
{
    application_initialize();

    while(1)
    {
        ret = ecu_keypad_get_value(&keypad_1, &keypad_val);
        if('1' == keypad_val)
        {
            ecu_led_turn_on(&led_1);
        }
        else if('0' == keypad_val)
        {
            ecu_led_turn_off(&led_1);
        }
        else
        {
        }
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
}