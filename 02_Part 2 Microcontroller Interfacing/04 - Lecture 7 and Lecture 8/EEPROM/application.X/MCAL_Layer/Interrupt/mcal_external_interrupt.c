/* 
 * File:   mcal_external_interrupt.c
 * Author: Ahmed Aseel
 *
 * Created on May 5, 2023, 4:24 PM
 */

#include "mcal_external_interrupt.h"

static InterruptHandler INT0_InterruptHandler = NULL;
static InterruptHandler INT1_InterruptHandler = NULL;
static InterruptHandler INT2_InterruptHandler = NULL;

static InterruptHandler RB4_InterruptHandler_Low = NULL;
static InterruptHandler RB4_InterruptHandler_High = NULL;
static InterruptHandler RB5_InterruptHandler_Low = NULL;
static InterruptHandler RB5_InterruptHandler_High = NULL;
static InterruptHandler RB6_InterruptHandler_Low = NULL;
static InterruptHandler RB6_InterruptHandler_High = NULL;
static InterruptHandler RB7_InterruptHandler_Low = NULL;
static InterruptHandler RB7_InterruptHandler_High = NULL;

static Std_ReturnType Interrupt_INTx_Enable(const INTx_config_t* _INTx_config);
static Std_ReturnType Interrupt_INTx_Disable(const INTx_config_t* _INTx_config);
static Std_ReturnType Interrupt_INTx_Priority_Init(const INTx_config_t* _INTx_config);
static Std_ReturnType Interrupt_INTx_Edge_Init(const INTx_config_t* _INTx_config);
static Std_ReturnType Interrupt_INTx_Pin_Init(const INTx_config_t* _INTx_config);
static Std_ReturnType Interrupt_INTx_Clear_Flag(const INTx_config_t* _INTx_config);
static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const INTx_config_t* _INTx_config);

static Std_ReturnType Interrupt_RBx_Enable(const RBx_intr_config_t* _RBx_intr_config);
static Std_ReturnType Interrupt_RBx_Disable(const RBx_intr_config_t* _RBx_intr_config);
static Std_ReturnType Interrupt_RBx_Priority_Init(const RBx_intr_config_t* _RBx_intr_config);
static Std_ReturnType Interrupt_RBx_Pin_Init(const RBx_intr_config_t* _RBx_intr_config);
static Std_ReturnType Interrupt_RBx_Clear_Flag(const RBx_intr_config_t* _RBx_intr_config);
static Std_ReturnType Interrupt_RBx_SetInterruptHandler(const RBx_intr_config_t* _RBx_intr_config);

Std_ReturnType mcal_interrupt_INTx_Init(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _INTx_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* disable external interrupt */
        ret = Interrupt_INTx_Disable(_INTx_config);
        /* clear interrupt flag: external interrupt didn't happen */
        ret |= Interrupt_INTx_Clear_Flag(_INTx_config);        
        /* configure external interrupt edge */
        ret |= Interrupt_INTx_Edge_Init(_INTx_config);
        /* configure external interrupt priority */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        ret |= Interrupt_INTx_Priority_Init(_INTx_config);
        #endif
        /* configure external interrupt I/O pin */
        ret |= Interrupt_INTx_Pin_Init(_INTx_config);
        /* configure default interrupt callback */
        ret |= Interrupt_INTx_SetInterruptHandler(_INTx_config);
        /* enable external interrupt */
        ret |= Interrupt_INTx_Enable(_INTx_config);
    }     
        
    return ret;      
}

void INT0_ISR(void)
{
    /* external INT0 occurred must clear interrupt flag  in software */
    EXT_INT0_InterruptFlagClear();
    
    /* Callback function will be called every time this ISR executed*/
    if(INT0_InterruptHandler)
    {
        INT0_InterruptHandler();
    }
    else {/* nothing */} 
}

void INT1_ISR(void)
{
    /* external INT1 occurred must clear interrupt flag  in software */
    EXT_INT1_InterruptFlagClear();
    
    /* Callback function will be called every time this ISR executed*/
    if(INT1_InterruptHandler)
    {
        INT1_InterruptHandler();
    }
    else {/* nothing */} 
}

