/**
  ******************************************************************************
  * @file    bootloader.c
  * @author  Ahmed Aseel
  * @brief   Source file of bootloader.
  ******************************************************************************
  */

/* -------------------- Includes -------------------- */
#include "bootloader.h"

/* -------------------- Static Functions Declarations -------------------- */
static void BootLoader_Get_Version(uint8_t *Host_Buffer);
static void BootLoader_Get_Help(uint8_t *Host_Buffer);
static void BootLoader_Get_Chip_Identification_Number(uint8_t *Host_Buffer);
static void BootLoader_Read_Protection_Level(uint8_t *Host_Buffer);
static void BootLoader_Jump_to_Address(uint8_t *Host_Buffer);
static void BootLoader_Erase_Flash(uint8_t *Host_Buffer);
static void BootLoader_Memory_Write(uint8_t *Host_Buffer);
static void BootLoader_Enable_RW_Protection(uint8_t *Host_Buffer);
static void BootLoader_Memory_Read(uint8_t *Host_Buffer);
static void BootLoader_Read_Sector_Protection_Status(uint8_t *Host_Buffer);
static void BootLoader_Read_OTP(uint8_t *Host_Buffer);
static void BootLoader_Change_Read_Protection_Level(uint8_t *Host_Buffer);

static uint8_t BootLoader_CRC_Verify(uint8_t *pData, uint8_t Data_Len, uint32_t Host_CRC);
static void BootLoader_Send_ACK(uint8_t Reply_Len);
static void BootLoader_Send_NACK(void);
static void BootLoader_Send_Data_to_Host(uint8_t *Host_Buffer, uint8_t Data_Len);
static uint8_t BootLoader_Get_RDP_Level(void);
static uint8_t Host_Address_Verification(uint32_t Jump_Address);
static uint8_t Perform_Flash_Erase(uint8_t Sector_Number, uint8_t Number_Of_Sectors);
static uint8_t Flash_Memory_Write_Payload(uint8_t *Host_Payload, uint32_t Payload_Start_Address, uint8_t Payload_Len);
static uint8_t Change_RDP_Level(uint8_t RDP_Level);

/* -------------------- Global Variables Definitions -------------------- */
/* Host Input Command */
static uint8_t BL_Host_Buffer[BL_HOST_RX_BUFFER_LENGTH];

static uint8_t BL_Supported_CMDs[12] =
{
    CBL_GET_VER_CMD,
	CBL_GET_HELP_CMD,
	CBL_GET_CID_CMD,
	CBL_GET_RDP_STATUS_CMD,
	CBL_GO_TO_ADDR_CMD,
	CBL_FLASH_ERASE_CMD,
	CBL_MEM_WRITE_CMD,
	CBL_ED_W_PROTECT_CMD,
	CBL_MEM_READ_CMD,
	CBL_READ_SECTOR_STATUS_CMD,
	CBL_OTP_READ_CMD,
	CBL_CHANGE_RDP_Level_CMD
};

/* -------------------- Software Interfaces Definitions -------------------- */
void BL_Print_Message(char* format, ...)
{
	char Message[100] = {0};
	/* holds the information needed by va_start, va_arg, va_end */
	va_list args;
	/* Enables access to the variable arguments */
	va_start(args, format);
	/* Write formatted data from variable argument list to string */
	vsprintf(Message, format, args);

	#if (BL_DEBUG_METHOD == BL_ENABLE_UART_DEBUG_MESSAGE)
		/* Trasmit the formatted data through the defined UART */
		HAL_UART_Transmit(BL_DEBUG_UART, (uint8_t *)Message, sizeof(Message), HAL_MAX_DELAY);
	#elif (BL_DEBUG_METHOD == BL_ENABLE_SPI_DEBUG_MESSAGE)
		/* Trasmit the formatted data through the defined SPI */
	#elif (BL_DEBUG_METHOD == BL_ENABLE_CAN_DEBUG_MESSAGE)
		/* Trasmit the formatted data through the defined CAN */
	#endif

	/* Performs cleanup for a va_list object initialized by a call to va_start */
	va_end(args);
}

