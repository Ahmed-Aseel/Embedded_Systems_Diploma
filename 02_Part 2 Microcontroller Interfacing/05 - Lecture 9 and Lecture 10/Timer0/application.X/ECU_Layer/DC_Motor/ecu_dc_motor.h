/* 
 * File:   ecu_dc_motor.h
 * Author: Ahmed Aseel
 *
 * Created on April 20, 2023, 3:17 AM
 */

#ifndef ECU_DC_MOTOR_H
#define	ECU_DC_MOTOR_H

/* Section: Includes */
#include "ecu_dc_motor_cfg.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"

/* Section: Macro Declarations */
#define DC_MOTOR_PIN1_INDEX         0
#define DC_MOTOR_PIN2_INDEX         1

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */
typedef enum {
    DC_MOTOR_OFF = 0,
    DC_MOTOR_ON
}dc_motor_state_t;

typedef struct {
    pin_config_t dc_motor_pins[2];
}dc_motor_config_t;

/* Section: Function Declarations */
Std_ReturnType ecu_dc_motor_initialize(const dc_motor_config_t* _dc_motor_config);
Std_ReturnType ecu_dc_motor_move_right(const dc_motor_config_t* _dc_motor_config);
Std_ReturnType ecu_dc_motor_move_left(const dc_motor_config_t* _dc_motor_config);
Std_ReturnType ecu_dc_motor_stop(const dc_motor_config_t* _dc_motor_config);

#endif	/* ECU_DC_MOTOR_H */