void INT2_ISR(void)
{
    /* external INT2 occurred must clear interrupt flag  in software */
    EXT_INT2_InterruptFlagClear();
    
    /* Callback function will be called every time this ISR executed*/
    if(INT2_InterruptHandler)
    {
        INT2_InterruptHandler();
    }
    else {/* nothing */} 
}

Std_ReturnType mcal_interrupt_INTx_DeInit(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _INTx_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = Interrupt_INTx_Disable(_INTx_config);
    }     
        
    return ret;    
}


Std_ReturnType mcal_interrupt_RBx_Init(const RBx_intr_config_t* _RBx_intr_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _RBx_intr_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        /* disable external interrupt, RBx */
        ret = Interrupt_RBx_Disable(_RBx_intr_config);
        /* clear interrupt flag: external interrupt RBx didn't happen */
        ret |= Interrupt_RBx_Clear_Flag(_RBx_intr_config);        
        /* configure external interrupt RBx priority */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        ret |= Interrupt_RBx_Priority_Init(_RBx_intr_config);
        #endif
        /* configure external interrupt RBx I/O pin */
        ret |= Interrupt_RBx_Pin_Init(_RBx_intr_config);
        /* configure default interrupt callback */
        ret |= Interrupt_RBx_SetInterruptHandler(_RBx_intr_config);
        /* enable external interrupt, RBx */
        ret |= Interrupt_RBx_Enable(_RBx_intr_config);
    }     
        
    return ret;    
}

void RB4_ISR(uint8 RB4_Source)
{
    Std_ReturnType ret, value;

    /*
     Note 1: A mismatch condition will continue to set the RBIF bit. Reading PORTB will end the
     mismatch condition and allow the bit to be cleared.
     2: RB port change interrupts also require the individual pin IOCB enables.
     */
    ret = gpio_port_read_logic(PORTB_INDEX, &value);

    /* external interrupt RB5 occurred must clear interrupt flag  in software */
    EXT_RBx_InterruptFlagClear();
        
    /* Callback function will be called every time this ISR executed*/
    if(0 == RB4_Source)
    {
        if(RB4_InterruptHandler_Low)
        {
            RB4_InterruptHandler_Low();
        }
        else {/* nothing */} 
    }
    else if(1 == RB4_Source)
    {
        if(RB4_InterruptHandler_High)
        {
            RB4_InterruptHandler_High();
        }
        else {/* nothing */} 
    }
    else {/* nothing */} 
}

void RB5_ISR(uint8 RB5_Source)
{
    Std_ReturnType ret, value;

    /*
     Note 1: A mismatch condition will continue to set the RBIF bit. Reading PORTB will end the
     mismatch condition and allow the bit to be cleared.
     2: RB port change interrupts also require the individual pin IOCB enables.
     */
    ret = gpio_port_read_logic(PORTB_INDEX, &value);

    /* external interrupt RB5 occurred must clear interrupt flag  in software */
    EXT_RBx_InterruptFlagClear();
        
    /* Callback function will be called every time this ISR executed*/
    if(0 == RB5_Source)
    {
        if(RB5_InterruptHandler_Low)
        {
            RB5_InterruptHandler_Low();
        }
        else {/* nothing */} 
    }
    else if(1 == RB5_Source)
    {
        if(RB5_InterruptHandler_High)
        {
            RB5_InterruptHandler_High();
        }
        else {/* nothing */} 
    }
    else {/* nothing */}     
}

