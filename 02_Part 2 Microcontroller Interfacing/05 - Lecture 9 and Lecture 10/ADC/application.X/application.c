/* 
 * File:   application.c
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 6:23 PM
 */

#include "application.h"

void ADC_App_ISR(void);

Std_ReturnType ret = E_NOT_OK;
volatile uint16 lm35_res_1, lm35_res_2;
uint16 lm35_res_1_celsius, lm35_res_2_celsius;
uint8 lm35_res_1_txt[7], lm35_res_2_txt[7];
volatile uint8 ADC_Req = 0;

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

int main()
{
    application_initialize();
    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW1, 7, "LM35 Test");
    
    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW2, 1, "Temp1: ");
    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW3, 1, "Temp2: ");

    while(1)
    {
        if(0 == ADC_Req)
        {
            ret = mcal_adc_StartConversion_Interrupt(&adc_obj, ADC_CHANNEL_AN0);
        }
        else if(1 == ADC_Req)
        {
            ret = mcal_adc_StartConversion_Interrupt(&adc_obj, ADC_CHANNEL_AN1);
        }
        else { /* Nothing */ }
        
        /* the sensitivity of lm35 is 10mv per degree celsius 
         * ex:- 250mv ----> 25C
         */
        lm35_res_1_celsius = lm35_res_1 * 4.883f; // step size = 5000mv / 1024 = 4.8828125
        lm35_res_1_celsius /= 10;

        lm35_res_2_celsius = lm35_res_2 * 4.883f;
        lm35_res_2_celsius /= 10;  
        
        ret = convert_uint16_to_string(lm35_res_1_celsius, lm35_res_1_txt);      
        ret = convert_uint16_to_string(lm35_res_2_celsius, lm35_res_2_txt);

        ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW2, 7, lm35_res_1_txt);
        ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW3, 7, lm35_res_2_txt);
        
        if(lm35_res_1_celsius > 20){
            ret = ecu_dc_motor_move_right(&dc_motor_1);
        }
        else{
            ret = ecu_dc_motor_stop(&dc_motor_1);
        }
        
        if(lm35_res_2_celsius > 25){
            ret = ecu_dc_motor_move_left(&dc_motor_2);
        }
        else{
            ret = ecu_dc_motor_stop(&dc_motor_2);
        }        
    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
    ret = mcal_adc_Init(&adc_obj);
}
        
void ADC_App_ISR(void)
{
    if(0 == ADC_Req)
    {
        ret = mcal_adc_GetConversionResult(&adc_obj, &lm35_res_1);
        ADC_Req = 1;
    }
    else if(1 == ADC_Req)
    {
        ret = mcal_adc_GetConversionResult(&adc_obj, &lm35_res_2);
        ADC_Req = 0;
    }
    else { /* Nothing */ }    
}
