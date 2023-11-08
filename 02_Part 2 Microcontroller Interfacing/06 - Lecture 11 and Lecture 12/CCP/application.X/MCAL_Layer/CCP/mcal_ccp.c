/* 
 * File:   mcal_ccp.c
 * Author: Ahmed Aseel
 *
 * Created on November 5, 2023, 6:18 PM
 */

#include "mcal_ccp.h"

#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static InterruptHandler CCP1_InterruptHandler = NULL;
#endif

#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static InterruptHandler CCP2_InterruptHandler = NULL;
#endif    
    
static inline Std_ReturnType CCP_Disable(const ccp_config_t* _ccp_config);   
static inline Std_ReturnType CCP_Mode_Select(const ccp_config_t* _ccp_config);   
static inline Std_ReturnType CCP_Capture_Mode_Select(const ccp_config_t* _ccp_config);   
static inline Std_ReturnType CCP_Compare_Mode_Select(const ccp_config_t* _ccp_config);   
static inline Std_ReturnType CCP_PWM_Mode_Select(const ccp_config_t* _ccp_config);   
static inline Std_ReturnType CCP_Cpt_Cmp_Timer_Select(const ccp_config_t* _ccp_config);   
static inline Std_ReturnType CCP_Intr_Config(const ccp_config_t* _ccp_config);

Std_ReturnType mcal_CCP_Init(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _ccp_config) 
    {
        ret = E_NOT_OK;
    }
  
    else
    { 
        /* Disable CCP Module */
        ret = CCP_Disable(_ccp_config);
 
        /* Configure CCP mode */
        ret = CCP_Mode_Select(_ccp_config);
                
        /* Pin Configuration 
         * In case of CCP2 its pin can be multiplexed 
         * with either PORTB or PORTC through CONFIG3H<CCP2MX> default is PORTC
         * In case of Capture mode pin must be configured as input
         */
        ret = gpio_pin_initialize(&_ccp_config->ccp_pin);

        /* Configure the interrupt */
        ret = CCP_Intr_Config(_ccp_config);
    }
    
    return ret;      
}

Std_ReturnType mcal_CCP_DeInit(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _ccp_config) 
    {
        ret = E_NOT_OK;
    }
     
    else
    {
        ret = CCP_Disable(_ccp_config);
    }
        
    return ret;       
}

#if (CCP1_CFG_MODE_SELECTED == CCP_CFG_CAPTURE_MODE_SELECTED) || \
    (CCP2_CFG_MODE_SELECTED == CCP_CFG_CAPTURE_MODE_SELECTED)
Std_ReturnType mcal_CCP_IsCapturedDataReady(const ccp_config_t* _ccp_config, uint8* capture_status)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _ccp_config || NULL == capture_status) 
    {
        ret = E_NOT_OK;
    }
     
    else
    {
        if(CCP1_INST == _ccp_config->ccp_inst)
        {
            if(CCP_CAPTURE_READY == PIR1bits.CCP1IF)
            {
                *capture_status = CCP_CAPTURE_READY;
                CCP1_InterruptFlagClear();
            }
            else
            {
                *capture_status = CCP_CAPTURE_NOT_READY;            
            } 
        }
        else if(CCP2_INST == _ccp_config->ccp_inst)
        {
            if(CCP_CAPTURE_READY == PIR2bits.CCP2IF)
            {
                *capture_status = CCP_CAPTURE_READY;
                CCP2_InterruptFlagClear();
            }
            else
            {
                *capture_status = CCP_CAPTURE_NOT_READY;            
            }       
        }
        else 
        {
            ret = E_NOT_OK;           
        }   
    }
        
    return ret;       
}

