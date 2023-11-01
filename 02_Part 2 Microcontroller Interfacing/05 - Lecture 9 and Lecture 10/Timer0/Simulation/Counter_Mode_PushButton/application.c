/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

void Timer0_App_ISR(void);

Std_ReturnType ret = E_NOT_OK;
volatile uint16 button_counter = 0;

timer0_config_t timer0_counter_obj = {
    .timer0_mode = TIMER0_COUNTER_MODE,
    .timer0_counter_edge = TIMER0_RISING_EDGE_CFG,
    .timer0_reg_size = TIMER0_16BIT_ENABLE,
    .timer0_prescaler_enable = TIMER0_PRESCALER_DISABLE_CFG,
    .timer0_preload_val = 0,
    #if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        .Timer0_InterruptHandler = Timer0_App_ISR,
        .priority = INTERRUPT_PRIORITY_HIGH,
    #endif
};

int main()
{
    application_initialize();

    while(1)
    {
        ret = mcal_Timer0_Read_Value(&timer0_counter_obj, &button_counter);
        if(5 == button_counter)
        {
            ret = ecu_led_turn_on(&led_1);
        }
        else
        {
            ret = ecu_led_turn_off(&led_1);            
        }
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
    ret = mcal_Timer0_Init(&timer0_counter_obj);
}
        
void Timer0_App_ISR(void)
{

}