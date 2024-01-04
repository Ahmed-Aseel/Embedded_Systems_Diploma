/**
  ******************************************************************************
  * @file    bootloader.h
  * @author  Ahmed Aseel
  * @brief   Header file of bootloader.
  ******************************************************************************
  */

#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_

/* -------------------- Includes -------------------- */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "usart.h"
#include "crc.h"

/* -------------------- Macro Declarations -------------------- */
/* Bootloader version */
#define CBL_VENDOR_ID                 100
#define CBL_SW_MAJOR_VERSION          1
#define CBL_SW_MINOR_VERSION          0
#define CBL_SW_PATCH_VERSION          0

#define BL_DEBUG_UART	 			  &huart2
#define BL_HOST_COMMUNICATION_UART    &huart3
#define CRC_ENGINE_OBJ				  &hcrc

#define DEBUG_INFO_DISABLE	     	  0
#define DEBUG_INFO_ENABLE	 	      1
#define BL_DEBUG_INFO_CFG	 		  (DEBUG_INFO_ENABLE)

#define BL_ENABLE_UART_DEBUG_MESSAGE  0x00
#define BL_ENABLE_SPI_DEBUG_MESSAGE   0x01
#define BL_ENABLE_CAN_DEBUG_MESSAGE   0x02
#define BL_DEBUG_METHOD	 			  (BL_ENABLE_UART_DEBUG_MESSAGE)

/* Max packet length sent by the host */
#define BL_HOST_RX_BUFFER_LENGTH      200

#define CBL_GET_VER_CMD               0x10
#define CBL_GET_HELP_CMD              0x11
#define CBL_GET_CID_CMD               0x12
/* Get Read Protection Status */
#define CBL_GET_RDP_STATUS_CMD        0x13
#define CBL_GO_TO_ADDR_CMD            0x14
#define CBL_FLASH_ERASE_CMD           0x15
#define CBL_MEM_WRITE_CMD             0x16
/* Enable/Disable Write Protection */
#define CBL_ED_W_PROTECT_CMD          0x17
#define CBL_MEM_READ_CMD              0x18
/* Get Sector Read/Write Protection Status */
#define CBL_READ_SECTOR_STATUS_CMD    0x19
#define CBL_OTP_READ_CMD              0x20
/* Change Read Protection Level */
#define CBL_CHANGE_RDP_Level_CMD      0x21

#define CBL_SEND_NACK				  0xAB
#define CBL_SEND_ACK				  0xCD

/* CRC Verification */
#define CRC_TYPE_SIZE_BYTE            4
#define CRC_VERIFICATION_FAILED       0x00
#define CRC_VERIFICATION_PASSED 	  0x01

/* Address Verification */
#define ADDRESS_IS_INVALID            0x00
#define ADDRESS_IS_VALID 			  0x01

#define STM32F407_SRAM1_SIZE		  (16 * 1024)
#define STM32F407_SRAM2_SIZE		  (112 * 1024)
#define STM32F407_SRAM3_SIZE		  (64 * 1024) /* CCMDATARAM */
#define STM32F407_FLASH_SIZE		  (1024 * 1024)

#define STM32F407_SRAM1_END 		  (SRAM1_BASE + STM32F407_SRAM1_SIZE)
#define STM32F407_SRAM2_END 		  (SRAM2_BASE + STM32F407_SRAM2_SIZE)
#define STM32F407_SRAM3_END 		  (CCMDATARAM_BASE + STM32F407_SRAM3_SIZE)
#define STM32F407_FLASH_END 		  (FLASH_BASE + STM32F407_FLASH_SIZE)

/* CBL_FLASH_ERASE_CMD */
#define FLASH_MAX_SECTOR_NUMBER   	  11
#define FLASH_MAX_NUMBER_OF_SECTORS   12
#define FLASH_MASS_ERASE			  0xFF
#define HAL_SUCCESSFUL_ERASE		  0xFFFFFFFFU
/* Flash erase return status */
#define UNSUCCESSFUL_ERASE			  0x00
#define SUCCESSFUL_ERASE			  0x01
#define INVALID_SECTOR_NUMBER         0x02
#define INVALID_NUMBER_OF_SECTORS	  0x03

/* CBL_MEM_WRITE_CMD */
#define FLASH_PAYLOAD_WRITE_FAILED    0x00
#define FLASH_PAYLOAD_WRITE_PASSED    0x01

/* CBL_CHANGE_RDP_Level_CMD */
#define RDP_LEVEL_CHANGE_INVALID      0x00
#define RDP_LEVEL_CHANGE_VALID        0X01
#define CBL_RDP_LEVEL_0               0xAA
#define CBL_RDP_LEVEL_1               0x55
#define CBL_RDP_LEVEL_2               0xCC /* Warning: When enabling it's no more possible to go back to level 1 or 0 */

/* -------------------- Macro Functions Declarations -------------------- */

/* -------------------- Data type Declarations -------------------- */
typedef enum {
	BL_NACK = 0,
	BL_OK
}BL_Status_t;

typedef void (*Jump_Ptr)(void);

/* -------------------- Software Interfaces Declarations -------------------- */
void BL_Print_Message(char* format, ...);
BL_Status_t BL_UART_Fetch_Host_Command(void);

#endif /* INC_BOOTLOADER_H_ */
