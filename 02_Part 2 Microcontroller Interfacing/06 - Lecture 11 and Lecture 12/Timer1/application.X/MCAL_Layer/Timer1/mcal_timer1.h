/* 
 * File:   mcal_timer1.h
 * Author: Ahmed Aseel
 *
 * Created on October 31, 2023, 9:40 PM
 */

#ifndef MCAL_TIMER1_H
#define	MCAL_TIMER1_H

/* Section: Includes */
#include "../pic18.h"
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/* Section: Macro Declarations */
#define TIMER1_TIMER_MODE               0
#define TIMER1_COUNTER_MODE             1

#define TIMER1_SYNC_COUNTER             0
#define TIMER1_ASYNC_COUNTER            1

#define TIMER1_OSC_DISABLE_CFG          0
#define TIMER1_OSC_ENABLE_CFG           1

#define TIMER1_8BIT_ENABLE              0
#define TIMER1_16BIT_ENABLE             1

/* Section: Macro Functions Declarations */
#define TIMER1_MODULE_DISABLE()                (T1CONbits.TMR1ON = 0)
#define TIMER1_MODULE_ENABLE()                 (T1CONbits.TMR1ON = 1)

#define TIMER1_TIMER_MODE_ENABLE()             (T1CONbits.TMR1CS = 0)
#define TIMER1_COUNTER_MODE_ENABLE()           (T1CONbits.TMR1CS = 1)

#define TIMER1_SYNC_COUNTER_ENABLE()           (T1CONbits.T1SYNC = 0)
#define TIMER1_ASYNC_COUNTER_ENABLE()          (T1CONbits.T1SYNC = 1)

#define TIMER1_OSC_DISABLE()                   (T1CONbits.T1OSCEN = 0)
#define TIMER1_OSC_ENABLE()                    (T1CONbits.T1OSCEN = 1)

#define TIMER1_8BIT_REGISTER()                 (T1CONbits.T1RD16 = 0)
#define TIMER1_16BIT_REGISTER()                (T1CONbits.T1RD16 = 1)

#define TIMER1_PRESCALER_SELECT_BITS(value)    (T1CONbits.T1CKPS = value)
#define TIMER1_SYSTEM_CLK_STATUS               (T1CONbits.T1RUN)

/* Section: Data type Declarations */
typedef enum {
    TIMER1_PRESCALER_DIV_BY_1 = 0,
    TIMER1_PRESCALER_DIV_BY_2,
    TIMER1_PRESCALER_DIV_BY_4,
    TIMER1_PRESCALER_DIV_BY_8,
}timer1_prescaler_select_t;

typedef struct {
    #if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        InterruptHandler Timer1_InterruptHandler;
        interrupt_priority_t priority;
    #endif
    timer1_prescaler_select_t prescaler;
    uint16 timer1_preload_val;
    uint8 timer1_mode             : 1;  
    uint8 timer1_counter_sync     : 1;
    uint8 timer1_osc_cfg          : 1;  
    uint8 timer1_reg_size         : 1;  
}timer1_config_t;

/* Section: Function Declarations */
Std_ReturnType mcal_Timer1_Init(const timer1_config_t* _timer1_config);
Std_ReturnType mcal_Timer1_DeInit(const timer1_config_t* _timer1_config);
Std_ReturnType mcal_Timer1_Write_Value(const timer1_config_t* _timer1_config, uint16 value);
Std_ReturnType mcal_Timer1_Read_Value(const timer1_config_t* _timer1_config, uint16* value);

#endif	/* MCAL_TIMER1_H */

