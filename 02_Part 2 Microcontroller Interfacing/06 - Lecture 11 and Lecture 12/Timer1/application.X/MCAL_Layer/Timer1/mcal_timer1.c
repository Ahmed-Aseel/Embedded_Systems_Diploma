/* 
 * File:   mcal_timer1.c
 * Author: Ahmed Aseel
 *
 * Created on October 31, 2023, 9:40 PM
 */

#include "mcal_timer1.h"

#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static InterruptHandler Timer1_InterruptHandler = NULL;
#endif
  
static uint16 timer1_preload = 0;
    
static inline void Timer1_Mode_Select(const timer1_config_t* _timer1_config);    
static inline void Timer1_Register_Size_Config(const timer1_config_t* _timer1_config);    
static inline void Timer1_Preload_Config(uint16 preload_value);    

Std_ReturnType mcal_Timer1_Init(const timer1_config_t* _timer1_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer1_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer1 Module */
        TIMER1_MODULE_DISABLE();
        /* Configure timer1 mode (Timer / Counter) */
        Timer1_Mode_Select(_timer1_config);
        /* Configure timer1 registers size */
        Timer1_Register_Size_Config(_timer1_config);
        /* Configure timer1 prescaler */        
        TIMER1_PRESCALER_SELECT_BITS(_timer1_config->prescaler);
        /* Configure timer1 mode preload value */        
        Timer1_Preload_Config(_timer1_config->timer1_preload_val);
        
        /* Configure the interrupt */
        #if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            Timer1_InterruptHandler = _timer1_config->Timer1_InterruptHandler;
            Timer1_InterruptEnable();
            Timer1_InterruptFlagClear();
            
            #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_PRIORITY_HIGH == _timer1_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                    Timer1_HighPrioritySet(); 
                }
                else if(INTERRUPT_PRIORITY_LOW == _timer1_config->priority)
                { 
                    INTERRUPT_GlobalInterruptLowEnable();
                    Timer1_LowPrioritySet(); 
                }
                else{/* nothing */}
                
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
        #endif    
                
        /* Enable the Timer1 Module */
        TIMER1_MODULE_ENABLE();        
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer1_DeInit(const timer1_config_t* _timer1_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer1_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable Timer1 Module */
        TIMER1_MODULE_DISABLE();
        /* Disable the interrupt */
        #if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            Timer1_InterruptDisable();
        #endif        
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer1_Write_Value(const timer1_config_t* _timer1_config, uint16 value)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer1_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        Timer1_Preload_Config(value);
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer1_Read_Value(const timer1_config_t* _timer1_config, uint16* value)
{
    Std_ReturnType ret = E_OK;
    uint8 l_tmr1L = 0, l_tmr1H = 0;

    if(NULL == _timer1_config || NULL == value) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
       l_tmr1L = TMR1L;
       l_tmr1H = TMR1H;
       *value = (uint16)((l_tmr1H << 8) + l_tmr1L);
    }   
    
    return ret;      
}

static inline void Timer1_Mode_Select(const timer1_config_t* _timer1_config)
{
    if(TIMER1_TIMER_MODE == _timer1_config->timer1_mode)
    {
        TIMER1_TIMER_MODE_ENABLE();
    }
    else if(TIMER1_COUNTER_MODE == _timer1_config->timer1_mode)
    {
        TIMER1_COUNTER_MODE_ENABLE();
        
        /* If Counter mode is selected, Configure Timer1 External Clock 
         * Input T13CKI (on the rising edge) Synchronization Select bit 
         */
        if(TIMER1_SYNC_COUNTER == _timer1_config->timer1_counter_sync)
        {
            TIMER1_SYNC_COUNTER_ENABLE();
        }
        else if(TIMER1_ASYNC_COUNTER == _timer1_config->timer1_counter_sync)
        {
            TIMER1_ASYNC_COUNTER_ENABLE();
        }
        else {/* nothing */}    
        
        /* configure pin RC0/T13CKI to be input pin*/
        pin_config_t _T13CKI_config = {.port = PORTC_INDEX, .pin = GPIO_PIN0, 
                                      .direction = GPIO_DIRECTION_INPUT, .logic = GPIO_LOW};
        gpio_pin_direction_initialize(&_T13CKI_config);
    }
    else {/* nothing */}
}

static inline void Timer1_Register_Size_Config(const timer1_config_t* _timer1_config)
{
    if(TIMER1_8BIT_ENABLE == _timer1_config->timer1_reg_size)
    {
        TIMER1_8BIT_REGISTER();
    }
    else if(TIMER1_16BIT_ENABLE == _timer1_config->timer1_reg_size)
    {
        TIMER1_16BIT_REGISTER();
    }
    else {/* nothing */}       
}

static inline void Timer1_Preload_Config(uint16 preload_value)
{
    TMR1H = (uint8)(preload_value >> 8);
    TMR1L = (uint8)(preload_value & 0xFF);
    timer1_preload = preload_value;
}

void TIMER1_ISR(void)
{
    #if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        Timer1_InterruptFlagClear();
        /* write preload value again */
        Timer1_Preload_Config(timer1_preload);
        
        if(Timer1_InterruptHandler)
        {
            Timer1_InterruptHandler();
        }    
    #endif
}