BL_Status_t BL_UART_Fetch_Host_Command(void)
{
	BL_Status_t BL_Status = BL_NACK;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t Data_Length = 0;

	memset(BL_Host_Buffer, 0, BL_HOST_RX_BUFFER_LENGTH);

	HAL_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, BL_Host_Buffer, 1, HAL_MAX_DELAY);
	if(HAL_OK == HAL_Status)
	{
		Data_Length = BL_Host_Buffer[0];
		HAL_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, &BL_Host_Buffer[1], Data_Length, HAL_MAX_DELAY);
		if(HAL_OK == HAL_Status)
		{
			BL_Status = BL_OK;

			switch(BL_Host_Buffer[1])
			{
				case CBL_GET_VER_CMD:
					BootLoader_Get_Version(BL_Host_Buffer);
					break;
				case CBL_GET_HELP_CMD:
					BootLoader_Get_Help(BL_Host_Buffer);
					break;
				case CBL_GET_CID_CMD:
					BootLoader_Get_Chip_Identification_Number(BL_Host_Buffer);
					break;
				case CBL_GET_RDP_STATUS_CMD:
					BootLoader_Read_Protection_Level(BL_Host_Buffer);
					break;
				case CBL_GO_TO_ADDR_CMD:
					BootLoader_Jump_to_Address(BL_Host_Buffer);
					break;
				case CBL_FLASH_ERASE_CMD:
					BootLoader_Erase_Flash(BL_Host_Buffer);
					break;
				case CBL_MEM_WRITE_CMD:
					BootLoader_Memory_Write(BL_Host_Buffer);
					break;
				case CBL_ED_W_PROTECT_CMD:
					BootLoader_Enable_RW_Protection(BL_Host_Buffer);
					break;
				case CBL_MEM_READ_CMD:
					BootLoader_Memory_Read(BL_Host_Buffer);
					break;
				case CBL_READ_SECTOR_STATUS_CMD:
					BootLoader_Read_Sector_Protection_Status(BL_Host_Buffer);
					break;
				case CBL_OTP_READ_CMD:
					BootLoader_Read_OTP(BL_Host_Buffer);
					break;
				case CBL_CHANGE_RDP_Level_CMD:
					BootLoader_Change_Read_Protection_Level(BL_Host_Buffer);
					break;
				default:
					BL_Status = BL_NACK;
					#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
						BL_Print_Message("Invalid Command Code Received From Host !! \n");
					#endif
					break;
			}
		}
		else
		{
			BL_Status = BL_NACK;
		}
	}
	else
	{
		BL_Status = BL_NACK;
	}

	return BL_Status;
}