Std_ReturnType mcal_CCP_Capture_Mode_Read_Value(const ccp_config_t* _ccp_config, uint16* capture_value)
{
    Std_ReturnType ret = E_OK;
    CCP_REG_t capture_temp_val = {.ccpr_low = 0, .ccpr_high = 0};
    
    if(NULL == _ccp_config || NULL == capture_value) 
    {
        ret = E_NOT_OK;
    }
     
    else
    {
        if(CCP1_INST == _ccp_config->ccp_inst)
        {
            capture_temp_val.ccpr_low  = CCPR1L;
            capture_temp_val.ccpr_high = CCPR1H;
            *capture_value = capture_temp_val.ccpr_16bits;       
        }
        else if(CCP2_INST == _ccp_config->ccp_inst)
        {
            capture_temp_val.ccpr_low  = CCPR2L;
            capture_temp_val.ccpr_high = CCPR2H;
            *capture_value = capture_temp_val.ccpr_16bits;        
        }
        else 
        {
            ret = E_NOT_OK;           
        }
    }
        
    return ret;       
}
#endif

#if (CCP1_CFG_MODE_SELECTED == CCP_CFG_COMPARE_MODE_SELECTED) || \
    (CCP2_CFG_MODE_SELECTED == CCP_CFG_COMPARE_MODE_SELECTED)
Std_ReturnType mcal_CCP_Compare_Mode_Set_Value(const ccp_config_t* _ccp_config, uint16 compare_value)
{
    Std_ReturnType ret = E_OK;
    CCP_REG_t capture_temp_val = {.ccpr_low = 0, .ccpr_high = 0};

    if(NULL == _ccp_config) 
    {
        ret = E_NOT_OK;
    }
     
    else
    {
        capture_temp_val.ccpr_16bits = compare_value;

        if(CCP1_INST == _ccp_config->ccp_inst)
        {
            CCPR1L = capture_temp_val.ccpr_low;
            CCPR1H = capture_temp_val.ccpr_high;       
        }
        else if(CCP2_INST == _ccp_config->ccp_inst)
        {
            CCPR2L = capture_temp_val.ccpr_low;
            CCPR2H = capture_temp_val.ccpr_high;       
        }
        else 
        {
            ret = E_NOT_OK;           
        }
    }
        
    return ret;       
}

Std_ReturnType mcal_CCP_IsCompareComplete(const ccp_config_t* _ccp_config, uint8* compare_status)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _ccp_config || NULL == compare_status) 
    {
        ret = E_NOT_OK;
    }
     
    else
    {
        if(CCP1_INST == _ccp_config->ccp_inst)
        {
            if(CCP_COMPARE_READY == PIR1bits.CCP1IF)
            {
                *compare_status = CCP_COMPARE_READY;
                CCP1_InterruptFlagClear();
            }
            else
            {
                *compare_status = CCP_COMPARE_NOT_READY;
            } 
        }
        else if(CCP2_INST == _ccp_config->ccp_inst)
        {
            if(CCP_COMPARE_READY == PIR2bits.CCP2IF)
            {
                *compare_status = CCP_COMPARE_READY;
                CCP2_InterruptFlagClear();
            }
            else
            {
                *compare_status = CCP_COMPARE_NOT_READY;            
            }       
        }
        else 
        {
            ret = E_NOT_OK;           
        }       
    }
        
    return ret;       
}
#endif

#if (CCP1_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED) || \
    (CCP2_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
Std_ReturnType mcal_CCP_PWM_Set_Duty(const ccp_config_t* _ccp_config, const uint8 duty)
{
    Std_ReturnType ret = E_OK;
    uint16 duty_temp = 0;
        
    if(NULL == _ccp_config) 
    {
        ret = E_NOT_OK;
    }
     
    else
    {
        duty_temp = (uint16)(4 * (PR2 + 1) * (duty / 100.0));

        if(CCP1_INST == _ccp_config->ccp_inst)
        {
            CCP1CONbits.DC1B = (uint8)(duty_temp & 0x0003);
            CCPR1L = (uint8)(duty_temp >> 2);        
        }
        else if(CCP2_INST == _ccp_config->ccp_inst)
        {
            CCP2CONbits.DC2B = (uint8)(duty_temp & 0x0003);
            CCPR2L = (uint8)(duty_temp >> 2);        
        }
        else 
        {
            ret = E_NOT_OK;           
        }
    }
        
    return ret;       
}

Std_ReturnType mcal_CCP_PWM_Start(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _ccp_config) 
    {
        ret = E_NOT_OK;
    }
     
    else
    {
        if(CCP1_INST == _ccp_config->ccp_inst)
        {
            CCP1_SET_MODE(CCP_PWM_MODE);        
        }
        else if(CCP2_INST == _ccp_config->ccp_inst)
        {
            CCP2_SET_MODE(CCP_PWM_MODE);        
        }
        else 
        {
            ret = E_NOT_OK;           
        }       
    }
        
    return ret;       
}

