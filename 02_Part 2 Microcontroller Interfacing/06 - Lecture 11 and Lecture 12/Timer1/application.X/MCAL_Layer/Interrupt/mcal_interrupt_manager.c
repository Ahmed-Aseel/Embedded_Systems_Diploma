/* 
 * File:   mcal_interrupt_manager.c
 * Author: Ahmed Aseel
 *
 * Created on May 5, 2023, 4:23 PM
 */

#include "mcal_interrupt_manager.h"

static volatile uint8 RB4_flag = 1, RB5_flag = 1, RB6_flag = 1, RB7_flag = 1;

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
void __interrupt() InterruptManagerHigh(void)
{
    if(INTERRUPT_ENABLE == INTCONbits.INT0IE && INTERRUPT_OCCURRED == INTCONbits.INT0IF)
    {
        INT0_ISR();
    }
    else {/* nothing */}
    
    if(INTERRUPT_ENABLE == INTCON3bits.INT2IE && INTERRUPT_OCCURRED == INTCON3bits.INT2IF)
    {
        INT2_ISR();
    }
    else {/* nothing */} 

    if((INTERRUPT_ENABLE == INTCONbits.TMR0IE) && (INTERRUPT_OCCURRED == INTCONbits.TMR0IF))
    {
        TIMER0_ISR();
    }
    else {/* nothing */}    

    if((INTERRUPT_ENABLE == PIE1bits.TMR1IE) && (INTERRUPT_OCCURRED == PIR1bits.TMR1IF))
    {
        TIMER1_ISR();
    }
    else {/* nothing */}    
}

void __interrupt(low_priority) InterruptManagerLow(void)
{
    if(INTERRUPT_ENABLE == INTCON3bits.INT1IE && INTERRUPT_OCCURRED == INTCON3bits.INT1IF)
    {
        INT1_ISR();
    }
    else {/* nothing */}
}
#else
void __interrupt() InterruptManager(void)
{
    if(INTERRUPT_ENABLE == INTCONbits.INT0IE && INTERRUPT_OCCURRED == INTCONbits.INT0IF)
    {
        INT0_ISR();
    }
    else {/* nothing */}
    
    if(INTERRUPT_ENABLE == INTCON3bits.INT1IE && INTERRUPT_OCCURRED == INTCON3bits.INT1IF)
    {
        INT1_ISR();
    }
    else {/* nothing */}
    
    if(INTERRUPT_ENABLE == INTCON3bits.INT2IE && INTERRUPT_OCCURRED == INTCON3bits.INT2IF)
    {
        INT2_ISR();
    }
    else {/* nothing */}    
    
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
       && PORTBbits.RB4 == GPIO_LOW && 1 == RB4_flag)
    {
        RB4_flag = 0;
        RB4_ISR(0);
    }
    else if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
            && PORTBbits.RB4 == GPIO_HIGH && 0 == RB4_flag)
    {
        RB4_flag = 1;
        RB4_ISR(1);        
    } 
    else {/* nothing */} 
    
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
       && PORTBbits.RB5 == GPIO_LOW && 1 == RB5_flag)
    {
        RB5_flag = 0;
        RB5_ISR(0);
    }
    else if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
            && PORTBbits.RB5 == GPIO_HIGH && 0 == RB5_flag)
    {
        RB5_flag = 1;
        RB5_ISR(1);        
    } 
    else {/* nothing */} 
    
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
       && PORTBbits.RB6 == GPIO_LOW && 1 == RB6_flag)
    {
        RB6_flag = 0;
        RB6_ISR(0);
    }
    else if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
            && PORTBbits.RB6 == GPIO_HIGH && 0 == RB6_flag)
    {
        RB6_flag = 1;
        RB6_ISR(1);        
    } 
    else {/* nothing */} 
    
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
       && PORTBbits.RB7 == GPIO_LOW && 1 == RB7_flag)
    {
        RB7_flag = 0;
        RB7_ISR(0);
    }
    else if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
            && PORTBbits.RB7 == GPIO_HIGH && 0 == RB7_flag)
    {
        RB7_flag = 1;
        RB7_ISR(1);        
    } 
    else {/* nothing */} 

    if((INTERRUPT_ENABLE == PIE1bits.ADIE) && (INTERRUPT_OCCURRED == PIR1bits.ADIF))
    {
        ADC_ISR();
    }
    else {/* nothing */}
}
#endif