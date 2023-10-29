/* 
 * File:   mcal_internal_interrupt.h
 * Author: Ahmed Aseel
 *
 * Created on May 5, 2023, 4:23 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H

/* Section: Includes */
#include "mcal_interrupt_cfg.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for the ADC Module */
#define ADC_InterruptDisable()         (PIE1bits.ADIE = 0)
/* set interrupt enable for the ADC Module */
#define ADC_InterruptEnable()          (PIE1bits.ADIE = 1)
/* clear interrupt flag for the ADC Module */
#define ADC_InterruptFlagClear()       (PIR1bits.ADIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
/* set ADC Module Interrupt Priority level to be Low */
#define ADC_LowPrioritySet()           (IPR1bits.ADIP = 0)
/* set ADC Module Interrupt Priority level to be High */
#define ADC_HighPrioritySet()          (IPR1bits.ADIP = 1)
#endif
#endif

/* Section: Data type Declarations */

/* Section: Function Declarations */

#endif	/* MCAL_INTERNAL_INTERRUPT_H */

