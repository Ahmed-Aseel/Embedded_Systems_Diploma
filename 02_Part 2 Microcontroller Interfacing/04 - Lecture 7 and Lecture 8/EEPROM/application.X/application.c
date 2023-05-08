/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

Std_ReturnType ret = E_NOT_OK;
volatile uint8 program_selected = 0;

void program_1(void){
    ret = ecu_led_turn_on(&led_1);
    __delay_ms(250);
    ret = ecu_led_turn_off(&led_1);
    __delay_ms(250);
}

void program_2(void){
    ret = ecu_led_turn_on(&led_2);
    __delay_ms(250);
    ret = ecu_led_turn_off(&led_2);
    __delay_ms(250);
}

void INT0_App_ISR(void)
{
    program_selected++;
    if(3 == program_selected)
    {
        program_selected = 0;   
    }            
    ret = mcal_EEPROM_WriteByte(0x3FF, program_selected);
}

INTx_config_t int0_obj = 
{
    .EXT_InterruptHandler = INT0_App_ISR,
    .source = INTERRUPT_EXTERNAL_INT0,
    .edge = INTERRUPT_RISING_EDGE,
    .priority = INTERRUPT_PRIORITY_HIGH,
    .INTx_pin.port = PORTB_INDEX, 
    .INTx_pin.pin = GPIO_PIN0,
    .INTx_pin.direction = GPIO_DIRECTION_INPUT,
    .INTx_pin.logic = GPIO_LOW
};

int main()
{
    application_initialize();
    ret = mcal_EEPROM_ReadByte(0x3FF, &program_selected);
        
    while(1)
    {
        if(1 == program_selected)
        {
            program_1();
        }
        else if(2 == program_selected)
        {
            program_2();
        }
        else
        {
             ret = ecu_led_turn_off(&led_1);
             ret = ecu_led_turn_off(&led_2);
        }
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
    ret = mcal_interrupt_INTx_Init(&int0_obj);
}