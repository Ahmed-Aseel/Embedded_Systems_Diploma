/* 
 * File:   ecu_button.h
 * Author: Ahmed Aseel
 *
 * Created on April 18, 2023, 2:20 PM
 */

#ifndef ECU_BUTTON_H
#define	ECU_BUTTON_H

/* Section: Includes */
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"
#include "ecu_button_cfg.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */
typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
}button_state_t;

typedef enum {
    BUTTON_ACTIVE_LOW = 0,
    BUTTON_ACTIVE_HIGH
}button_active_t;

typedef struct {
    pin_config_t button_pin;
    button_state_t button_state;
    button_active_t button_connection;
}button_config_t;

/* Section: Function Declarations */
Std_ReturnType ecu_button_initialize(const button_config_t* _btn_config);
Std_ReturnType ecu_button_read_state(const button_config_t* _btn_config, button_state_t* btn_state);

#endif	/* ECU_BUTTON_H */