void RB6_ISR(uint8 RB6_Source)
{
    Std_ReturnType ret, value;
 
    /*
     Note 1: A mismatch condition will continue to set the RBIF bit. Reading PORTB will end the
     mismatch condition and allow the bit to be cleared.
     2: RB port change interrupts also require the individual pin IOCB enables.
     */
    ret = gpio_port_read_logic(PORTB_INDEX, &value);

    /* external interrupt RB6 occurred must clear interrupt flag  in software */
    EXT_RBx_InterruptFlagClear();
           
    /* Callback function will be called every time this ISR executed*/
    if(0 == RB6_Source)
    {
        if(RB6_InterruptHandler_Low)
        {
            RB6_InterruptHandler_Low();
        }
        else {/* nothing */} 
    }
    else if(1 == RB6_Source)
    {
        if(RB6_InterruptHandler_High)
        {
            RB6_InterruptHandler_High();
        }
        else {/* nothing */} 
    }
    else {/* nothing */}      
}

void RB7_ISR(uint8 RB7_Source)
{
    Std_ReturnType ret, value;

    /*
     Note 1: A mismatch condition will continue to set the RBIF bit. Reading PORTB will end the
     mismatch condition and allow the bit to be cleared.
     2: RB port change interrupts also require the individual pin IOCB enables.
     */
    ret = gpio_port_read_logic(PORTB_INDEX, &value);

    /* external interrupt RB7 occurred must clear interrupt flag  in software */
    EXT_RBx_InterruptFlagClear();
            
    /* Callback function will be called every time this ISR executed*/
    if(0 == RB7_Source)
    {
        if(RB7_InterruptHandler_Low)
        {
            RB7_InterruptHandler_Low();
        }
        else {/* nothing */} 
    }
    else if(1 == RB7_Source)
    {
        if(RB7_InterruptHandler_High)
        {
            RB7_InterruptHandler_High();
        }
        else {/* nothing */} 
    }
    else {/* nothing */}     
}

Std_ReturnType mcal_interrupt_RBx_DeInit(const RBx_intr_config_t* _RBx_intr_config)
{
    Std_ReturnType ret = E_OK;

    if(NULL == _RBx_intr_config) 
    {
        ret = E_NOT_OK;
    }
    
    else
    {
        ret = Interrupt_RBx_Disable(_RBx_intr_config);
    }     
        
    return ret;    
}

static Std_ReturnType Interrupt_INTx_Enable(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;
    
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        if(INTERRUPT_PRIORITY_LOW == _INTx_config->priority && INTERRUPT_EXTERNAL_INT0 != _INTx_config->source)
        {
            INTERRUPT_PriorityLevelsEnable();
            INTERRUPT_GlobalInterruptLowEnable();
        }
        else if(INTERRUPT_PRIORITY_HIGH == _INTx_config->priority)
        {
            if(INTERRUPT_EXTERNAL_INT0 != _INTx_config->source)
            {
                INTERRUPT_PriorityLevelsEnable();
            }
            else {/* nothing */}
            
            INTERRUPT_GlobalInterruptHighEnable();
        }
        else {/* nothing */}
    #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
    #endif
            
    switch(_INTx_config->source)
    {
        case INTERRUPT_EXTERNAL_INT0:
            EXT_INT0_InterruptEnable();
            break;

        case INTERRUPT_EXTERNAL_INT1:
            EXT_INT1_InterruptEnable();
            break;
            
        case INTERRUPT_EXTERNAL_INT2:
            EXT_INT2_InterruptEnable();
            break;
            
        default:
            ret = E_NOT_OK;
    }
    
    return ret;    
}

static Std_ReturnType Interrupt_INTx_Disable(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;

    switch(_INTx_config->source)
    {
        case INTERRUPT_EXTERNAL_INT0:
            EXT_INT0_InterruptDisable();
            break;

        case INTERRUPT_EXTERNAL_INT1:
            EXT_INT1_InterruptDisable();
            break;
            
        case INTERRUPT_EXTERNAL_INT2:
            EXT_INT2_InterruptDisable();
            break;
            
        default:
            ret = E_NOT_OK;
    }
    
    return ret;    
}

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
static Std_ReturnType Interrupt_INTx_Priority_Init(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;
    
    switch(_INTx_config->source)
    {
        case INTERRUPT_EXTERNAL_INT1:
            if(INTERRUPT_PRIORITY_LOW == _INTx_config->priority)
            {
                EXT_INT1_LowPrioritySet();
            }
            else if(INTERRUPT_PRIORITY_HIGH == _INTx_config->priority)
            {
                EXT_INT1_HighPrioritySet();
            }
            else {/* nothing */}
            break;
            
        case INTERRUPT_EXTERNAL_INT2:
            if(INTERRUPT_PRIORITY_LOW == _INTx_config->priority)
            {
                EXT_INT2_LowPrioritySet();
            }
            else if(INTERRUPT_PRIORITY_HIGH == _INTx_config->priority)
            {
                EXT_INT2_HighPrioritySet();
            }
            else {/* nothing */}
            break;
            
        default:
            ret = E_NOT_OK;
    }
        
    return ret;    
}
#endif

