/* 
 * File:   mcal_timer2.h
 * Author: Ahmed Aseel
 *
 * Created on November 1, 2023, 11:41 PM
 */

#ifndef MCAL_TIMER2_H
#define	MCAL_TIMER2_H

/* Section: Includes */
#include "../pic18.h"
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */
#define TIMER2_MODULE_DISABLE()                       (T2CONbits.TMR2ON = 0)
#define TIMER2_MODULE_ENABLE()                        (T2CONbits.TMR2ON = 1)

#define TIMER2_PRESCALER_SELECT_BITS(_PRESCALER_)     (T2CONbits.T2CKPS = _PRESCALER_)
#define TIMER2_POSTSCALER_SELECT_BITS(_POSTSCALER_)   (T2CONbits.TOUTPS = _POSTSCALER_)

/* Section: Data type Declarations */
typedef enum {
    TIMER2_PRESCALER_DIV_BY_1 = 0,
    TIMER2_PRESCALER_DIV_BY_4,
    TIMER2_PRESCALER_DIV_BY_16,
}timer2_prescaler_select_t;

typedef enum {
    TIMER2_POSTSCALER_DIV_BY_1 = 0,
    TIMER2_POSTSCALER_DIV_BY_2,
    TIMER2_POSTSCALER_DIV_BY_3,
    TIMER2_POSTSCALER_DIV_BY_4,
    TIMER2_POSTSCALER_DIV_BY_5,
    TIMER2_POSTSCALER_DIV_BY_6,
    TIMER2_POSTSCALER_DIV_BY_7,
    TIMER2_POSTSCALER_DIV_BY_8,
    TIMER2_POSTSCALER_DIV_BY_9,
    TIMER2_POSTSCALER_DIV_BY_10,
    TIMER2_POSTSCALER_DIV_BY_11,
    TIMER2_POSTSCALER_DIV_BY_12,
    TIMER2_POSTSCALER_DIV_BY_13,
    TIMER2_POSTSCALER_DIV_BY_14,
    TIMER2_POSTSCALER_DIV_BY_15,
    TIMER2_POSTSCALER_DIV_BY_16,
}timer2_postscaler_select_t;

typedef struct {
    #if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        InterruptHandler Timer2_InterruptHandler;
        interrupt_priority_t priority;
    #endif
    timer2_prescaler_select_t prescaler;
    timer2_postscaler_select_t postscaler;
    uint8 timer2_preload_val; 
}timer2_config_t;

/* Section: Function Declarations */
Std_ReturnType mcal_Timer2_Init(const timer2_config_t* _timer2_config);
Std_ReturnType mcal_Timer2_DeInit(const timer2_config_t* _timer2_config);
Std_ReturnType mcal_Timer2_Write_Value(const timer2_config_t* _timer2_config, uint8 value);
Std_ReturnType mcal_Timer2_Read_Value(const timer2_config_t* _timer2_config, uint8* value);

#endif	/* MCAL_TIMER2_H */