/* -------------------- Static Functions Definitions -------------------- */
static void BootLoader_Get_Version(uint8_t *Host_Buffer)
{
	uint8_t BL_Version[4] = {CBL_VENDOR_ID, CBL_SW_MAJOR_VERSION, CBL_SW_MINOR_VERSION, CBL_SW_PATCH_VERSION};
	uint8_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;

	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Read the bootloader version from the MCU \n");
	#endif

	/* Extract packet length and CRC32 sent by the host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC verification */
	if(CRC_VERIFICATION_PASSED == BootLoader_CRC_Verify(Host_Buffer, (Host_CMD_Packet_Len - CRC_TYPE_SIZE_BYTE), Host_CRC32))
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Passed \n");
		#endif
		/* Report BootLoader version to Host */
		BootLoader_Send_ACK(4);
		BootLoader_Send_Data_to_Host(BL_Version, 4);
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
			BL_Print_Message("Bootloader Ver: %d.%d.%d \n", BL_Version[1], BL_Version[2], BL_Version[3]);
		#endif
	}
	else
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Failed \n");
		#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Get_Help(uint8_t *Host_Buffer)
{
	uint8_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;

	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Read the commands supported by the bootloader \n");
	#endif

	/* Extract packet length and CRC32 sent by the host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC verification */
	if(CRC_VERIFICATION_PASSED == BootLoader_CRC_Verify(Host_Buffer, (Host_CMD_Packet_Len - CRC_TYPE_SIZE_BYTE), Host_CRC32))
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Passed \n");
		#endif
		/* Report Supported CMDs to Host */
		BootLoader_Send_ACK(12);
		BootLoader_Send_Data_to_Host(BL_Supported_CMDs, 12);
	}
	else
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Failed \n");
		#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Get_Chip_Identification_Number(uint8_t *Host_Buffer)
{
	uint8_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint16_t MCU_Identification_Number = 0;

	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Read the MCU chip identification number \n");
	#endif

	/* Extract packet length and CRC32 sent by the host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC verification */
	if(CRC_VERIFICATION_PASSED == BootLoader_CRC_Verify(Host_Buffer, (Host_CMD_Packet_Len - CRC_TYPE_SIZE_BYTE), Host_CRC32))
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Passed \n");
		#endif
		/* Get MCU chip identification number */
		MCU_Identification_Number = (uint16_t)(DBGMCU->IDCODE & 0xFFF);
		/* Report MCU chip identification number to Host */
		BootLoader_Send_ACK(2);
		BootLoader_Send_Data_to_Host((uint8_t *)&MCU_Identification_Number, 2);
	}
	else
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Failed \n");
		#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Read_Protection_Level(uint8_t *Host_Buffer)
{
	uint8_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint8_t Read_Level = 0;

	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Read the FLASH ReadOut Protection level \n");
	#endif

	/* Extract packet length and CRC32 sent by the host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC verification */
	if(CRC_VERIFICATION_PASSED == BootLoader_CRC_Verify(Host_Buffer, (Host_CMD_Packet_Len - CRC_TYPE_SIZE_BYTE), Host_CRC32))
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Passed \n");
		#endif

		/* Read Protection Level */
		Read_Level = BootLoader_Get_RDP_Level();
		/* Report Valid Protection Level to Host */
		BootLoader_Send_ACK(1);
		BootLoader_Send_Data_to_Host((uint8_t *)&Read_Level, 1);
	}
	else
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Failed \n");
		#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Jump_to_Address(uint8_t *Host_Buffer)
{
	uint8_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint32_t Host_Jump_Address = 0;
	uint8_t Address_Verification = ADDRESS_IS_INVALID;

	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Jump bootloader to specified address \n");
	#endif

	/* Extract packet length and CRC32 sent by the host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC verification */
	if(CRC_VERIFICATION_PASSED == BootLoader_CRC_Verify(Host_Buffer, (Host_CMD_Packet_Len - CRC_TYPE_SIZE_BYTE), Host_CRC32))
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Passed \n");
		#endif

		BootLoader_Send_ACK(1);
		/* Extract Host Jump Address */
		Host_Jump_Address = *((uint32_t *)&Host_Buffer[2]);
		Address_Verification = Host_Address_Verification(Host_Jump_Address);
		if(ADDRESS_IS_VALID == Address_Verification)
		{
			#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
				BL_Print_Message("Address Verification Is Valid \n");
			#endif

			/* Prepare address to jump, LSB must be set to 1 ---> thumb state */
			Jump_Ptr Jump_Address = (Jump_Ptr)(Host_Jump_Address | 0x01);
			#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
				BL_Print_Message("Jump to : 0x%X \n", Jump_Address);
			#endif
		}
		else
		{
			#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
				BL_Print_Message("Address Verification Is Invalid \n");
			#endif
		}
		/* Report address verification status(valid/invalid) to Host */
		BootLoader_Send_Data_to_Host((uint8_t *)&Address_Verification, 1);
	}
	else
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Failed \n");
		#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Erase_Flash(uint8_t *Host_Buffer)
{
	uint8_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint8_t Erase_Status = UNSUCCESSFUL_ERASE;

	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Mass erase or sector erase of the user flash \n");
	#endif

	/* Extract packet length and CRC32 sent by the host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC verification */
	if(CRC_VERIFICATION_PASSED == BootLoader_CRC_Verify(Host_Buffer, (Host_CMD_Packet_Len - CRC_TYPE_SIZE_BYTE), Host_CRC32))
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Passed \n");
		#endif

		BootLoader_Send_ACK(1);
		/* Perform Mass erase or sector erase of the user flash */
		Erase_Status = Perform_Flash_Erase(Host_Buffer[2], Host_Buffer[3]);
		if(SUCCESSFUL_ERASE == Erase_Status)
		{
			#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
				BL_Print_Message("Successful Flash Erase \n");
			#endif
		}
		else
		{
			#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
				BL_Print_Message("Unsuccessful Flash Erase \n");
			#endif
		}
		/* Report Erase status(successful/unsuccessful) to Host */
		BootLoader_Send_Data_to_Host((uint8_t *)&Erase_Status, 1);
	}
	else
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Failed \n");
		#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Memory_Write(uint8_t *Host_Buffer)
{
	uint8_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint32_t Host_Address = 0;
	uint8_t Payload_Len = 0;
	uint8_t Address_Verification = ADDRESS_IS_INVALID;
	uint8_t Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;

	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Write data into different memories of the MCU \n");
	#endif

	/* Extract packet length and CRC32 sent by the host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC verification */
	if(CRC_VERIFICATION_PASSED == BootLoader_CRC_Verify(Host_Buffer, (Host_CMD_Packet_Len - CRC_TYPE_SIZE_BYTE), Host_CRC32))
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Passed \n");
		#endif

		BootLoader_Send_ACK(1);
		/* Extract The Start Address From Host Packet */
		Host_Address = *((uint32_t *)&Host_Buffer[2]);
		Address_Verification = Host_Address_Verification(Host_Address);
		if(ADDRESS_IS_VALID == Address_Verification)
		{
			#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
				BL_Print_Message("Address Verification Is Valid \n");
			#endif

			/* Extract Payload Length From Host Packet */
			Payload_Len	= Host_Buffer[6];
			Flash_Payload_Write_Status = Flash_Memory_Write_Payload((uint8_t *)&Host_Buffer[7], Host_Address, Payload_Len);
			if(FLASH_PAYLOAD_WRITE_PASSED == Flash_Payload_Write_Status)
			{
				#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
					BL_Print_Message("Flash Payload Write Passed \n");
				#endif
			}
			else
			{
				#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
					BL_Print_Message("Flash Payload Write Failed \n");
				#endif
			}
			/* Report flash payload write status(passed/failed) to Host */
			BootLoader_Send_Data_to_Host((uint8_t *)&Flash_Payload_Write_Status, 1);
		}
		else
		{
			#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
				BL_Print_Message("Address Verification Is Invalid \n");
			#endif
			/* Report address verification status is invalid to Host */
			BootLoader_Send_Data_to_Host((uint8_t *)&Address_Verification, 1);
		}
	}
	else
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Failed \n");
		#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Enable_RW_Protection(uint8_t *Host_Buffer)
{

}

