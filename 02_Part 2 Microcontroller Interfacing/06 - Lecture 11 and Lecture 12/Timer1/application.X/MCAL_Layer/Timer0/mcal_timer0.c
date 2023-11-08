/* 
 * File:   mcal_timer0.c
 * Author: Ahmed Aseel
 *
 * Created on October 30, 2023, 11:04 AM
 */

#include "mcal_timer0.h"

#if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static InterruptHandler Timer0_InterruptHandler = NULL;
#endif
  
static uint16 timer0_preload = 0;
    
static inline void Timer0_Mode_Select(const timer0_config_t* _timer0_config);    
static inline void Timer0_Register_Size_Config(const timer0_config_t* _timer0_config);    
static inline void Timer0_Prescaler_Config(const timer0_config_t* _timer0_config);    
static inline void Timer0_Preload_Config(uint16 preload_value);    

Std_ReturnType mcal_Timer0_Init(const timer0_config_t* _timer0_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer0_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable the Timer0 Module */
        TIMER0_MODULE_DISABLE();
        /* Configure timer0 mode (Timer / Counter) */
        Timer0_Mode_Select(_timer0_config);
        /* Configure timer0 registers size */
        Timer0_Register_Size_Config(_timer0_config);
        /* Configure timer0 prescaler if needed */        
        Timer0_Prescaler_Config(_timer0_config);
        /* Configure timer0 mode preload value */        
        Timer0_Preload_Config(_timer0_config->timer0_preload_val);
        
        /* Configure the interrupt */
        #if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            Timer0_InterruptHandler = _timer0_config->Timer0_InterruptHandler;
            Timer0_InterruptEnable();
            Timer0_InterruptFlagClear();
            
            #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_PRIORITY_HIGH == _timer0_config->priority)
                { 
                    INTERRUPT_GlobalInterruptHighEnable();
                    Timer0_HighPrioritySet(); 
                }
                else if(INTERRUPT_PRIORITY_LOW == _timer0_config->priority)
                { 
                    INTERRUPT_GlobalInterruptLowEnable();
                    Timer0_LowPrioritySet(); 
                }
                else{/* nothing */}
                
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
        #endif    
                
        /* Enable the Timer0 Module */
        TIMER0_MODULE_ENABLE();        
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer0_DeInit(const timer0_config_t* _timer0_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer0_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* Disable Timer0 Module */
        TIMER0_MODULE_DISABLE();
        /* Disable the interrupt */
        #if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            Timer0_InterruptDisable();
        #endif        
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer0_Write_Value(const timer0_config_t* _timer0_config, uint16 value)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _timer0_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        Timer0_Preload_Config(value);
    }   
    
    return ret;      
}

Std_ReturnType mcal_Timer0_Read_Value(const timer0_config_t* _timer0_config, uint16* value)
{
    Std_ReturnType ret = E_OK;
    uint8 l_tmroL = 0, l_tmroH = 0;

    if(NULL == _timer0_config || NULL == value) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
       l_tmroL = TMR0L;
       l_tmroH = TMR0H;
       *value = (uint16)((l_tmroH << 8) + l_tmroL);
    }   
    
    return ret;      
}

static inline void Timer0_Mode_Select(const timer0_config_t* _timer0_config)
{    
    if(TIMER0_TIMER_MODE == _timer0_config->timer0_mode)
    {
        TIMER0_TIMER_MODE_ENABLE();
    }
    else if(TIMER0_COUNTER_MODE == _timer0_config->timer0_mode)
    {
        TIMER0_COUNTER_MODE_ENABLE();
        
        /* If Counter mode is selected, Timer0 increments either
         * on every rising or falling edge of pin RA4/T0CKI/C1OUT 
         */
        if(TIMER0_RISING_EDGE_CFG == _timer0_config->timer0_counter_edge)
        {
            TIMER0_COUNTER_RISING_EDGE();
        }
        else if(TIMER0_FALLING_EDGE_CFG == _timer0_config->timer0_counter_edge)
        {
            TIMER0_COUNTER_FALLING_EDGE();
        }
        else {/* nothing */}    
        
        /* configure pin RA4/T0CKI/C1OUT to be input pin*/
        pin_config_t _T0CKI_config = {.port = PORTA_INDEX, .pin = GPIO_PIN4, 
                                      .direction = GPIO_DIRECTION_INPUT, .logic = GPIO_LOW};
        gpio_pin_direction_initialize(&_T0CKI_config);
    }
    else {/* nothing */}
}

static inline void Timer0_Register_Size_Config(const timer0_config_t* _timer0_config)
{
    if(TIMER0_8BIT_ENABLE == _timer0_config->timer0_reg_size)
    {
        TIMER0_8BIT_REGISTER();
    }
    else if(TIMER0_16BIT_ENABLE == _timer0_config->timer0_reg_size)
    {
        TIMER0_16BIT_REGISTER();
    }
    else {/* nothing */}       
}

static inline void Timer0_Prescaler_Config(const timer0_config_t* _timer0_config)
{
    if(TIMER0_PRESCALER_DISABLE_CFG == _timer0_config->timer0_prescaler_enable)
    {
        TIMER0_PRESCALER_DISABLE();
    }
    else if(TIMER0_PRESCALER_ENABLE_CFG == _timer0_config->timer0_prescaler_enable)
    {
        TIMER0_PRESCALER_ENABLE();
        TIMER0_PRESCALER_SELECT_BITS(_timer0_config->prescaler);
    }
    else {/* nothing */}     
}

static inline void Timer0_Preload_Config(uint16 preload_value)
{
    TMR0H = (uint8)(preload_value >> 8);
    TMR0L = (uint8)(preload_value & 0xFF);
    timer0_preload = preload_value;
}

void TIMER0_ISR(void)
{
    #if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        Timer0_InterruptFlagClear();
        /* write preload value again */
        Timer0_Preload_Config(timer0_preload);

        if(Timer0_InterruptHandler)
        {
            Timer0_InterruptHandler();
        }    
    #endif
}
