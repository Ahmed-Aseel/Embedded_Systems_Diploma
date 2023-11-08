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

/************************** ADC ***************************/
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

/************************** TIMER0 ***************************/
#if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for the TIMER0 Module */
#define Timer0_InterruptDisable()      (INTCONbits.TMR0IE = 0)
/* set interrupt enable for the TIMER0 Module */
#define Timer0_InterruptEnable()       (INTCONbits.TMR0IE = 1)
/* clear interrupt flag for the TIMER0 Module */
#define Timer0_InterruptFlagClear()    (INTCONbits.TMR0IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
/* set TIMER0 Module Interrupt Priority level to be Low */
#define Timer0_LowPrioritySet()        (INTCON2bits.TMR0IP = 0)
/* set TIMER0 Module Interrupt Priority level to be High */
#define Timer0_HighPrioritySet()       (INTCON2bits.TMR0IP = 1)
#endif
#endif

/************************** TIMER1 ***************************/
#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for the TIMER1 Module */
#define Timer1_InterruptDisable()      (PIE1bits.TMR1IE = 0)
/* set interrupt enable for the TIMER1 Module */
#define Timer1_InterruptEnable()       (PIE1bits.TMR1IE = 1)
/* clear interrupt flag for the TIMER1 Module */
#define Timer1_InterruptFlagClear()    (PIR1bits.TMR1IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
/* set TIMER1 Module Interrupt Priority level to be Low */
#define Timer1_LowPrioritySet()        (IPR1bits.TMR1IP = 0)
/* set TIMER1 Module Interrupt Priority level to be High */
#define Timer1_HighPrioritySet()       (IPR1bits.TMR1IP = 1)
#endif
#endif

/************************** TIMER2 ***************************/
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for the TIMER2 Module */
#define Timer2_InterruptDisable()      (PIE1bits.TMR2IE = 0)
/* set interrupt enable for the TIMER2 Module */
#define Timer2_InterruptEnable()       (PIE1bits.TMR2IE = 1)
/* clear interrupt flag for the TIMER2 Module */
#define Timer2_InterruptFlagClear()    (PIR1bits.TMR2IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
/* set TIMER2 Module Interrupt Priority level to be Low */
#define Timer2_LowPrioritySet()        (IPR1bits.TMR2IP = 0)
/* set TIMER2 Module Interrupt Priority level to be High */
#define Timer2_HighPrioritySet()       (IPR1bits.TMR2IP = 1)
#endif
#endif

/************************** TIMER3 ***************************/
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for the TIMER3 Module */
#define Timer3_InterruptDisable()      (PIE2bits.TMR3IE = 0)
/* set interrupt enable for the TIMER3 Module */
#define Timer3_InterruptEnable()       (PIE2bits.TMR3IE = 1)
/* clear interrupt flag for the TIMER3 Module */
#define Timer3_InterruptFlagClear()    (PIR2bits.TMR3IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
/* set TIMER3 Module Interrupt Priority level to be Low */
#define Timer3_LowPrioritySet()        (IPR2bits.TMR3IP = 0)
/* set TIMER3 Module Interrupt Priority level to be High */
#define Timer3_HighPrioritySet()       (IPR2bits.TMR3IP = 1)
#endif
#endif

/************************** CCP1 ***************************/
#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for the CCP1 Module */
#define CCP1_InterruptDisable()      (PIE1bits.CCP1IE = 0)
/* set interrupt enable for the CCP1 Module */
#define CCP1_InterruptEnable()       (PIE1bits.CCP1IE = 1)
/* clear interrupt flag for the CCP1 Module */
#define CCP1_InterruptFlagClear()    (PIR1bits.CCP1IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
/* set CCP1 Module Interrupt Priority level to be Low */
#define CCP1_LowPrioritySet()        (IPR1bits.CCP1IP = 0)
/* set CCP1 Module Interrupt Priority level to be High */
#define CCP1_HighPrioritySet()       (IPR1bits.CCP1IP = 1)
#endif
#endif

/************************** CCP2 ***************************/
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/* clear interrupt enable for the CCP2 Module */
#define CCP2_InterruptDisable()      (PIE2bits.CCP2IE = 0)
/* set interrupt enable for the CCP2 Module */
#define CCP2_InterruptEnable()       (PIE2bits.CCP2IE = 1)
/* clear interrupt flag for the CCP2 Module */
#define CCP2_InterruptFlagClear()    (PIR2bits.CCP2IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE 
/* set CCP2 Module Interrupt Priority level to be Low */
#define CCP2_LowPrioritySet()        (IPR2bits.CCP2IP = 0)
/* set CCP2 Module Interrupt Priority level to be High */
#define CCP2_HighPrioritySet()       (IPR2bits.CCP2IP = 1)
#endif
#endif

/* Section: Data type Declarations */

/* Section: Function Declarations */

#endif	/* MCAL_INTERNAL_INTERRUPT_H */