static void BootLoader_Memory_Read(uint8_t *Host_Buffer)
{

}

static void BootLoader_Read_Sector_Protection_Status(uint8_t *Host_Buffer)
{

}

static void BootLoader_Read_OTP(uint8_t *Host_Buffer)
{

}

static void BootLoader_Change_Read_Protection_Level(uint8_t *Host_Buffer)
{
	uint8_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint8_t Host_RDP_Level = 0;
	uint8_t RDP_Level_Status = RDP_LEVEL_CHANGE_INVALID;

	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Change the FLASH ReadOut Protection level \n");
	#endif

	/* Extract packet length and CRC32 sent by the host */
	Host_CMD_Packet_Len = Host_Buffer[0] + 1;
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_SIZE_BYTE));

	/* CRC verification */
	if(CRC_VERIFICATION_PASSED == BootLoader_CRC_Verify(Host_Buffer, (Host_CMD_Packet_Len - CRC_TYPE_SIZE_BYTE), Host_CRC32))
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Passed \n");
		#endif

		BootLoader_Send_ACK(1);
		/* Extract Host Read Protection Level */
		Host_RDP_Level = Host_Buffer[2];
		/* Warning: When enabling it's no more possible to go back to level 1 or 0 */
		if((CBL_RDP_LEVEL_2 == Host_RDP_Level))
		{
			/* do nothing */
			#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
				BL_Print_Message("Warning!! Trying To Enable RDP Level 2 \n");
			#endif
			RDP_Level_Status = RDP_LEVEL_CHANGE_INVALID;
		}
		else
		{
			RDP_Level_Status = Change_RDP_Level(Host_RDP_Level);
		}
		/* Report RDP level status(valid/invalid) to Host */
		BootLoader_Send_Data_to_Host((uint8_t *)&RDP_Level_Status, 1);
	}
	else
	{
		#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
			BL_Print_Message("CRC Verification Failed \n");
		#endif
		BootLoader_Send_NACK();
	}
}

static uint8_t BootLoader_CRC_Verify(uint8_t *pData, uint8_t Data_Len, uint32_t Host_CRC)
{
	uint8_t CRC_Status = CRC_VERIFICATION_FAILED;
	uint32_t MCU_CRC_Calculated = 0;
	uint8_t Data_Counter = 0;
	uint32_t Data_Buffer = 0;

	/* Calculate CRC32 */
	for (Data_Counter = 0; Data_Counter < Data_Len; Data_Counter++)
	{
		Data_Buffer = (uint32_t)(pData[Data_Counter]);
		MCU_CRC_Calculated = HAL_CRC_Accumulate(CRC_ENGINE_OBJ, &Data_Buffer, 1);
	}

	/* Reset CRC Data Register (Calculation Unit) */
	__HAL_CRC_DR_RESET(CRC_ENGINE_OBJ);
	/* Compare the Host CRC and Calculated CRC */
	if(MCU_CRC_Calculated == Host_CRC)
	{
		CRC_Status = CRC_VERIFICATION_PASSED;
	}
	else
	{
		CRC_Status = CRC_VERIFICATION_FAILED;
	}

	return CRC_Status;
}

