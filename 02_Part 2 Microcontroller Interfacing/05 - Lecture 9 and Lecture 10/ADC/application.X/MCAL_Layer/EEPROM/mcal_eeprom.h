/* 
 * File:   mcal_eeprom.h
 * Author: Ahmed Aseel
 *
 * Created on May 8, 2023, 10:42 AM
 */

#ifndef MCAL_EEPROM_H
#define	MCAL_EEPROM_H

/* Section: Includes */
#include "mcal_eeprom_cfg.h"

/* Section: Macro Declarations */

/* Flash Program or Data EEPROM Memory Select  */
#define ACCESS_FLASH_PROGRAM_MEMORY       1
#define ACCESS_DATA_EEPROM_MEMORY         0
/* Flash Program/Data EEPROM or Configuration Select */
#define ACCESS_CONFIG_REGISTERS           1
#define ACCESS_FLASH_EEPROM_MEMORY        0
/* Flash Program/Data EEPROM Write Enable */
#define ALLOW_WRITE_CYCLES_FLASH_EEPROM   1
#define INHIBT_WRITE_CYCLES_FLASH_EEPROM  0
/* Write Control */
#define INITIATE_DATA_EEPROM_WRITE_ERASE  1
#define DATA_EEPROM_WRITE_ERASE_COMPLETED 0
/* Read Control */
#define INITIATE_DATA_EEPROM_READ         1

/* Section: Macro Functions Declarations */

/* Section: Data type Declarations */

/* Section: Function Declarations */
Std_ReturnType mcal_EEPROM_WriteByte(uint16 address, uint8 data);
Std_ReturnType mcal_EEPROM_ReadByte(uint16 address, uint8* data);

#endif	/* MCAL_EEPROM_H */

