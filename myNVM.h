/*
 * Nvm.h
 *
 *  Created on: 20 Apr 2020
 *      Author: aangelino
 */

#ifndef NVM_H_
#define NVM_H_

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#define NELEM_TYPE_A	74                     /**< */
#define NELEM_TYPE_AA	29                     /**< */
#define NELEM_TYPE_M	487                    /**< */
#define NELEM_TYPE_V	8                      /**< */
#define NELEM_TYPE_L	96                      /**< */
//#define NELEM_TYPE_I	2                      /**< */
//#define NELEM_TYPE_AB	2                      /**< */


typedef struct
{
	uint16_t A[NELEM_TYPE_A];                        /**< */
	uint16_t AA[NELEM_TYPE_AA];                        /**< */
	uint16_t M[NELEM_TYPE_M];                        /**< */
    uint16_t V[NELEM_TYPE_V];                        /**< */
   // uint16_t I[NELEM_TYPE_I];                        /**< */
    uint16_t L[NELEM_TYPE_L];                        /**< */
    //uint16_t AB[NELEM_TYPE_AB];                      /**< */

    uint16_t StartStopCnt;
    uint16_t u16_crc;                  /**< Cyclic redundancy check */
} sNvmStorage_t;

typedef enum
{
	A_PARAMETERS,
	AA_PARAMETERS,
	M_PARAMETERS,
	V_PARAMETERS,
	L_PARAMETERS,
}sParametersGroup;

void NVM_Init_Temporany(void);
bool saveOvenType(uint32_t dataOvenType);
bool NVM_Init_struct(void);
bool NVM_SaveStorage(void);
void set_NVM_Param(sParametersGroup groupParam, uint16_t index, uint16_t val);
void return_NVM_Param(sParametersGroup groupParam, uint16_t index);

#endif /* NVM_H_ */
