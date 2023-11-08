/* 
 * File:   mcal_external_interrupt.h
 * Author: Ahmed Aseel
 *
 * Created on May 5, 2023, 4:24 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H

/* Section: Includes */
#include "mcal_interrupt_cfg.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/************************** EXT Interrupt ***************************/
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for external interrupt, INT0 */
#define EXT_INT0_InterruptDisable()       (INTCONbits.INT0IE = 0)
/* set interrupt enable for external interrupt, INT0 */
#define EXT_INT0_InterruptEnable()        (INTCONbits.INT0IE = 1)
/* clear interrupt flag for external interrupt, INT0 */
#define EXT_INT0_InterruptFlagClear()     (INTCONbits.INT0IF = 0)
/* set the edge detect of external INT0 to negative edge */
#define EXT_INT0_FallingEdgeSet()         (INTCON2bits.INTEDG0 = 0)
/* set the edge detect of external INT0 to positive edge */
#define EXT_INT0_RisingEdgeSet()          (INTCON2bits.INTEDG0 = 1)

/* clear interrupt enable for external interrupt, INT1 */
#define EXT_INT1_InterruptDisable()       (INTCON3bits.INT1IE = 0)
/* set interrupt enable for external interrupt, INT1 */
#define EXT_INT1_InterruptEnable()        (INTCON3bits.INT1IE = 1)
/* clear interrupt flag for external interrupt, INT1 */
#define EXT_INT1_InterruptFlagClear()     (INTCON3bits.INT1IF = 0)
/* set the edge detect of external INT1 to negative edge */
#define EXT_INT1_FallingEdgeSet()         (INTCON2bits.INTEDG1 = 0)
/* set the edge detect of external INT1 to positive edge */
#define EXT_INT1_RisingEdgeSet()          (INTCON2bits.INTEDG1 = 1)

/* clear interrupt enable for external interrupt, INT2 */
#define EXT_INT2_InterruptDisable()       (INTCON3bits.INT2IE = 0)
/* set interrupt enable for external interrupt, INT2 */
#define EXT_INT2_InterruptEnable()        (INTCON3bits.INT2IE = 1)
/* clear interrupt flag for external interrupt, INT2 */
#define EXT_INT2_InterruptFlagClear()     (INTCON3bits.INT2IF = 0)
/* set the edge detect of external INT2 to negative edge */
#define EXT_INT2_FallingEdgeSet()         (INTCON2bits.INTEDG2 = 0)
/* set the edge detect of external INT2 to positive edge */
#define EXT_INT2_RisingEdgeSet()          (INTCON2bits.INTEDG2 = 1)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/* set external INT1 priority level to be low */
#define EXT_INT1_LowPrioritySet()         (INTCON3bits.INT1IP = 0)
/* set external INT1 priority level to be high */
#define EXT_INT1_HighPrioritySet()        (INTCON3bits.INT1IP = 1)

/* set external INT2 priority level to be low */
#define EXT_INT2_LowPrioritySet()         (INTCON3bits.INT2IP = 0)
/* set external INT2 priority level to be high */
#define EXT_INT2_HighPrioritySet()        (INTCON3bits.INT2IP = 1)
#endif
#endif

/************************* ON Change Interrupt **************************/
#if EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for external interrupt, RBx */
#define EXT_RBx_InterruptDisable()        (INTCONbits.RBIE = 0)
/* set interrupt enable for external interrupt, RBx */
#define EXT_RBx_InterruptEnable()         (INTCONbits.RBIE = 1)
/* clear interrupt flag for external interrupt, RBx */
#define EXT_RBx_InterruptFlagClear()      (INTCONbits.RBIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/* set external RBx priority level to be low */
#define EXT_RBx_LowPrioritySet()          (INTCON2bits.RBIP = 0)
/* set external RBx priority level to be high */
#define EXT_RBx_HighPrioritySet()         (INTCON2bits.RBIP = 1)
#endif
#endif

/* Section: Data type Declarations */

typedef enum {
    INTERRUPT_FALLING_EDGE = 0,
    INTERRUPT_RISING_EDGE        
}INTx_edge_t;

typedef enum {
    INTERRUPT_EXTERNAL_INT0 = 0,
    INTERRUPT_EXTERNAL_INT1,
    INTERRUPT_EXTERNAL_INT2
}INTx_src_t;

typedef struct {
    pin_config_t INTx_pin;
    INTx_edge_t edge;
    INTx_src_t  source;
    interrupt_priority_t priority;
    InterruptHandler EXT_InterruptHandler; // EXT_InterruptHandler is a pointer to fun take void and ret void     
}INTx_config_t;

typedef struct {
    pin_config_t RBx_pin;
    interrupt_priority_t priority;
    InterruptHandler EXT_InterruptHandler_Low;  // EXT_InterruptHandler_Low is a pointer to fun take void and ret void      
    InterruptHandler EXT_InterruptHandler_High; // EXT_InterruptHandler_High is a pointer to fun take void and ret void      
}RBx_intr_config_t;

/* Section: Function Declarations */
Std_ReturnType mcal_interrupt_INTx_Init(const INTx_config_t* _INTx_config);
Std_ReturnType mcal_interrupt_INTx_DeInit(const INTx_config_t* _INTx_config);

Std_ReturnType mcal_interrupt_RBx_Init(const RBx_intr_config_t* _RBx_intr_config);
Std_ReturnType mcal_interrupt_RBx_DeInit(const RBx_intr_config_t* _RBx_intr_config);

#endif	/* MCAL_EXTERNAL_INTERRUPT_H */