static Std_ReturnType Interrupt_INTx_Edge_Init(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;

    switch(_INTx_config->source)
    {
        case INTERRUPT_EXTERNAL_INT0:
            if(INTERRUPT_FALLING_EDGE == _INTx_config->edge)
            {
                EXT_INT0_FallingEdgeSet();
            }
            else if(INTERRUPT_RISING_EDGE == _INTx_config->edge)
            {
                EXT_INT0_RisingEdgeSet();
            }
            else {/* nothing */}
            break;

        case INTERRUPT_EXTERNAL_INT1:
            if(INTERRUPT_FALLING_EDGE == _INTx_config->edge)
            {
                EXT_INT1_FallingEdgeSet();
            }
            else if(INTERRUPT_RISING_EDGE == _INTx_config->edge)
            {
                EXT_INT1_RisingEdgeSet();
            }
            else {/* nothing */}            
            break;
            
        case INTERRUPT_EXTERNAL_INT2:
            if(INTERRUPT_FALLING_EDGE == _INTx_config->edge)
            {
                EXT_INT2_FallingEdgeSet();
            }
            else if(INTERRUPT_RISING_EDGE == _INTx_config->edge)
            {
                EXT_INT2_RisingEdgeSet();
            }
            else {/* nothing */}            
            break;
            
        default:
            ret = E_NOT_OK;
    }
        
    return ret;    
}

static Std_ReturnType Interrupt_INTx_Pin_Init(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;

    switch(_INTx_config->source)
    {
        case INTERRUPT_EXTERNAL_INT0:
            ADCON1bits.PCFG = 0b0011; /* configure pin AN12->INT0 to be digital bit */
            break;

        case INTERRUPT_EXTERNAL_INT1:
            ADCON1bits.PCFG = 0b0101; /* configure pin AN10->INT1 to be digital bit */
            break;
            
        case INTERRUPT_EXTERNAL_INT2:
            ADCON1bits.PCFG = 0b0111; /* configure pin AN8->INT3 to be digital bit */
            break;
            
        default:
            ret = E_NOT_OK;
    }
        
    ret |= gpio_pin_direction_initialize(&_INTx_config->INTx_pin);
    
    return ret;    
}

static Std_ReturnType Interrupt_INTx_Clear_Flag(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;

    switch(_INTx_config->source)
    {
        case INTERRUPT_EXTERNAL_INT0:
            EXT_INT0_InterruptFlagClear();
            break;

        case INTERRUPT_EXTERNAL_INT1:
            EXT_INT1_InterruptFlagClear();
            break;
            
        case INTERRUPT_EXTERNAL_INT2:
            EXT_INT2_InterruptFlagClear();
            break;
            
        default:
            ret = E_NOT_OK;
    }
    
    return ret;     
}

static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const INTx_config_t* _INTx_config)
{
    Std_ReturnType ret = E_OK;

    switch(_INTx_config->source)
    {
        case INTERRUPT_EXTERNAL_INT0:
            INT0_InterruptHandler = _INTx_config->EXT_InterruptHandler;
            break;

        case INTERRUPT_EXTERNAL_INT1:
            INT1_InterruptHandler = _INTx_config->EXT_InterruptHandler;
            break;
            
        case INTERRUPT_EXTERNAL_INT2:
            INT2_InterruptHandler = _INTx_config->EXT_InterruptHandler;
            break;
            
        default:
            ret = E_NOT_OK;
    }
    
    return ret;       
}

