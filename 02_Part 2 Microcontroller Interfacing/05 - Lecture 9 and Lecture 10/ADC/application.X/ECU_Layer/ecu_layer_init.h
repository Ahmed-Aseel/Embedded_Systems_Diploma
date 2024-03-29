/* 
 * File:   ecu_layer_init.h
 * Author: Ahmed Aseel
 *
 * Created on April 22, 2023, 1:53 AM
 */

#ifndef ECU_LAYER_INIT_H
#define	ECU_LAYER_INIT_H

/* Section: Includes */
#include "LED/ecu_led.h"
#include "Button/ecu_button.h"
#include "DC_Motor/ecu_dc_motor.h"
#include "7_Segment/ecu_7_segment.h"
#include "KeyPad/ecu_keypad.h"
#include "LCD/ecu_lcd.h"

/* Section: Data type Declarations */
extern lcd_4bits_config_t lcd_1;
extern dc_motor_config_t dc_motor_1;
extern dc_motor_config_t dc_motor_2;

/* Section: Function Declarations */
void ecu_layer_initialize(void);

#endif	/* ECU_LAYER_INIT_H */

