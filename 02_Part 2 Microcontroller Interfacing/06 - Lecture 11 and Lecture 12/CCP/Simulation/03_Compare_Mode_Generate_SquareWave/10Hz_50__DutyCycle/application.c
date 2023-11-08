/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

void CCP1_App_ISR(void);

Std_ReturnType ret = E_NOT_OK;

timer3_config_t timer3_timer_obj = {
    .timer3_mode = TIMER3_TIMER_MODE,
    .timer3_reg_rw_mode = TIMER3_16BIT_RW_MODE,
    .prescaler = TIMER3_PRESCALER_DIV_BY_1,
    .timer3_preload_val = 0,
    #if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        .Timer3_InterruptHandler = NULL,
        .priority = INTERRUPT_PRIORITY_HIGH,
    #endif
};

ccp_config_t ccp1_obj = {
    #if (CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE)
        .CCP_InterruptHandler = CCP1_App_ISR,
        .priority = INTERRUPT_PRIORITY_LOW,
    #endif    
    .ccp_inst = CCP1_INST,
    .ccp_timer = CCP1_CCP2_TIMER3,
    .ccp_mode = CCP_COMPARE_MODE_SELECT,
    .ccp_mode_variant = CCP_COMPARE_MODE_TOGGLE_ON_MATCH,  
    .ccp_pin = {.port = PORTC_INDEX, .pin = GPIO_PIN2, 
                 .direction = GPIO_DIRECTION_OUTPUT, .logic = GPIO_LOW}
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
    /* square wave 10Hz duty 50% */
    ret = mcal_CCP_Compare_Mode_Set_Value(&ccp1_obj, 50000);
    ret = mcal_CCP_Init(&ccp1_obj);
    ret = mcal_Timer3_Init(&timer3_timer_obj);
}

void CCP1_App_ISR(void)
{
    Std_ReturnType ret = E_NOT_OK;

    ret = mcal_Timer3_Write_Value(&timer3_timer_obj, 0);   
}
