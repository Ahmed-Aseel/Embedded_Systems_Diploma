/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

void ADC_App_ISR(void);

Std_ReturnType ret = E_NOT_OK;
uint16 adc1_res, adc2_res, adc3_res, adc4_res;

adc_config_t adc_obj = {
    .acquisition_time = ADC_12_TAD, 
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .adc_channel = ADC_CHANNEL_AN0, 
    .result_format = ADC_RESULT_RIGHT, 
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLE,
    
    #if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        .ADC_InterruptHandler = ADC_App_ISR,
        .priority = INTERRUPT_PRIORITY_HIGH,
    #endif     
};
        
void ADC_App_ISR(void)
{
    adc2_res++;
    ret = mcal_adc_GetConversionResult(&adc_obj, &adc1_res);
}

int main()
{
    application_initialize();
    
    while(1)
    {
//        ret = mcal_adc_GetConversion_Blocking(&adc_obj, ADC_CHANNEL_AN0, &adc1_res);
//        ret = mcal_adc_GetConversion_Blocking(&adc_obj, ADC_CHANNEL_AN1, &adc2_res);
//        ret = mcal_adc_GetConversion_Blocking(&adc_obj, ADC_CHANNEL_AN2, &adc3_res);
//        ret = mcal_adc_GetConversion_Blocking(&adc_obj, ADC_CHANNEL_AN3, &adc4_res);

        ret = mcal_adc_StartConversion_Interrupt(&adc_obj, ADC_CHANNEL_AN0);
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ret = mcal_adc_Init(&adc_obj);
}