static void BootLoader_Send_ACK(uint8_t Reply_Len)
{
	uint8_t Ack_Value[2] = {0};
	Ack_Value[0] = CBL_SEND_ACK;
	Ack_Value[1] = Reply_Len;
	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Bootloader Sending ACK \n");
	#endif
	BootLoader_Send_Data_to_Host(Ack_Value, 2);
}

static void BootLoader_Send_NACK(void)
{
	uint8_t Ack_Value = CBL_SEND_NACK;
	#if (BL_DEBUG_INFO_CFG == DEBUG_INFO_ENABLE)
		BL_Print_Message("Bootloader Sending NACK \n");
	#endif
	BootLoader_Send_Data_to_Host(&Ack_Value, 1);
}

static void BootLoader_Send_Data_to_Host(uint8_t *Host_Buffer, uint8_t Data_Len)
{
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, Host_Buffer, Data_Len, HAL_MAX_DELAY);
}

static uint8_t BootLoader_Get_RDP_Level(void)
{
	FLASH_OBProgramInitTypeDef FLASH_OBProgram;
	HAL_FLASHEx_OBGetConfig(&FLASH_OBProgram);
	return ((uint8_t)(FLASH_OBProgram.RDPLevel));
}

static uint8_t Host_Address_Verification(uint32_t Jump_Address)
{
	uint8_t Address_Verification = ADDRESS_IS_INVALID;

	if((Jump_Address >= SRAM1_BASE) && (Jump_Address <= STM32F407_SRAM1_END))
	{
		Address_Verification = ADDRESS_IS_VALID;
	}
	else if((Jump_Address >= SRAM2_BASE) && (Jump_Address <= STM32F407_SRAM2_END))
	{
		Address_Verification = ADDRESS_IS_VALID;
	}
	else if((Jump_Address >= CCMDATARAM_BASE) && (Jump_Address <= STM32F407_SRAM3_END))
	{
		Address_Verification = ADDRESS_IS_VALID;
	}
	else if((Jump_Address >= FLASH_BASE) && (Jump_Address <= STM32F407_FLASH_END))
	{
		Address_Verification = ADDRESS_IS_VALID;
	}
	else
	{
		Address_Verification = ADDRESS_IS_INVALID;
	}

	return Address_Verification;
}

static uint8_t Perform_Flash_Erase(uint8_t Sector_Number, uint8_t Number_Of_Sectors)
{
	uint8_t Erase_Status = UNSUCCESSFUL_ERASE;
	uint8_t Remaining_Sectors = 0;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	FLASH_EraseInitTypeDef EraseInit;
	uint32_t SectorError = 0;

	if(Number_Of_Sectors > FLASH_MAX_NUMBER_OF_SECTORS)
	{
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
			BL_Print_Message("Flash Invalid Number Of Sectors!! \n");
		#endif
		Erase_Status = INVALID_NUMBER_OF_SECTORS;
	}
	else
	{
		if((Sector_Number <= FLASH_MAX_SECTOR_NUMBER) || (FLASH_MASS_ERASE == Sector_Number))
		{
			/* Check if user needs Mass erase */
			if(FLASH_MASS_ERASE == Sector_Number)
			{
				#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
					BL_Print_Message("Flash Mass erase activation \n");
				#endif
				EraseInit.TypeErase = FLASH_TYPEERASE_MASSERASE; /* Flash Mass erase */
			}
			else /* User needs Sector erase */
			{
				#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
					BL_Print_Message("Flash Sector erase activation \n");
				#endif
				Remaining_Sectors = FLASH_MAX_NUMBER_OF_SECTORS - Sector_Number;
				if(Number_Of_Sectors > Remaining_Sectors)
				{
					Number_Of_Sectors = Remaining_Sectors;
				}
				else{ /* nothing */}

				EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS; /* Flash Sector erase */
				EraseInit.Sector = Sector_Number;
				EraseInit.NbSectors = Number_Of_Sectors;
			}
			EraseInit.Banks = FLASH_BANK_1; /* Bank 1 */
			EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3; /* Device operating range: 2.7V to 3.6V */

			/* Unlock the FLASH control register access */
			HAL_Status = HAL_FLASH_Unlock();
			/* Perform a mass erase or erase the specified FLASH memory sectors */
			HAL_Status = HAL_FLASHEx_Erase(&EraseInit, &SectorError);
			if(HAL_SUCCESSFUL_ERASE == HAL_Status)
			{
				Erase_Status = SUCCESSFUL_ERASE;
			}
			else
			{
				Erase_Status = UNSUCCESSFUL_ERASE;
			}
			/* Locks the FLASH control register access */
			HAL_Status = HAL_FLASH_Lock();
		}
		else
		{
			#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
				BL_Print_Message("Flash Invalid Sector Number!! \n");
			#endif
			Erase_Status = INVALID_SECTOR_NUMBER;
		}
	}

	return Erase_Status;
}

