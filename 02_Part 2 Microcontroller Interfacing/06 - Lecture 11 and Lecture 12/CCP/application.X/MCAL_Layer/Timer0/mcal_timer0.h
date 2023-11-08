/* 
 * File:   mcal_timer0.h
 * Author: Ahmed Aseel
 *
 * Created on October 30, 2023, 11:04 AM
 */

#ifndef MCAL_TIMER0_H
#define	MCAL_TIMER0_H

/* Section: Includes */
#include "../pic18.h"
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/* Section: Macro Declarations */
#define TIMER0_TIMER_MODE               0
#define TIMER0_COUNTER_MODE             1

#define TIMER0_8BIT_ENABLE              1
#define TIMER0_16BIT_ENABLE             0

#define TIMER0_PRESCALER_ENABLE_CFG     1
#define TIMER0_PRESCALER_DISABLE_CFG    0

#define TIMER0_RISING_EDGE_CFG          1
#define TIMER0_FALLING_EDGE_CFG         0

/* Section: Macro Functions Declarations */
#define TIMER0_MODULE_DISABLE()                (T0CONbits.TMR0ON = 0)
#define TIMER0_MODULE_ENABLE()                 (T0CONbits.TMR0ON = 1)

#define TIMER0_TIMER_MODE_ENABLE()             (T0CONbits.T0CS = 0)
#define TIMER0_COUNTER_MODE_ENABLE()           (T0CONbits.T0CS = 1)

#define TIMER0_8BIT_REGISTER()                 (T0CONbits.T08BIT = 1)
#define TIMER0_16BIT_REGISTER()                (T0CONbits.T08BIT = 0)

#define TIMER0_PRESCALER_DISABLE()             (T0CONbits.PSA = 1)
#define TIMER0_PRESCALER_ENABLE()              (T0CONbits.PSA = 0)

#define TIMER0_COUNTER_RISING_EDGE()           (T0CONbits.T0SE = 0)
#define TIMER0_COUNTER_FALLING_EDGE()          (T0CONbits.T0SE = 1)

#define TIMER0_PRESCALER_SELECT_BITS(value)    (T0CONbits.T0PS = value)

/* Section: Data type Declarations */
typedef enum {
    TIMER0_PRESCALER_DIV_BY_2 = 0,
    TIMER0_PRESCALER_DIV_BY_4,
    TIMER0_PRESCALER_DIV_BY_8,
    TIMER0_PRESCALER_DIV_BY_16,
    TIMER0_PRESCALER_DIV_BY_32,
    TIMER0_PRESCALER_DIV_BY_64,
    TIMER0_PRESCALER_DIV_BY_128,
    TIMER0_PRESCALER_DIV_BY_256
}timer0_prescaler_select_t;

typedef struct {
    #if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        InterruptHandler Timer0_InterruptHandler;
        interrupt_priority_t priority;
    #endif
    timer0_prescaler_select_t prescaler;
    uint16 timer0_preload_val;
    uint8 timer0_mode             : 1;  
    uint8 timer0_reg_size         : 1;  
    uint8 timer0_prescaler_enable : 1;
    uint8 timer0_counter_edge     : 1;
}timer0_config_t;

/* Section: Function Declarations */
Std_ReturnType mcal_Timer0_Init(const timer0_config_t* _timer0_config);
Std_ReturnType mcal_Timer0_DeInit(const timer0_config_t* _timer0_config);
Std_ReturnType mcal_Timer0_Write_Value(const timer0_config_t* _timer0_config, uint16 value);
Std_ReturnType mcal_Timer0_Read_Value(const timer0_config_t* _timer0_config, uint16* value);

#endif	/* MCAL_TIMER0_H */

