/* 
 * File:   mcal_adc.c
 * Author: Ahmed Aseel
 *
 * Created on May 30, 2023, 6:30 PM
 */

#include "mcal_adc.h"

#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static InterruptHandler ADC_InterruptHandler = NULL;
#endif

static inline void adc_input_channel_port_configure(adc_channel_select_t channel);
static inline void adc_select_result_format(const adc_config_t *_adc_config);
static inline void adc_voltage_reference_configure(const adc_config_t *_adc_config);

Std_ReturnType mcal_adc_Init(const adc_config_t* _adc_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _adc_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        /* Configure the acquisition time */
        ADCON2bits.ACQT = _adc_config->acquisition_time;
        /* Configure the conversion clock */
        ADCON2bits.ADCS = _adc_config->conversion_clock;
        /* Configure the default channel */
        ADCON0bits.CHS = _adc_config->adc_channel;
        adc_input_channel_port_configure(_adc_config->adc_channel);
        
        /* Configure the interrupt */
        #if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            ADC_InterruptHandler = _adc_config->ADC_InterruptHandler;
            ADC_InterruptEnable();
            ADC_InterruptFlagClear();
            
            #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_PRIORITY_HIGH == _adc_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                    ADC_HighPrioritySet(); 
                }
                else if(INTERRUPT_PRIORITY_LOW == _adc_config->priority)
                { 
                    INTERRUPT_GlobalInterruptLowEnable();
                    ADC_LowPrioritySet(); 
                }
                else{/* nothing */}
                
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
        #endif

        /* Configure the result format */
        adc_select_result_format(_adc_config);
        /* Configure the voltage reference */
        adc_select_result_format(_adc_config);
        /* Enable the ADC */
        ADC_CONVERTER_ENABLE();
    }     
        
    return ret;      
}

Std_ReturnType mcal_adc_DeInit(const adc_config_t* _adc_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _adc_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        /* Disable the interrupt */
        #if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            ADC_InterruptDisable();
        #endif
    }     
        
    return ret;     
}

Std_ReturnType mcal_adc_SelectChannel(const adc_config_t* _adc_config, adc_channel_select_t channel)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _adc_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Configure the default channel */
        ADCON0bits.CHS = channel;
        adc_input_channel_port_configure(channel);
    }     
        
    return ret;     
}

Std_ReturnType mcal_adc_StartConversion(const adc_config_t* _adc_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _adc_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ADC_START_CONVERSION();
    }     
        
    return ret;     
}

Std_ReturnType mcal_adc_IsConversionDone(const adc_config_t* _adc_config, uint8* conversion_status)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _adc_config || NULL == conversion_status) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        *conversion_status = (uint8)(!(ADCON0bits.GO_nDONE));
    }     
        
    return ret;     
}

Std_ReturnType mcal_adc_GetConversionResult(const adc_config_t* _adc_config, adc_result_t* conversion_result)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _adc_config || NULL == conversion_result) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        if(ADC_RESULT_RIGHT == _adc_config->result_format)
        {
            *conversion_result = (adc_result_t)((ADRESH << 8) + ADRESL);
        }
        else if(ADC_RESULT_LEFT == _adc_config->result_format)
        {
            *conversion_result = (adc_result_t)(((ADRESH << 8) + ADRESL) >> 6);
        }
        else
        {
            *conversion_result = (adc_result_t)((ADRESH << 8) + ADRESL);
        }
    }     
        
    return ret;     
}

Std_ReturnType mcal_adc_GetConversion_Blocking(const adc_config_t* _adc_config, adc_channel_select_t channel, 
                                               adc_result_t* conversion_result)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _adc_config || NULL == conversion_result) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* select the A/D channel */
        ret &= mcal_adc_SelectChannel(_adc_config, channel);
        /* Start the conversion */
        ret &= mcal_adc_StartConversion(_adc_config);
        /* Check if conversion is completed */
        while(ADCON0bits.GO_nDONE);
        ret &= mcal_adc_GetConversionResult(_adc_config, conversion_result);
    }     
        
    return ret;     
}

#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
Std_ReturnType mcal_adc_StartConversion_Interrupt(const adc_config_t* _adc_config, adc_channel_select_t channel)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _adc_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ADC_InterruptFlagClear();
        /* select the A/D channel */
        ret &= mcal_adc_SelectChannel(_adc_config, channel);
        /* Start the conversion */
        ret &= mcal_adc_StartConversion(_adc_config);
    }     
        
    return ret;     
}
#endif

static inline void adc_input_channel_port_configure(adc_channel_select_t channel)
{
    switch(channel)
    {
        case ADC_CHANNEL_AN0  : SET_BIT(TRISA, _TRISA_RA0_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN1  : SET_BIT(TRISA, _TRISA_RA1_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN2  : SET_BIT(TRISA, _TRISA_RA2_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN3  : SET_BIT(TRISA, _TRISA_RA3_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN4  : SET_BIT(TRISA, _TRISA_RA5_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN5  : SET_BIT(TRISE, _TRISE_RE0_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN6  : SET_BIT(TRISE, _TRISE_RE1_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN7  : SET_BIT(TRISE, _TRISE_RE2_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN8  : SET_BIT(TRISB, _TRISB_RB2_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN9  : SET_BIT(TRISB, _TRISB_RB3_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN10 : SET_BIT(TRISB, _TRISB_RB1_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN11 : SET_BIT(TRISB, _TRISB_RB4_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL_AN12 : SET_BIT(TRISB, _TRISB_RB0_POSN); break; /* Disable the digital output driver */
        default               : /* Nothing */;
    }    
}

static inline void adc_select_result_format(const adc_config_t *_adc_config)
{
    if(ADC_RESULT_RIGHT == _adc_config->result_format)
    {
        ADC_RESULT_RIGHT_FORMAT();
    }
    else if(ADC_RESULT_LEFT == _adc_config->result_format)
    {
        ADC_RESULT_LEFT_FORMAT();
    }
    else
    {
        ADC_RESULT_RIGHT_FORMAT();
    }    
}

static inline void adc_voltage_reference_configure(const adc_config_t *_adc_config)
{
    if(ADC_VOLTAGE_REFERENCE_ENABLE == _adc_config->voltage_reference)
    {
        ADC_ENABLE_VOLTAGE_REFERENCE();
    }
    else if(ADC_VOLTAGE_REFERENCE_DISABLE == _adc_config->voltage_reference)
    {
        ADC_DISABLE_VOLTAGE_REFERENCE();
    }
    else
    {
        ADC_DISABLE_VOLTAGE_REFERENCE();
    }    
}

void ADC_ISR(void)
{
    #if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        ADC_InterruptFlagClear();
        if(ADC_InterruptHandler)
        {
            ADC_InterruptHandler();
        }    
    #endif
}
