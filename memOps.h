/**
 @file memOps.h
 @SWVersion SW_VERSION

 @brief   Embedded Flash Memory.
 @details Declarations required by the services for reading/writing
 operations on the embedded Flash memory.

 Project ATB
 Platform STM32L451xx
 
 (c) Copyright YEAR Vodafone Automotive - All Rights Reserved.
 */
#ifndef MEMOPS_H_
#define MEMOPS_H_

/*==================================================================
*                              INCLUDE FILES
==================================================================*/
#include <stdbool.h>
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

/*===================================================================                                        					
 *                                  ENUMS
 ===================================================================*/

/*===================================================================					
 *                      STRUCTURES AND OTHER TYPEDEFS
 ===================================================================*/

/*==================================================================					
 *                     GLOBAL VARIABLE DECLARATIONS
 ===================================================================*/

/*===================================================================
*                          FUNCTION PROTOTYPES
===================================================================*/
bool MemOps_EraseFlash(uint8_t startPage, uint8_t endPage);
bool MemOps_WriteFlash(uint32_t srcAddress, uint32_t dstAddress, uint32_t length);

#endif /* MEMOPS_H_ */
