/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

void Timer3_App_ISR(void);
void CCP1_App_ISR(void);

Std_ReturnType ret = E_NOT_OK;
volatile uint8 ccp1_intr_flag = 0;
volatile uint16 second_capture = 0;
volatile uint32 timer3_overflow = 0;
volatile uint32 total_period_us = 0;
volatile uint32 freq = 0;

timer3_config_t timer3_timer_obj = {
    .timer3_mode = TIMER3_TIMER_MODE,
    .timer3_reg_rw_mode = TIMER3_16BIT_RW_MODE,
    .prescaler = TIMER3_PRESCALER_DIV_BY_1,
    .timer3_preload_val = 0,
    #if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        .Timer3_InterruptHandler = Timer3_App_ISR,
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
    .ccp_mode = CCP_CAPTURE_MODE_SELECT,
    .ccp_mode_variant = CCP_CAPTURE_MODE_1_RISING_EDGE,
    #if (CCP1_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
        .PWM_freq = 20000,
        .timer2_prescaler = 1,
        .timer2_postscaler = 1,
    #endif    
    .ccp_pin = {.port = PORTC_INDEX, .pin = GPIO_PIN2, 
                 .direction = GPIO_DIRECTION_INPUT, .logic = GPIO_LOW}
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
    ret = mcal_Timer3_Init(&timer3_timer_obj);
    ret = mcal_CCP_Init(&ccp1_obj);
}

void CCP1_App_ISR(void)
{
    Std_ReturnType ret = E_NOT_OK;

    ccp1_intr_flag++; 
    
    if(1 == ccp1_intr_flag)
    {
        ret = mcal_Timer3_Write_Value(&timer3_timer_obj, 0);
    }
    else if(2 == ccp1_intr_flag)
    {
        ret = mcal_CCP_Capture_Mode_Read_Value(&ccp1_obj, &second_capture);
        total_period_us = (timer3_overflow * 65536) + second_capture;
        timer3_overflow = 0;
        freq = 1 / (total_period_us / 1000000.0);
        ccp1_intr_flag = 0;
    }
    else{/* nothing */}
}

void Timer3_App_ISR(void)
{
    timer3_overflow++;
}