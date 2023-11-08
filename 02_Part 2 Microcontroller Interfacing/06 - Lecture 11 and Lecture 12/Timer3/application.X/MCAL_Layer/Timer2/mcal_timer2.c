/* 
 * File:   mcal_timer2.c
 * Author: Ahmed Aseel
 *
 * Created on November 1, 2023, 11:41 PM
 */

#include "mcal_timer2.h"

#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static InterruptHandler Timer2_InterruptHandler = NULL;
#endif
  
static uint8 timer2_preload = 0;
      
static inline void Timer2_Preload_Config(uint8 preload_value);    

Std_ReturnType mcal_Timer2_Init(const timer2_config_t* _timer2_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer2_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer2 Module */
        TIMER2_MODULE_DISABLE();
        /* Configure timer2 prescaler */        
        TIMER2_PRESCALER_SELECT_BITS(_timer2_config->prescaler);
        /* Configure timer2 postscaler */        
        TIMER2_POSTSCALER_SELECT_BITS(_timer2_config->postscaler);
        /* Configure timer2 mode preload value */        
        Timer2_Preload_Config(_timer2_config->timer2_preload_val);
        
        /* Configure the interrupt */
        #if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            Timer2_InterruptHandler = _timer2_config->Timer2_InterruptHandler;
            Timer2_InterruptEnable();
            Timer2_InterruptFlagClear();
            
            #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_PRIORITY_HIGH == _timer2_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                    Timer2_HighPrioritySet(); 
                }
                else if(INTERRUPT_PRIORITY_LOW == _timer2_config->priority)
                { 
                    INTERRUPT_GlobalInterruptLowEnable();
                    Timer2_LowPrioritySet(); 
                }
                else{/* nothing */}
                
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
        #endif    
                
        /* Enable the Timer2 Module */
        TIMER2_MODULE_ENABLE();        
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer2_DeInit(const timer2_config_t* _timer2_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer2_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable Timer2 Module */
        TIMER2_MODULE_DISABLE();
        /* Disable the interrupt */
        #if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            Timer2_InterruptDisable();
        #endif        
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer2_Write_Value(const timer2_config_t* _timer2_config, uint8 value)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer2_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        Timer2_Preload_Config(value);
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer2_Read_Value(const timer2_config_t* _timer2_config, uint8* value)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer2_config || NULL == value) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
       *value = TMR2;
    }   
    
    return ret;      
}

static inline void Timer2_Preload_Config(uint8 preload_value)
{
    TMR2 = preload_value;
    timer2_preload = preload_value;
}

void TIMER2_ISR(void)
{
    #if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        Timer2_InterruptFlagClear();
        /* write preload value again */
        Timer2_Preload_Config(timer2_preload);
        
        if(Timer2_InterruptHandler)
        {
            Timer2_InterruptHandler();
        }    
    #endif
}
