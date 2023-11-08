/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

Std_ReturnType ret = E_NOT_OK;
uint8 ccp1_pwm_duty = 0;

timer2_config_t timer2_timer_obj = {
    .Timer2_InterruptHandler = NULL,
    .priority = INTERRUPT_PRIORITY_LOW,
    .prescaler = TIMER2_PRESCALER_DIV_BY_1,
    .postscaler = TIMER2_POSTSCALER_DIV_BY_1,
    .timer2_preload_val = 0
};

ccp_config_t ccp1_obj = {
    #if (CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE) || \
        (CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE)
        .CCP_InterruptHandler = NULL,
        .priority = INTERRUPT_PRIORITY_LOW,
    #endif    
    .ccp_inst = CCP1_INST,
    .ccp_mode = CCP_PWM_MODE_SELECT,
    .ccp_mode_variant = CCP_PWM_MODE,
    #if (CCP1_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED) || \
        (CCP2_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
        .PWM_freq = 20000,
        .timer2_prescaler = 1,
        .timer2_postscaler = 1,
    #endif    
    .ccp_pin = {.port = PORTC_INDEX, .pin = GPIO_PIN2, 
                 .direction = GPIO_DIRECTION_OUTPUT, .logic = GPIO_LOW}

};

ccp_config_t ccp2_obj = {
    #if (CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE) || \
        (CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE)
        .CCP_InterruptHandler = NULL,
        .priority = INTERRUPT_PRIORITY_LOW,
    #endif    
    .ccp_inst = CCP2_INST,
    .ccp_mode = CCP_PWM_MODE_SELECT,
    .ccp_mode_variant = CCP_PWM_MODE,
    #if (CCP1_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED) || \
        (CCP2_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
        .PWM_freq = 20000,
        .timer2_prescaler = 1,
        .timer2_postscaler = 1,
    #endif    
    .ccp_pin = {.port = PORTC_INDEX, .pin = GPIO_PIN1, 
                 .direction = GPIO_DIRECTION_OUTPUT, .logic = GPIO_LOW}

};

int main()
{
    application_initialize();
               
    ret = mcal_CCP_PWM_Set_Duty(&ccp1_obj, 25);
    ret = mcal_CCP_PWM_Set_Duty(&ccp2_obj, 75);
    
    while(1)
    {
//        for(ccp1_pwm_duty = 0; ccp1_pwm_duty <= 100; ccp1_pwm_duty += 5)
//        {
//            ret = mcal_CCP_PWM_Set_Duty(&ccp1_obj, ccp1_pwm_duty);
//            __delay_ms(100);
//        }
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ret = mcal_Timer2_Init(&timer2_timer_obj);
    ret = mcal_CCP_Init(&ccp1_obj);
    ret = mcal_CCP_Init(&ccp2_obj);
}
