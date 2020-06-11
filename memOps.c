/**
 @file memOps.c
 @SWVersion @SW_VERSION

 @brief    Embedded Flash Memory driver.
 @details  This module manages all the services for reading/writing
 operations on the embedded Flash memory.

 Project ATB
 Platform STM32L451xx

 (c) Copyright 2018 Vodafone Automotive - All Rights Reserved.
 */
/******************************************************************/
/**
 *
 * @defgroup group_hal_flash Flash Memory
 * @ingroup  group_hal
 * @brief    Embedded Flash Memory Driver
 * @details
 * | Memory Layout 	| Page range 	| Address range  		| Length in KB 	|
 * | :------------:	| :-----------:	| :-------------------: | :-----------: |
 * | Bootloader    	| 0 - 27     	| 0x8000000 - 0x800E000 | 112			|
*  | CONFIG1  		| 28 - 29   	| 0x800E000 - 0x800F000 | 4				|
 * | CONFIG2  		| 30 - 31   	| 0x800F000 - 0x8010000 | 4				|
 * | Application  	| 32 - 143   	| 0x8010000 - 0x8048000 | 452			|
 * | Firmware  		| 144 - 255   	| 0x8048000 - 0x8080000 | 452			|
 * |				|				|						| 1024			|
 */
/*******************************************************************/
/******************************************************************/
/**
 * @addtogroup group_hal_flash
 * @{
 **/
/***************************************************************/

/*==================================================================
 INCLUDE FILES
 ==================================================================*/
#include "memOps.h"
#include "stm32l4xx.h"

/********************************************************************/
/** @brief Erase MCU flash memory portion between \p startPage and \p endPage.
 *
 * @param[in]  startPage - first page to be erased. Should be between 0 - 255.
 * @param[in]  endPage   - last page to be erased. Should be between 0 - 255.
 *
 * @return @c true if the operation is completed successfully,
 * otherwise @c false if \p startPage > \p endPage
 *
 * */
/********************************************************************/
bool MemOps_EraseFlash(uint8_t startPage, uint8_t endPage)
{
    bool retVal = true;
    bool status;
    uint32_t pageError;
    FLASH_EraseInitTypeDef eraseInit;

    if (startPage > endPage)
    {
        retVal = false;
    }
    else
    {
        eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInit.Banks = FLASH_BANK_1;
        eraseInit.Page = startPage;
        eraseInit.NbPages = ((uint32_t)endPage - (uint32_t)startPage) + 1u;

        status = (HAL_OK == HAL_FLASH_Unlock()) ? (true) : (false);
        /* Clear all FLASH flags */
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);

        if (status)
        {
            status = (HAL_FLASHEx_Erase(&eraseInit, &pageError) == HAL_OK) ? (true) : (false);
        }

        (void)HAL_FLASH_Lock();
    }

	return (retVal);
}

/********************************************************************/
/** @brief Write/Program @p length bytes of MCU flash memory between \p srcAddress and \p dstAddress.
 *
 * @param[in]  srcAddress - source memory address where data to be copied begins.
 * Should be between 0x8000000UL and 0x8080000UL.
 * @param[in]  dstAddress - destination memory address where data will be copied to.
 * Should be between 0x8000000UL and 0x8080000UL.
 * @param[in]  length     - length of data to be copied, in bytes
 *
 * @return @c true if the operation is completed successfully, otherwise @c false if
 * 	- \p dstAddress is not 8 bytes aligned
 * 	- \p length is not multiple of 8 bytes
 * */
/********************************************************************/
bool MemOps_WriteFlash(uint32_t srcAddress, uint32_t dstAddress, uint32_t length)
{
	bool b_VarCheck;
    bool status;
    bool retval;

    b_VarCheck = (uint8_t)((dstAddress % 8u) != 0u) | (uint8_t)((length % 8u) != 0u);
    if (b_VarCheck != 0u)
    {
        /* cannot handle unaligned access */
        retval = false;
    }
    else
    {
        status = (HAL_FLASH_Unlock() == HAL_OK) ? true : false;

		/* Clear all FLASH flags */
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);

		while (status && (length != 0u))
		{
			status = (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, dstAddress, *((uint64_t*) srcAddress)) == HAL_OK)?(true):(false);
			srcAddress += 8u;
			dstAddress += 8u;
			length -= 8u;
		}

		if(HAL_FLASH_Lock() == HAL_OK)
		{
			/* misra */
		}

		retval = status;
	}
	return retval;

}

/*************************************************************************************************************/
/**
 * @}
 **/
/*************************************************************************************************************/