Std_ReturnType mcal_CCP_PWM_Stop(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _ccp_config) 
    {
        ret = E_NOT_OK;
    }
     
    else
    {
        if(CCP1_INST == _ccp_config->ccp_inst)
        {
            CCP1_SET_MODE(CCP_MODULE_DISABLE);        
        }
        else if(CCP2_INST == _ccp_config->ccp_inst)
        {
            CCP2_SET_MODE(CCP_MODULE_DISABLE);        
        }
        else 
        {
            ret = E_NOT_OK;           
        }
    }
        
    return ret;       
}
#endif

static inline Std_ReturnType CCP_Disable(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    if(CCP1_INST == _ccp_config->ccp_inst)
    {
        CCP1_SET_MODE(CCP_MODULE_DISABLE);        
        #if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptDisable();
        #endif   
    }
    else if(CCP2_INST == _ccp_config->ccp_inst)
    {
        CCP2_SET_MODE(CCP_MODULE_DISABLE);        
        #if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptDisable();
        #endif   
    }
    else 
    {
        ret = E_NOT_OK;           
    }   

    return ret;       
}

static inline Std_ReturnType CCP_Capture_Mode_Select(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    switch(_ccp_config->ccp_mode_variant)
    {
        case CCP_CAPTURE_MODE_1_FALLING_EDGE:
        case CCP_CAPTURE_MODE_1_RISING_EDGE:
        case CCP_CAPTURE_MODE_4_RISING_EDGE:
        case CCP_CAPTURE_MODE_16_RISING_EDGE:
            if(CCP1_INST == _ccp_config->ccp_inst)
            {
                CCP1_SET_MODE(_ccp_config->ccp_mode_variant);        
            }
            else if(CCP2_INST == _ccp_config->ccp_inst)
            {
                CCP2_SET_MODE(_ccp_config->ccp_mode_variant);       
            }
            else{/* nothing */}
            break;
                     
        default: 
            ret = E_NOT_OK;
    }    

    return ret;       
}

static inline Std_ReturnType CCP_Compare_Mode_Select(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    switch(_ccp_config->ccp_mode_variant)
    {
        case CCP_COMPARE_MODE_SET_PIN_LOW:
        case CCP_COMPARE_MODE_SET_PIN_HIGH:
        case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
        case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
        case CCP_COMPARE_MODE_GEN_EVENT:
            if(CCP1_INST == _ccp_config->ccp_inst)
            {
                CCP1_SET_MODE(_ccp_config->ccp_mode_variant);        
            }
            else if(CCP2_INST == _ccp_config->ccp_inst)
            {
                CCP2_SET_MODE(_ccp_config->ccp_mode_variant);       
            }
            else{/* nothing */}
            break;
                   
        default: 
            ret = E_NOT_OK;
    } 
    
    return ret;       
}

