/* 
 * File:   ecu_7_segment.h
 * Author: Ahmed Aseel
 *
 * Created on April 20, 2023, 8:03 PM
 */

#ifndef ECU_7_SEGMENT_H
#define	ECU_7_SEGMENT_H

/* Section: Includes */
#include "ecu_7_segment_cfg.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"

/* Section: Macro Declarations */
#define SEGMENT_PIN1_INDEX         0
#define SEGMENT_PIN2_INDEX         1
#define SEGMENT_PIN3_INDEX         2
#define SEGMENT_PIN4_INDEX         3

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */
typedef enum {
    SEGMENT_COMMON_ANODE = 0,
    SEGMENT_COMMON_CATHODE
}segment_type_t;

typedef struct {
    pin_config_t segment_pins[4];
    segment_type_t segment_type;
}segment_config_t;

/* Section: Function Declarations */
Std_ReturnType ecu_7_segment_initialize(const segment_config_t* _segment_config);
Std_ReturnType ecu_7_segment_write_number(const segment_config_t* _segment_config, uint8 number);

#endif	/* ECU_7_SEGMENT_H */

