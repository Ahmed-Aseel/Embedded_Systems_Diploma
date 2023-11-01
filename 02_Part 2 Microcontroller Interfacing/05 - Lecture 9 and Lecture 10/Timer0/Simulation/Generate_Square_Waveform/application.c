/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

void Timer0_App_ISR(void);

Std_ReturnType ret = E_NOT_OK;
volatile uint8 sq_wave_50Hz = 0, sq_wave_25Hz = 0;

timer0_config_t timer0_timer_obj = {
    .timer0_mode = TIMER0_TIMER_MODE,
    .timer0_reg_size = TIMER0_16BIT_ENABLE,
    .timer0_prescaler_enable = TIMER0_PRESCALER_DISABLE_CFG,
    .prescaler = TIMER0_PRESCALER_DIV_BY_4,
    .timer0_preload_val = 55536, /* freq 50Hz */
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
        /* led_1 freq 50Hz */
        if(sq_wave_50Hz == 1)
        {
            ret = ecu_led_toggle(&led_1);
            sq_wave_50Hz = 0;
        }
        
        /* led_2 freq 25Hz */
        if(sq_wave_25Hz == 2)
        {
            ret = ecu_led_toggle(&led_2);
            sq_wave_25Hz = 0;
        }
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
    ret = mcal_Timer0_Init(&timer0_timer_obj);
}
        
void Timer0_App_ISR(void)
{
    sq_wave_50Hz++;
    sq_wave_25Hz++;
}