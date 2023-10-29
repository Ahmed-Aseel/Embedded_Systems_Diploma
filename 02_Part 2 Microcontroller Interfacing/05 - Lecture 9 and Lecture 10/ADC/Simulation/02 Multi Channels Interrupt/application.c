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
uint8 adc1_res_txt[6], adc2_res_txt[6], adc3_res_txt[6], adc4_res_txt[6];
uint8 ADC_Req = 0;

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
    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW1, 7, "ADC Test");
    __delay_ms(2000);
    ret = ecu_lcd_4bits_send_command(&lcd_1, LCD_CLEAR);
    
    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW1, 1, "Pot1: ");
    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW2, 1, "Pot2: ");
    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW3, 1, "Pot3: ");
    ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW4, 1, "Pot4: ");
    
    
    while(1)
    {
        if(0 == ADC_Req){
            ret = mcal_adc_StartConversion_Interrupt(&adc_obj, ADC_CHANNEL_AN0);
        }
        else if(1 == ADC_Req){
            ret = mcal_adc_StartConversion_Interrupt(&adc_obj, ADC_CHANNEL_AN1);
        }
        else if(2 == ADC_Req){
            ret = mcal_adc_StartConversion_Interrupt(&adc_obj, ADC_CHANNEL_AN2);
        }
        else if(3 == ADC_Req){
            ret = mcal_adc_StartConversion_Interrupt(&adc_obj, ADC_CHANNEL_AN3);
        }
        else { /* Nothing */ }
        
        ret = convert_uint16_to_string(adc1_res, adc1_res_txt);
        ret = convert_uint16_to_string(adc2_res, adc2_res_txt);
        ret = convert_uint16_to_string(adc3_res, adc3_res_txt);
        ret = convert_uint16_to_string(adc4_res, adc4_res_txt);

        ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW1, 7, adc1_res_txt);
        ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW2, 7, adc2_res_txt);
        ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW3, 7, adc3_res_txt);
        ret = ecu_lcd_4bits_send_string_pos(&lcd_1, ROW4, 7, adc4_res_txt);

    }
    
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ret = mcal_adc_Init(&adc_obj);
    ecu_layer_initialize();
}
        
void ADC_App_ISR(void)
{
    if(0 == ADC_Req){
        ret = mcal_adc_GetConversionResult(&adc_obj, &adc1_res);
        ADC_Req = 1;
    }
    else if(1 == ADC_Req){
        ret = mcal_adc_GetConversionResult(&adc_obj, &adc2_res);
        ADC_Req = 2;
    }
    else if(2 == ADC_Req){
        ret = mcal_adc_GetConversionResult(&adc_obj, &adc3_res);
        ADC_Req = 3;
    }
    else if(3 == ADC_Req){
        ret = mcal_adc_GetConversionResult(&adc_obj, &adc4_res);
        ADC_Req = 0;
    }
    else { /* Nothing */ }    
}