static inline Std_ReturnType CCP_PWM_Mode_Select(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    #if (CCP1_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED) || \
        (CCP2_CFG_MODE_SELECTED == CCP_CFG_PWM_MODE_SELECTED)
        
        if(CCP_PWM_MODE == _ccp_config->ccp_mode_variant)
        {
            if(CCP1_INST == _ccp_config->ccp_inst)
            {
                CCP1_SET_MODE(_ccp_config->ccp_mode_variant);        
            }
            else if(CCP2_INST == _ccp_config->ccp_inst)
            {
                CCP2_SET_MODE(_ccp_config->ccp_mode_variant);       
            }
            else{/* nothing */}

            /* PR2 is common between CCP1 & CCP2 so they will have the 
            * same frequency but each one can have different duty cycle
            */
            PR2 = (uint8)((_XTAL_FREQ / (_ccp_config->PWM_freq * 4.0 * 
                           _ccp_config->timer2_prescaler * _ccp_config->timer2_postscaler)) - 1);
        }
        else
        {
            ret = E_NOT_OK;                  
        }
    #endif  
    
    return ret;        
}

static inline Std_ReturnType CCP_Mode_Select(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;
    
    if(CCP_CAPTURE_MODE_SELECT == _ccp_config->ccp_mode)
    {
        ret = CCP_Capture_Mode_Select(_ccp_config);
        ret = CCP_Cpt_Cmp_Timer_Select(_ccp_config);
    }
    else if(CCP_COMPARE_MODE_SELECT == _ccp_config->ccp_mode)
    {
        ret = CCP_Compare_Mode_Select(_ccp_config);
        ret = CCP_Cpt_Cmp_Timer_Select(_ccp_config);
    }
    else if(CCP_PWM_MODE_SELECT == _ccp_config->ccp_mode)
    {
        ret = CCP_PWM_Mode_Select(_ccp_config);
    }
    else
    {
        ret = E_NOT_OK;                  
    }
    
    return ret;       
}

static inline Std_ReturnType CCP_Cpt_Cmp_Timer_Select(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    if(CCP1_CCP2_TIMER1 == _ccp_config->ccp_timer)
    {
        CCP1_CCP2_TIMER1_SELECT();
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp_config->ccp_timer)
    {
        CCP1_TIMER1_CCP2_TIMER3_SELECT();
    }
    else if(CCP1_CCP2_TIMER3 == _ccp_config->ccp_timer)
    {
        CCP1_CCP2_TIMER3_SELECT();
    }
    else
    {
        ret = E_NOT_OK;                          
    }   
    
    return ret;       
}

static inline Std_ReturnType CCP_Intr_Config(const ccp_config_t* _ccp_config)
{
    Std_ReturnType ret = E_OK;

    if(CCP1_INST == _ccp_config->ccp_inst)
    {
        #if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptHandler = _ccp_config->CCP_InterruptHandler;
            CCP1_InterruptEnable();
            CCP1_InterruptFlagClear();
            
            #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_PRIORITY_HIGH == _ccp_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                    CCP1_HighPrioritySet(); 
                }
                else if(INTERRUPT_PRIORITY_LOW == _ccp_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable(); /* enable global interrupt */
                    INTERRUPT_GlobalInterruptLowEnable();
                    CCP1_LowPrioritySet(); 
                }
                else{/* nothing */}
                
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
        #endif 
    }
    else if(CCP2_INST == _ccp_config->ccp_inst)
    {
        #if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptHandler = _ccp_config->CCP_InterruptHandler;
            CCP2_InterruptEnable();
            CCP2_InterruptFlagClear();
            
            #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_PRIORITY_HIGH == _ccp_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                    CCP2_HighPrioritySet(); 
                }
                else if(INTERRUPT_PRIORITY_LOW == _ccp_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable(); /* enable global interrupt */
                    INTERRUPT_GlobalInterruptLowEnable();
                    CCP2_LowPrioritySet(); 
                }
                else{/* nothing */}
                
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
        #endif 
    }
    else{/* nothing */}   
    
    return ret;
}

void CCP1_ISR(void)
{
    #if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        CCP1_InterruptFlagClear();
        if(CCP1_InterruptHandler)
        {
            CCP1_InterruptHandler();
        }    
    #endif
}

void CCP2_ISR(void)
{
    #if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        CCP2_InterruptFlagClear();
        if(CCP2_InterruptHandler)
        {
            CCP2_InterruptHandler();
        }    
    #endif
}