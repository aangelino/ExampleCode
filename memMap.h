/**
 @file memMap.h
 @SWVersion @SW_VERSION

 @brief   FLASH Memory Mappings.
 @details Macros definitions for application, bootloader, firmware and configurations sections
 managed by MCU.

 Project ATB
 Platform STM32L451xx

 (c) Copyright 2018 Vodafone Automotive - All Rights Reserved.
 */

#ifndef MEMMAP_H_
#define MEMMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================                                  					
 *                             INCLUDE FILES
 ==================================================================*/
#include <stdint.h>


/*==================================================================                                                 					
 *                      SOURCE FILE VERSION INFORMATION
 ==================================================================*/

/*==================================================================					
 *                           FILE VERSION CHECKS
 ==================================================================*/

/*==================================================================					
 *                                CONSTANTS
 ==================================================================*/

/*==================================================================					
 *                           DEFINES AND MACROS
 ==================================================================*/
#define BOOTLOADER_BASE_ADDRESS         0x8000000UL             /**< base address of the bootloader image in MCU flash memory*/
#define BOOTLOADER_BOOT_ADDRESS         (BOOTLOADER_BASE_ADDRESS) /**< boot address of the bootloader */
#define BOOTLOADER_END_ADDRESS          0x800E000UL             /**< top of bootloader MCU flash address space */
#define BOOTLOADER_BASE_PAGE            0u                       /**< base page of bootloader MCU flash memory */
#define BOOTLOADER_END_PAGE             27u                      /**< last page of bootloader MCU flash memory */
#define BOOTLOADER_SIZE                 (BOOTLOADER_END_ADDRESS - BOOTLOADER_BASE_ADDRESS)  /**< bootloader maximum size in bytes - 56KB */

#define CONFIG1_BASE_ADDRESS            0x800E000UL             /**< base address of the config section in MCU flash memory*/
#define CONFIG1_END_ADDRESS             0x800F000UL             /**< top of config section MCU flash address space */
#define CONFIG1_BASE_PAGE               28u                      /**< base page of config section MCU flash memory */
#define CONFIG1_END_PAGE                29u                      /**< last page of config section MCU flash memory */
#define CONFIG1_SIZE                    (CONFIG1_END_ADDRESS - CONFIG1_BASE_ADDRESS)    /**< config section maximum size in bytes - 4KB */

#define CONFIG2_BASE_ADDRESS            0x800F000UL /**< base address of the config section in MCU flash memory*/
#define CONFIG2_END_ADDRESS             0x8010000UL /**< top of config section MCU flash address space */
#define CONFIG2_BASE_PAGE               30u  /**< base page of config section MCU flash memory */
#define CONFIG2_END_PAGE                31u  /**< last page of config section MCU flash memory */
#define CONFIG2_SIZE                    (CONFIG2_END_ADDRESS - CONFIG2_BASE_ADDRESS)    /**< config section maximum size in bytes - 4KB */

#define APPLICATION_BASE_ADDRESS        0x8010000UL /**< base address of the application firmware in MCU flash memory*/
#define APPLICATION_BOOT_ADDRESS        0x8010200UL /**< application firmware start address */
#define APPLICATION_END_ADDRESS         0x8048000UL /**< top of config section MCU flash address space */
#define APPLICATION_BASE_PAGE           32u  /**< first page of application firmware */
#define APPLICATION_END_PAGE            143u /**< last page of application firmware */
#define APPLICATION_MAXLENGTH           (APPLICATION_END_ADDRESS - APPLICATION_BOOT_ADDRESS) /**< application firmware maximum usable size -> (224KB - 512B) */
#define APPLICATION_SIZE                (APPLICATION_END_ADDRESS - APPLICATION_BASE_ADDRESS) /**< application firmware section total size -> 224KB */

#define FIRMWARE_UPDATE_BASE_ADDRESS    0x8048000UL /**< base address of the application firmware update section */
#define FIRMWARE_UPDATE_BOOT_ADDRESS    0x8048200UL /**< application firmware update boot address */
#define FIRMWARE_UPDATE_END_ADDRESS     0x8080000UL /**< top of application firmware update section in MCU flash */
#define FIRMWARE_UPDATE_BASE_PAGE       144u /**< first page of the firmware update section */
#define FIRMWARE_UPDATE_END_PAGE        255u /**< last page of the firmware update section */
#define FIRMWARE_UPDATE_MAXLENGTH       (FIRMWARE_UPDATE_END_ADDRESS - FIRMWARE_UPDATE_BOOT_ADDRESS) /**< application firmware update maximum usable size -> (224KB - 512B) */
#define FIRMWARE_UPDATE_SIZE            (FIRMWARE_UPDATE_END_ADDRESS - FIRMWARE_UPDATE_BASE_ADDRESS) /**< application firmware update section total size -> 224K */


/*===================================================================                                        					
 *                                  ENUMS
 ===================================================================*/

/*===================================================================					
 *                      STRUCTURES AND OTHER TYPEDEFS
 ===================================================================*/
/** @brief firmware bootup check parameters */
typedef struct __attribute__((packed))
{
    uint32_t length;    /**< firmware length in bytes */
    uint32_t crc;       /**< 32 bit CRC value of the firmware image */
}sFirmwareParams_t;


/*==================================================================					
 *                     GLOBAL VARIABLE DECLARATIONS
 ===================================================================*/

/*===================================================================					
 *                         FUNCTION PROTOTYPES
 ===================================================================*/



#ifdef __cplusplus
}
#endif
#endif /* MEMMAP_H_ */