static uint8_t Flash_Memory_Write_Payload(uint8_t *Host_Payload, uint32_t Payload_Start_Address, uint8_t Payload_Len)
{
	uint8_t Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t Payload_Counter = 0;

	/* Unlock the FLASH control register access */
	HAL_Status = HAL_FLASH_Unlock();
	if(HAL_OK == HAL_Status)
	{
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
			BL_Print_Message("Passed -> Unlock the FLASH Option Control Registers access \n");
		#endif

		for(Payload_Counter = 0; Payload_Counter < Payload_Len; Payload_Counter++)
		{
			/* Program byte at a specified address */
			HAL_Status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, (Payload_Start_Address + Payload_Counter), Host_Payload[Payload_Counter]);
			if(HAL_OK != HAL_Status)
			{
				Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
				break;
			}
			else{/* nothing */}
		}

		/* Check if all payload bytes were written into the Flash*/
		if((Payload_Counter == Payload_Len) && (HAL_OK == HAL_Status))
		{
			#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
				BL_Print_Message("Passed -> Write Payload Into the FLASH \n");
			#endif
			Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_PASSED;
		}
		else
		{
			#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
				BL_Print_Message("Failed -> Write Payload Into the FLASH \n");
			#endif
		}

		/* Locks the FLASH control register access */
		HAL_Status = HAL_FLASH_Lock();
	}
	else
	{
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
			BL_Print_Message("Failed -> Unlock the FLASH Option Control Registers access \n");
		#endif
		Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
	}

	return Flash_Payload_Write_Status;
}

static uint8_t Change_RDP_Level(uint8_t RDP_Level)
{
	uint8_t RDP_Level_Status = RDP_LEVEL_CHANGE_INVALID;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	FLASH_OBProgramInitTypeDef FLASH_OBProgram;

	/* Unlock the FLASH control register access */
	HAL_Status = HAL_FLASH_Unlock();
	if(HAL_OK == HAL_Status)
	{
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
			BL_Print_Message("Passed -> Unlock the FLASH Option Control Registers access \n");
		#endif
		FLASH_OBProgram.OptionType = OPTIONBYTE_RDP;
		FLASH_OBProgram.Banks = FLASH_BANK_1;
		FLASH_OBProgram.RDPLevel = RDP_Level;

		/* Program option bytes */
		HAL_Status = HAL_FLASHEx_OBProgram(&FLASH_OBProgram);
		if(HAL_OK == HAL_Status)
		{
			#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
				BL_Print_Message("Passed -> Program option bytes \n");
			#endif
			/* Launch the option byte loading */
			HAL_Status = HAL_FLASH_OB_Launch();
			if(HAL_OK == HAL_Status)
			{
				RDP_Level_Status = RDP_LEVEL_CHANGE_VALID;
			}
			else
			{
				RDP_Level_Status = RDP_LEVEL_CHANGE_INVALID;
			}
		}
		else
		{
			#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
				BL_Print_Message("Failed -> Program option bytes \n");
			#endif
			RDP_Level_Status = RDP_LEVEL_CHANGE_INVALID;
		}

		/* Locks the FLASH control register access */
		HAL_Status = HAL_FLASH_Lock();
	}
	else
	{
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
			BL_Print_Message("Failed -> Unlock the FLASH Option Control Registers access \n");
		#endif
		RDP_Level_Status = RDP_LEVEL_CHANGE_INVALID;
	}

	return RDP_Level_Status;
}
