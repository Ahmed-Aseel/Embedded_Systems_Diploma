/* 
 * File:   mcal_timer3.h
 * Author: Ahmed Aseel
 *
 * Created on November 2, 2023, 9:58 PM
 */

#ifndef MCAL_TIMER3_H
#define	MCAL_TIMER3_H

/* Section: Includes */
#include "../pic18.h"
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/* Section: Macro Declarations */
#define TIMER3_TIMER_MODE               0
#define TIMER3_COUNTER_MODE             1

#define TIMER3_SYNC_COUNTER             0
#define TIMER3_ASYNC_COUNTER            1

#define TIMER3_8BIT_RW_MODE             0
#define TIMER3_16BIT_RW_MODE            1

/* Section: Macro Functions Declarations */
#define TIMER3_MODULE_DISABLE()                (T3CONbits.TMR3ON = 0)
#define TIMER3_MODULE_ENABLE()                 (T3CONbits.TMR3ON = 1)

#define TIMER3_TIMER_MODE_ENABLE()             (T3CONbits.TMR3CS = 0)
#define TIMER3_COUNTER_MODE_ENABLE()           (T3CONbits.TMR3CS = 1)

#define TIMER3_SYNC_COUNTER_ENABLE()           (T3CONbits.T3SYNC = 0)
#define TIMER3_ASYNC_COUNTER_ENABLE()          (T3CONbits.T3SYNC = 1)

#define TIMER3_8BIT_RW_MODE_ENABLE()           (T3CONbits.T3RD16 = 0)
#define TIMER3_16BIT_RW_MODE_ENABLE()          (T3CONbits.T3RD16 = 1)

#define TIMER3_PRESCALER_SELECT_BITS(value)    (T3CONbits.T3CKPS = value)

/* Section: Data type Declarations */
typedef enum {
    TIMER3_PRESCALER_DIV_BY_1 = 0,
    TIMER3_PRESCALER_DIV_BY_2,
    TIMER3_PRESCALER_DIV_BY_4,
    TIMER3_PRESCALER_DIV_BY_8,
}timer3_prescaler_select_t;

typedef struct {
    #if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        InterruptHandler Timer3_InterruptHandler;
        interrupt_priority_t priority;
    #endif
    timer3_prescaler_select_t prescaler;
    uint16 timer3_preload_val;
    uint8 timer3_mode             : 1;  
    uint8 timer3_counter_sync     : 1;
    uint8 timer3_reg_rw_mode      : 1;  
}timer3_config_t;

/* Section: Function Declarations */
Std_ReturnType mcal_Timer3_Init(const timer3_config_t* _timer3_config);
Std_ReturnType mcal_Timer3_DeInit(const timer3_config_t* _timer3_config);
Std_ReturnType mcal_Timer3_Write_Value(const timer3_config_t* _timer3_config, uint16 value);
Std_ReturnType mcal_Timer3_Read_Value(const timer3_config_t* _timer3_config, uint16* value);

#endif	/* MCAL_TIMER3_H */

