/* 
 * File:   mcal_eeprom.c
 * Author: Ahmed Aseel
 *
 * Created on May 8, 2023, 10:42 AM
 */

#include "mcal_eeprom.h"

Std_ReturnType mcal_EEPROM_WriteByte(uint16 address, uint8 data)
{
    Std_ReturnType ret = E_OK;
    uint8 Global_Interrupt_Status;
    
    /* Read the Interrupt Status "Enabled or Disabled" */
    Global_Interrupt_Status = INTCONbits.GIE;
    /* Update the Address Registers */
    EEADR = (uint8)(address & 0xFF);
    EEADRH = (uint8)((address >> 8) & 0x03);
    /* Update the Data Register */
    EEDATA = data;
    /* Select Access data EEPROM memory */
    EECON1bits.EEPGD = ACCESS_DATA_EEPROM_MEMORY;
    /* Select Access Flash program or data EEPROM memory */
    EECON1bits.CFGS = ACCESS_FLASH_EEPROM_MEMORY;    
    /* Allows write cycles to Flash program/data EEPROM */
    EECON1bits.WREN = ALLOW_WRITE_CYCLES_FLASH_EEPROM;
    /* Disable all interrupts "General Interrupt" */
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GlobalInterruptHighDisable();
    #else
        INTERRUPT_GlobalInterruptDisable();
    #endif
    /* Write the required sequence : 0x55 -> 0xAA */
    EECON2 = 0x55;
    EECON2 = 0xAA;    
    /* Initiates a data EEPROM erase/write cycle */
    EECON1bits.WR = INITIATE_DATA_EEPROM_WRITE_ERASE;
    /* Wait for write to complete */
    while (EECON1bits.WR);
    /* Inhibits write cycles to Flash program/data EEPROM */
    EECON1bits.WREN = INHIBT_WRITE_CYCLES_FLASH_EEPROM;    
    /* Restore the Interrupt Status "Enabled or Disabled" */
    INTCONbits.GIE = Global_Interrupt_Status;
    
    return ret;
}

Std_ReturnType mcal_EEPROM_ReadByte(uint16 address, uint8* data)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == data) 
    {
        ret = E_NOT_OK;
    } 
    else
    {
        /* Update the Address Registers */
        EEADR = (uint8)(address & 0xFF);
        EEADRH = (uint8)((address >> 8) & 0x03);        
        /* Select Access data EEPROM memory */
        EECON1bits.EEPGD = ACCESS_DATA_EEPROM_MEMORY;
         /* Select Access Flash program or data EEPROM memory */
        EECON1bits.CFGS = ACCESS_FLASH_EEPROM_MEMORY;    
        /* Initiates a data EEPROM read cycle */
        EECON1bits.RD = INITIATE_DATA_EEPROM_READ;
        NOP(); /* NOPs may be required for latency at high frequencies */
        NOP(); /* NOPs may be required for latency at high frequencies */
        /* Return data */
        *data = EEDATA;
    }
    
    return ret;    
}