static Std_ReturnType Interrupt_RBx_Enable(const RBx_intr_config_t* _RBx_intr_config)
{
    Std_ReturnType ret = E_OK;
    
    EXT_RBx_InterruptEnable();
    
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        
        if(INTERRUPT_PRIORITY_LOW == _RBx_intr_config->priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
        }
        else if(INTERRUPT_PRIORITY_HIGH == _RBx_intr_config->priority)
        {
                INTERRUPT_GlobalInterruptHighEnable();
        }
        else {/* nothing */}
        
    #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
    #endif
        
    return ret;     
}

static Std_ReturnType Interrupt_RBx_Disable(const RBx_intr_config_t* _RBx_intr_config)
{
    Std_ReturnType ret = E_OK;
    
    ret = EXT_RBx_InterruptDisable();
    
    return ret;     
}

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
static Std_ReturnType Interrupt_RBx_Priority_Init(const RBx_intr_config_t* _RBx_intr_config)
{
    Std_ReturnType ret = E_OK;

    INTERRUPT_PriorityLevelsEnable();
      
    if(INTERRUPT_PRIORITY_LOW == _RBx_intr_config->priority)
    {
        EXT_RBx_LowPrioritySet();
    }
    else if(INTERRUPT_PRIORITY_HIGH == _RBx_intr_config->priority)
    {
        EXT_RBx_HighPrioritySet();
    }
    else {/* nothing */}

    return ret;     
}
#endif

static Std_ReturnType Interrupt_RBx_Pin_Init(const RBx_intr_config_t* _RBx_intr_config)
{
    Std_ReturnType ret = E_OK;
    
    if(GPIO_PIN4 == _RBx_intr_config->RBx_pin.pin)
    {
        ADCON1bits.PCFG = 0b0100; /* configure pin AN11->RB4 intr to be digital bit */
    }
    else { /* nothing */}
    
    ret = gpio_pin_direction_initialize(&_RBx_intr_config->RBx_pin);
    
    return ret;      
}

static Std_ReturnType Interrupt_RBx_Clear_Flag(const RBx_intr_config_t* _RBx_intr_config)
{
    Std_ReturnType ret = E_OK, value;
    
    /*
     Note 1: A mismatch condition will continue to set the RBIF bit. Reading PORTB will end the
     mismatch condition and allow the bit to be cleared.
     2: RB port change interrupts also require the individual pin IOCB enables.
     */
    ret = gpio_port_read_logic(PORTB_INDEX, &value);
    
    ret = EXT_RBx_InterruptFlagClear();
    
    return ret;     
}

static Std_ReturnType Interrupt_RBx_SetInterruptHandler(const RBx_intr_config_t* _RBx_intr_config)
{
    Std_ReturnType ret = E_OK;

    switch(_RBx_intr_config->RBx_pin.pin)
    {
        case GPIO_PIN4:
            RB4_InterruptHandler_Low = _RBx_intr_config->EXT_InterruptHandler_Low;
            RB4_InterruptHandler_High = _RBx_intr_config->EXT_InterruptHandler_High;
            break;

        case GPIO_PIN5:
            RB5_InterruptHandler_Low = _RBx_intr_config->EXT_InterruptHandler_Low;
            RB5_InterruptHandler_High = _RBx_intr_config->EXT_InterruptHandler_High;
            break;
            
        case GPIO_PIN6:
            RB6_InterruptHandler_Low = _RBx_intr_config->EXT_InterruptHandler_Low;
            RB6_InterruptHandler_High = _RBx_intr_config->EXT_InterruptHandler_High;
            break;
            
        case GPIO_PIN7:
            RB7_InterruptHandler_Low = _RBx_intr_config->EXT_InterruptHandler_Low;
            RB7_InterruptHandler_High = _RBx_intr_config->EXT_InterruptHandler_High;
            break;
                        
        default:
            ret = E_NOT_OK;
    }
    
    return ret;     
}
