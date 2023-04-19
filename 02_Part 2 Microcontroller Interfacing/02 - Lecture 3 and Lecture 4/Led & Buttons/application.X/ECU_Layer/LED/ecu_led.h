/* 
 * File:   ecu_led.h
 * Author: Ahmed Aseel
 *
 * Created on April 15, 2023, 7:16 PM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H

/* Section: Includes */
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"
#include "ecu_led_cfg.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */
typedef enum {
    LED_OFF = 0,
    LED_ON
}led_state_t;

typedef struct {
    pin_config_t led_pin;
//    uint8 port      : 3;
//    uint8 pin       : 3;
//    uint8 led_status: 1;
//    uint8 reserved  : 1;
}led_config_t;

/* Section: Function Declarations */
Std_ReturnType ecu_led_initialize(const led_config_t* _led_config);
Std_ReturnType ecu_led_turn_off(const led_config_t* _led_config);
Std_ReturnType ecu_led_turn_on(const led_config_t* _led_config);
Std_ReturnType ecu_led_toggle(const led_config_t* _led_config);

#endif	/* ECU_LED_H */

