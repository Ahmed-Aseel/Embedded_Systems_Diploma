/* 
 * File:   ecu_keypad.h
 * Author: Ahmed Aseel
 *
 * Created on April 22, 2023, 1:29 AM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

/* Section: Includes */
#include "ecu_keypad_cfg.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"

/* Section: Macro Declarations */
#define KEYPAD_ROWS_NUMBER       4
#define KEYPAD_COLUMNS_NUMBER    4

#define SEGMENT_PIN1_INDEX         0
#define SEGMENT_PIN2_INDEX         1
#define SEGMENT_PIN3_INDEX         2
#define SEGMENT_PIN4_INDEX         3

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */
typedef struct {
    pin_config_t keypad_rows_pins[KEYPAD_ROWS_NUMBER];
    pin_config_t keypad_columns_pins[KEYPAD_COLUMNS_NUMBER];
}keypad_config_t;

/* Section: Function Declarations */
Std_ReturnType ecu_keypad_initialize(const keypad_config_t* _keypad_config);
Std_ReturnType ecu_keypad_get_value(const keypad_config_t* _keypad_config, uint8* value);

#endif	/* ECU_KEYPAD_H */

