/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

void Timer0_App_ISR(void);

Std_ReturnType ret = E_NOT_OK;
volatile uint16 freq = 0;

timer0_config_t timer0_timer_obj = {
    .timer0_mode = TIMER0_TIMER_MODE,
    .timer0_reg_size = TIMER0_16BIT_ENABLE,
    .timer0_prescaler_enable = TIMER0_PRESCALER_ENABLE_CFG,
    .prescaler = TIMER0_PRESCALER_DIV_BY_16,
    .timer0_preload_val = 3036, /* delay 1s */
    #if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        .Timer0_InterruptHandler = Timer0_App_ISR,
        .priority = INTERRUPT_PRIORITY_HIGH,
    #endif
};

timer1_config_t timer1_counter_obj = {
    .timer1_mode = TIMER1_COUNTER_MODE,
    .timer1_counter_sync = TIMER1_SYNC_COUNTER,
    .timer1_reg_size = TIMER1_16BIT_ENABLE,
    .prescaler = TIMER1_PRESCALER_DIV_BY_1,
    .timer1_preload_val = 0,
    #if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        .Timer1_InterruptHandler = NULL,
        .priority = INTERRUPT_PRIORITY_HIGH,
    #endif
};

int main()
{
    application_initialize();

    while(1)
    {
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
    
    ret = mcal_Timer0_Init(&timer0_timer_obj);
    ret = mcal_Timer1_Init(&timer1_counter_obj);
}
        
void Timer0_App_ISR(void)
{
    ret = ecu_led_toggle(&led_1);
    
    ret = mcal_Timer1_Read_Value(&timer1_counter_obj, &freq);
    ret = mcal_Timer1_Write_Value(&timer1_counter_obj, 0);
}
