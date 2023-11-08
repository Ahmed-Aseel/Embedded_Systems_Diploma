/* 
 * File:   application.h
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 7:07 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section: Includes */
#include "ECU_Layer/ecu_layer_init.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/ADC/mcal_adc.h"
#include "MCAL_Layer/Timer0/mcal_timer0.h"
#include "MCAL_Layer/Timer1/mcal_timer1.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */

/* Section: Function Declarations */
void application_initialize(void);

#endif	/* APPLICATION_H */

