/* 
 * File:   mcal_timer3.c
 * Author: Ahmed Aseel
 *
 * Created on November 2, 2023, 9:58 PM
 */

#include "mcal_timer3.h"

#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static InterruptHandler Timer3_InterruptHandler = NULL;
#endif
  
static uint16 timer3_preload = 0;
    
static inline void Timer3_Mode_Select(const timer3_config_t* _timer3_config);    
static inline void Timer3_RW_Mode_Config(const timer3_config_t* _timer3_config);    
static inline void Timer3_Preload_Config(uint16 preload_value);    

Std_ReturnType mcal_Timer3_Init(const timer3_config_t* _timer3_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer3_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer3 Module */
        TIMER3_MODULE_DISABLE();
        /* Configure timer3 mode (Timer / Counter) */
        Timer3_Mode_Select(_timer3_config);
        /* Configure timer3 registers read/write mode 
         * read/write of Timer3 in one 16-bit operation OR in two 8-bit operations
         */
        Timer3_RW_Mode_Config(_timer3_config);
        /* Configure timer3 prescaler */        
        TIMER3_PRESCALER_SELECT_BITS(_timer3_config->prescaler);
        /* Configure timer3 mode preload value */        
        Timer3_Preload_Config(_timer3_config->timer3_preload_val);
        
        /* Configure the interrupt */
        #if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            Timer3_InterruptHandler = _timer3_config->Timer3_InterruptHandler;
            Timer3_InterruptEnable();
            Timer3_InterruptFlagClear();
            
            #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_PRIORITY_HIGH == _timer3_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                    Timer3_HighPrioritySet(); 
                }
                else if(INTERRUPT_PRIORITY_LOW == _timer3_config->priority)
                { 
                    INTERRUPT_GlobalInterruptLowEnable();
                    Timer3_LowPrioritySet(); 
                }
                else{/* nothing */}
                
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
        #endif    
                
        /* Enable the Timer3 Module */
        TIMER3_MODULE_ENABLE();        
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer3_DeInit(const timer3_config_t* _timer3_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer3_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable Timer3 Module */
        TIMER3_MODULE_DISABLE();
        /* Disable the interrupt */
        #if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            Timer3_InterruptDisable();
        #endif        
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer3_Write_Value(const timer3_config_t* _timer3_config, uint16 value)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer3_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        Timer3_Preload_Config(value);
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer3_Read_Value(const timer3_config_t* _timer3_config, uint16* value)
{
    Std_ReturnType ret = E_OK;
    uint8 l_tmr3L = 0, l_tmr3H = 0;

    if(NULL == _timer3_config || NULL == value) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        l_tmr3L = TMR3L;
        l_tmr3H = TMR3H;
        *value = (uint16)((l_tmr3H << 8) + l_tmr3L);
    }   
    
    return ret;      
}

static inline void Timer3_Mode_Select(const timer3_config_t* _timer3_config)
{
    if(TIMER3_TIMER_MODE == _timer3_config->timer3_mode)
    {
        TIMER3_TIMER_MODE_ENABLE();
    }
    else if(TIMER3_COUNTER_MODE == _timer3_config->timer3_mode)
    {
        TIMER3_COUNTER_MODE_ENABLE();
        
        /* If Counter mode is selected, Configure Timer3 External Clock 
         * Input T13CKI (on the rising edge after the first
         * falling edge ) Synchronization Select bit 
         */
        if(TIMER3_SYNC_COUNTER == _timer3_config->timer3_counter_sync)
        {
            TIMER3_SYNC_COUNTER_ENABLE();
        }
        else if(TIMER3_ASYNC_COUNTER == _timer3_config->timer3_counter_sync)
        {
            TIMER3_ASYNC_COUNTER_ENABLE();
        }
        else {/* nothing */}    
        
        /* configure pin RC0/T13CKI to be input pin*/
        pin_config_t _T13CKI_config = {.port = PORTC_INDEX, .pin = GPIO_PIN0, 
                                      .direction = GPIO_DIRECTION_INPUT, .logic = GPIO_LOW};
        gpio_pin_direction_initialize(&_T13CKI_config);
    }
    else {/* nothing */}
}

static inline void Timer3_RW_Mode_Config(const timer3_config_t* _timer3_config)
{
    if(TIMER3_8BIT_RW_MODE == _timer3_config->timer3_reg_rw_mode)
    {
        TIMER3_8BIT_RW_MODE_ENABLE();
    }
    else if(TIMER3_16BIT_RW_MODE == _timer3_config->timer3_reg_rw_mode)
    {
        TIMER3_16BIT_RW_MODE_ENABLE();
    }
    else {/* nothing */}       
}

static inline void Timer3_Preload_Config(uint16 preload_value)
{
    TMR3H = (uint8)(preload_value >> 8);
    TMR3L = (uint8)(preload_value & 0xFF);
    timer3_preload = preload_value;
}

void TIMER3_ISR(void)
{
    #if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        Timer3_InterruptFlagClear();
        /* write preload value again */
        Timer3_Preload_Config(timer3_preload);
        
        if(Timer3_InterruptHandler)
        {
            Timer3_InterruptHandler();
        }    
    #endif
}