/* 
 * File:   mcal_ccp_cfg.h
 * Author: Ahmed Aseel
 *
 * Created on November 5, 2023, 6:18 PM
 */

#ifndef MCAL_CCP_CFG_H
#define	MCAL_CCP_CFG_H

/* Section: Includes */
#include "../pic18.h"
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/mcal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/* Section: Macro Declarations */
#define CCP_CFG_CAPTURE_MODE_SELECTED    0x00
#define CCP_CFG_COMPARE_MODE_SELECTED    0x01
#define CCP_CFG_PWM_MODE_SELECTED        0x02

#define CCP1_CFG_MODE_SELECTED           CCP_CFG_COMPARE_MODE_SELECTED
#define CCP2_CFG_MODE_SELECTED           CCP_CFG_COMPARE_MODE_SELECTED

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */

/* Section: Function Declarations */

#endif	/* MCAL_CCP_CFG_H */

