/* 
 * File:   mcal_interrupt_cfg.h
 * Author: Ahmed Aseel
 *
 * Created on May 5, 2023, 4:22 PM
 */

#ifndef MCAL_INTERRUPT_CFG_H
#define	MCAL_INTERRUPT_CFG_H

/* Section: Includes */
#include "../pic18.h"
#include "../mcal_std_types.h"
#include "mcal_interrupt_gen_cfg.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"

/* Section: Macro Declarations */
#define INTERRUPT_DISABLE              0
#define INTERRUPT_ENABLE               1
#define INTERRUPT_NOT_OCCURRED         0
#define INTERRUPT_OCCURRED             1
#define INTERRUPT_PRIORITY_DISABLE     0
#define INTERRUPT_PRIORITY_ENABLE      1

/* Section: Macro Functions Declarations */

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/* disable priority levels on interrupts */
#define INTERRUPT_PriorityLevelsDisable()         (RCONbits.IPEN = 0)
/* enable priority levels on interrupts */
#define INTERRUPT_PriorityLevelsEnable()          (RCONbits.IPEN = 1)

/* disable high priority global interrupts */
#define INTERRUPT_GlobalInterruptHighDisable()    (INTCONbits.GIEH = 0)
/* enable high priority global interrupts */
#define INTERRUPT_GlobalInterruptHighEnable()     (INTCONbits.GIEH = 1)

/* disable low priority global interrupts */
#define INTERRUPT_GlobalInterruptLowDisable()     (INTCONbits.GIEL = 0)
/* enable low priority global interrupts */
#define INTERRUPT_GlobalInterruptLowEnable()      (INTCONbits.GIEL = 1)
#else
/* disable global interrupts */
#define INTERRUPT_GlobalInterruptDisable()        (INTCONbits.GIE = 0)
/* enable global interrupts */
#define INTERRUPT_GlobalInterruptEnable()         (INTCONbits.GIE = 1)

/* disable peripheral interrupts */
#define INTERRUPT_PeripheralInterruptDisable()    (INTCONbits.PEIE = 0)
/* enable peripheral interrupts */
#define INTERRUPT_PeripheralInterruptEnable()     (INTCONbits.PEIE = 1)
#endif

/* Section: Data type Declarations */
typedef enum {
    INTERRUPT_PRIORITY_LOW = 0,
    INTERRUPT_PRIORITY_HIGH        
}interrupt_priority_t;

/* Section: Function Declarations */


#endif	/* MCAL_INTERRUPT_CFG_H */

