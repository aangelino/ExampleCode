/**
 @file    NVM.c
 @SWVersion @SW_VERSION

 @brief		NVM Module
 @details	This module implements a Non Volatile Memory management using EEPROM Flash emulation solution.

 Project ATB
 Platform STM32L451xx

 (c) Copyright 2017 Vodafone Automotive - All Rights Reserved.
 */
/******************************************************************/
/**
 *
 * @defgroup group_mw_nvm  NVM
 * @ingroup group_mw
 * @brief NVM management module
 */
/*******************************************************************/
/*******************************************************************/
/**
 * @addtogroup group_mw_nvm
 * @{
 **/
/******************************************************************/
#include "NVM.h"

#include "memMap.h"
#include "memOps.h"
#include "crc.h"
#include "static_assert.h"

#include "msg.h"

#include <string.h>
#include <stddef.h>

#include "rtx_os.h"

#include "comm.h"

//#define PRODUCTION_RELEASE 1	/* enable production server's parameters */

/** @brief Information stored in EEPROM Flash memory */
typedef struct
{
    sConfigParameters_t config;		/**< Configuration parameter information */
    sDataIdentifiers_t did;			/**< Data identifier information */
    sDiagParameters_t diag;			/**< Diagnostic information */
    sBattData_t battData;			/**< Battery information */
    uint8_t resetCode;				/**< Reset code information */
    uint8_t dbgModuleID;			/**< Reset code information */
    uint8_t dbgErroID;  			/**< Reset code information */
    uint8_t dbgThreadName[6];       /**< Store Thread Name */
    uint8_t wdgForceDisable;        /**<  */
    uint16_t u16_crc;	    		/**< Cyclic redundancy check */
} sNvmStorage_t;

static const sConfigParameters_t s_defaultConfig =
    {
        .rfTransmissionPower = CONFIG_RF_TRANSM_PWR_RANGE_MAX,
        .rfTransmissionChannel = 10,
        .rfDutyOn = 400,
        .rfDutyOff = 600,
#ifdef DEBUG
        .wakeupPeriod = 1, /* minutes */
        .wakeupTime = 180, /* seconds */
        .wakeupCount = 1,
#else /* RELEASE */
        .wakeupPeriod = 1, //1430, /* minutes 23h50m */
        .wakeupTime = 180, //120,    /* seconds */
        .wakeupCount = 1, //3,
#endif
        .communicationTimeout = 60, /* seconds  used as INI wait time */
        .networkTimeout = 180,
#if 0 /* set 1 for Romanian SIM */
        .networkApn = "live.vodafone.com",
        .networkLogin = "live",
        .networkPassword = "vodafone",
#else /* ITALY APN settings  */
       .networkApn = "internet.mts.ru",
       .networkLogin = "mts",
        .networkPassword = "mts",
//		        .networkApn = "mobile.vodafone.it",
//		        .networkLogin = "",
//		        .networkPassword = "",
#endif
#ifdef PRODUCTION_RELEASE
		.gprsGatewayUrl = "195.149.197.243",
		.gprsGatewayPort = 1002,
		.smsCallNumber = { "+79851503234" },
#else
		.gprsGatewayUrl = "94.25.115.106",
		.gprsGatewayPort = 1012,
		.smsCallNumber = { "+79853153631" },
#endif
        .transmissionPolicy = GPRS_SMS, /* GPRS_SMS */

    };

static const sBattData_t s_defaultBattData =
    {
        .running = 0,
        .lowPower = 0
    };

static const sDiagParameters_t s_defaultDiagParams =
    {
        .serialNumber = "000096130528",
#ifdef DEBUG
        /* Development Version */
        .softwareVersion = "X058",
#else
        .softwareVersion = "0058",
#endif
        .boardRevision = "00",
    };

static bool s_isInit = false;
static uint8_t s_nvmData[(((sizeof(sNvmStorage_t) + 7u) / 8u) * 8u)] __attribute__((aligned (8)));

/*==================================================================
                       DEFINITION OF FUNCTIONS
==================================================================*/
static inline sBattData_t *getBattData(void);
static inline sDataIdentifiers_t *getDID(void);
static inline sConfigParameters_t *getConfig(void);
static inline sDiagParameters_t *getDiag(void);
static inline uint8_t *getResetCode(void);

/*==================================================================
                       DECLARATION OF FUNCTIONS
==================================================================*/

/********************************************************************/
/** @brief Get information related to the battery
 * @return pointer to battery data information field of #sNvmStorage_t
 * */
/********************************************************************/
static inline sBattData_t *getBattData(void)
{
    return &(((sNvmStorage_t *) s_nvmData)->battData);
}

/********************************************************************/
/** @brief Get information related to data like app software version and
 * hardware version numbers
 * @return pointer to data identifier field of #sNvmStorage_t
 * */
/********************************************************************/
static inline sDataIdentifiers_t *getDID(void)
{
    return &(((sNvmStorage_t *) s_nvmData)->did);
}

/********************************************************************/
/** @brief Get information related to configuration parameters like information
 * about the network, GPRS module, low power mode and so on (see #sConfigParameters_t)
 * @return pointer to configuration parameter field of #sNvmStorage_t
 * */
/********************************************************************/
static inline sConfigParameters_t *getConfig(void)
{
    return &(((sNvmStorage_t *) s_nvmData)->config);
}

/********************************************************************/
/** @brief Get information related to diagnostic information
 * @return pointer to diagnostic parameter field of #sNvmStorage_t
 * */
/********************************************************************/
static inline sDiagParameters_t *getDiag(void)
{
    return &(((sNvmStorage_t *) s_nvmData)->diag);
}

/********************************************************************/
/** @brief Get information related to reset code that identifies the cause of the reset
 * @return pointer to reset code field of #sNvmStorage_t
 * */
/********************************************************************/
static inline uint8_t *getResetCode(void)
{
    return &(((sNvmStorage_t *) s_nvmData)->resetCode);
}

/********************************************************************/
/** @brief Get information related to wdgForceDisable
 * @return pointer to wdgForceDisable field of #sNvmStorage_t
 * */
/********************************************************************/
static inline uint8_t *getWdgCodePtr(void)
{
    return &(((sNvmStorage_t *) s_nvmData)->wdgForceDisable);
}


/********************************************************************/
/** @brief Initialize NVM module configuration with one of the two
 * configuration areas already in memory, or in case of corrupted configuration
 * (checks with CRC) load the default configuration.
 *
 * @return @c true on success,@c false otherwise
 * */
/********************************************************************/
bool NVM_Init(void)
{
    STATIC_ASSERT(sizeof(s_nvmData) <= CONFIG1_SIZE, "NVM data larger than storage space!");

    bool crcValid1 = true;
    bool crcValid2 = true;
    uint16_t crc1 = 0u;
    uint16_t crc2 = 0u;
    bool rc = false;

    if (s_nvmData != NULL)
    {
    	/* Misra 20_3-3 */
    }
    /* check first image CRC */
    memcpy(s_nvmData, (void *) CONFIG1_BASE_ADDRESS, sizeof(s_nvmData));
    crc1 = crcFast((uint8_t *) s_nvmData, offsetof(sNvmStorage_t, u16_crc));
    if (crc1 != ((sNvmStorage_t *) s_nvmData)->u16_crc)
    {   /* crc mismatch */
        crcValid1 = false;
    }

    /*check second image CRC*/
    memcpy(s_nvmData, (void *) CONFIG2_BASE_ADDRESS, sizeof(s_nvmData));
    crc2 = crcFast((uint8_t *) s_nvmData, offsetof(sNvmStorage_t, u16_crc));
    if (crc2 != ((sNvmStorage_t *) s_nvmData)->u16_crc)
    {   /*crc mismatch */
        crcValid2 = false;
    }

    /*recovery handling*/
    if ((crcValid1 == false) && (crcValid2 == false))
    {   /*restore defaults*/
        rc = NVM_RestoreDefaults();
    }
    else if (crcValid1 == false)
    {   /*copy from Image 2 */
        memcpy(s_nvmData, (void *) CONFIG2_BASE_ADDRESS, sizeof(s_nvmData));
        rc = NVM_SaveStorage();
    }
    else if (crcValid2 == false)
    {   /*copy from Image 1*/
        memcpy(s_nvmData, (void *) CONFIG1_BASE_ADDRESS, sizeof(s_nvmData));
        rc = NVM_SaveStorage();
    }
    else
    {
        rc = true;
    }

    {
        uint8_t sw_version_buff[DIAG_SOFTWAREVERSION_MAXLEN]= {0};
        sDiagParameters_t *ptr = NVM_GetDiagParam();
        strncpy((char*)sw_version_buff, (char*)ptr->softwareVersion, DIAG_SOFTWAREVERSION_MAXLEN);

        /* SW version check: default vs NVM */
        if (strncmp((char*) sw_version_buff, (char*) &s_defaultDiagParams.softwareVersion[0],
            DIAG_SOFTWAREVERSION_MAXLEN) != 0)
        {
            /*update NVM SW version*/
        	sDiagParameters_t *diagPtr = NVM_GetDiagParam();
        	memcpy(diagPtr->softwareVersion,&s_defaultDiagParams.softwareVersion[0],(DIAG_SOFTWAREVERSION_MAXLEN + 1u));
            rc = NVM_SaveStorage();
        }
    }


    if (rc)
    {
        s_isInit = true;
    }

    return rc;
}

/********************************************************************/
/** @brief Get information related to the battery
 * @return pointer to battery data information or NULL if NVM is not initialize
 * */
/********************************************************************/
sBattData_t *NVM_GetBattData(void)
{
    sBattData_t *batData = NULL;

    if (s_isInit)
    {
        batData = getBattData();
    }

    return batData;
}

/********************************************************************/
/** @brief Get information related to data like app software version and
 * hardware version numbers
 * @return pointer to data identifier or NULL if NVM is not initialize
 * */
/********************************************************************/
sDataIdentifiers_t *NVM_GetDID(void)
{
    sDataIdentifiers_t *didPtr = NULL;

    if (s_isInit)
    {
    	didPtr = getDID();
    }

    return didPtr;
}

/********************************************************************/
/** @brief Get information related to diagnostic information
 * @return pointer to diagnostic parameter or NULL if NVM is not initialize
 * */
/********************************************************************/
sDiagParameters_t *NVM_GetDiagParam(void)
{
    sDiagParameters_t *diagPtr = NULL;

    diagPtr = getDiag();

    return diagPtr;
}

/********************************************************************/
/** @brief Get information related to configuration parameters (see #sConfigParameters_t)
 * @return pointer to configuration parameter or NULL if NVM is not initialize
 * */
/********************************************************************/
sConfigParameters_t *NVM_GetConfig(void)
{
    sConfigParameters_t *configPtr = NULL;

    if (s_isInit)
    {
    	configPtr = getConfig();
    }

    return configPtr;
}

/********************************************************************/
/** @brief Get information related to reset code that identifies the cause of the reset
 * @return reset code field or 0 if NVM is not initialize
 * */
/********************************************************************/
uint8_t NVM_GetResetCode(void)
{
	uint8_t *resetCodePtr = NULL;
	uint8_t retVal = 0u;

    if (s_isInit)
    {
    	resetCodePtr = &(((sNvmStorage_t *) s_nvmData)->resetCode);

    	if (resetCodePtr != NULL)
    	{
    	    retVal = *resetCodePtr;
    	}
    }

    return retVal;
}

/********************************************************************/
/** @brief Set information related to reset code that identifies the cause of the reset
 *
 * @param[in] code - New reset code, possible values are defined in #eResetCode_t
 *
 * @retval None
 * */
/********************************************************************/
void NVM_SetResetCode(uint8_t code)
{
    if (s_isInit)
    {
        uint8_t *resetCodePtr = NULL;
        resetCodePtr = getResetCode();

        if (resetCodePtr != NULL)
        {
            *resetCodePtr = code;
        }
    }
}

/********************************************************************/
/** @brief Store the actual configuration information inside the
 * two configuration areas of the memory
 * @return @ true if the configuration are store correctly in both areas,
 * @c false otherwise
 *
 * */
/********************************************************************/
bool NVM_SaveStorage(void)
{
    bool rc = true;

    /* compute NVM crc */
    ((sNvmStorage_t *) s_nvmData)->u16_crc = crcFast((uint8_t *) s_nvmData, offsetof(sNvmStorage_t, u16_crc));

    /* Erase Image 1 */
    if (rc)
    {
        rc = MemOps_EraseFlash(CONFIG1_BASE_PAGE, CONFIG1_END_PAGE);
    }

    /* Write Image 1 */
    if (rc)
    {
        rc = MemOps_WriteFlash((uint32_t) s_nvmData, (uint32_t)CONFIG1_BASE_ADDRESS, sizeof(s_nvmData));
    }

    /* Check Image 1 */
    if (rc)
    {
        rc = (memcmp(s_nvmData, (void *) CONFIG1_BASE_ADDRESS, sizeof(s_nvmData)) == 0)?true:false;
    }

    /* Erase Image 2 */
    if (rc)
    {
        rc = MemOps_EraseFlash(CONFIG2_BASE_PAGE, CONFIG2_END_PAGE);
    }

    /* Write Image 2 */
    if (rc)
    {
        rc = MemOps_WriteFlash((uint32_t) s_nvmData, (uint32_t)CONFIG2_BASE_ADDRESS, sizeof(s_nvmData));
    }

    /* Check Image 2 */
    if (rc)
    {
        rc = (memcmp(s_nvmData, (void *) CONFIG2_BASE_ADDRESS, sizeof(s_nvmData)) == 0)?true:false;
    }

    return rc;
}

/********************************************************************/
/** @brief Store in memory the default configuration
 * @return @ true on success, @c false otherwise
 * */
/********************************************************************/
bool NVM_RestoreDefaults(void)
{
	static const sDataIdentifiers_t s_defaultDid = {0};
	bool rc = false;

	sConfigParameters_t*	st_Config 	= getConfig();
	sDataIdentifiers_t*		st_DID 		= getDID();
	sDiagParameters_t*		st_Diag		= getDiag();
    uint8_t *resetCodePtr = getResetCode();
    uint8_t *wdgCodePtr = getWdgCodePtr();

	if (	(st_Config != NULL ) && (st_DID != NULL) && (st_Diag != NULL)	)
	{
		memcpy(st_Config, &s_defaultConfig, sizeof(s_defaultConfig));
		memcpy(st_DID, &s_defaultDid, sizeof(s_defaultDid));
		memcpy(st_Diag, &s_defaultDiagParams, sizeof(s_defaultDiagParams));
		memcpy(getBattData(), &s_defaultBattData, sizeof(s_defaultBattData));

		*resetCodePtr = (uint8_t)R_POWER_FAIL;
		*wdgCodePtr = 0x0;

		rc = NVM_SaveStorage();
	}
	else
	{
		rc = false;
	}

    return rc;
}

/********************************************************************/
/** @brief Store in memory a new configuration
 *
 *  @param[in] module - module identifier saved in FLASH memory
 * 	@param[in] code - reset code saved in FLASH memory
 * 	@retval None
 * */
/********************************************************************/
void NVM_SaveDbgErr(uint8_t module, uint8_t code)
{
    if (s_isInit)
    {
        osThreadId_t current;

        /* store module ID*/
    	((sNvmStorage_t *) s_nvmData)->dbgModuleID = module;
    	/* store erro ID*/
    	((sNvmStorage_t *) s_nvmData)->dbgErroID = code;

    	/* store thread info */
    	current = osThreadGetId();

    	if (current != NULL)
    	{
    	    /* store thread name to EEPROM */
    	    strncpy((char*) ((sNvmStorage_t *) s_nvmData)->dbgThreadName, osThreadGetName(current), 5 );
    	    ((sNvmStorage_t *) s_nvmData)->dbgThreadName[5] = '\0';
    	}

    }

    (void) NVM_SaveStorage();
}

/********************************************************************/
/** @brief Get the error Module identifier
 *
 * 	@return debug module ID saved in FLASH memory
 * */
/********************************************************************/
uint8_t NVM_GetErr_ModId(void)
{
    return ((sNvmStorage_t *) s_nvmData)->dbgModuleID;
}

/********************************************************************/
/** @brief Get the error identifier
 *
 * 	@return debug error ID saved in FLASH memory
 * */
/********************************************************************/
uint8_t NVM_GetErr_Code(void)
{
    return ((sNvmStorage_t *) s_nvmData)->dbgErroID;
}

/********************************************************************/
/** @brief Get the error thread Name
 *
 * 	@return debug thread name saved in FLASH memory
 * */
/********************************************************************/
uint8_t * NVM_GetErr_ThreadName(void)
{
    return ((sNvmStorage_t *) s_nvmData)->dbgThreadName;
}

/********************************************************************/
/** @brief Clear error Module identifier, error identifier
 * and error thread name
 *
 * 	@retval None
 * */
/********************************************************************/
void NVM_ClearErrorCodes(void)
{
    /* store module ID*/
    ((sNvmStorage_t *) s_nvmData)->dbgModuleID = 0u;
    /* store erro ID*/
    ((sNvmStorage_t *) s_nvmData)->dbgErroID = 0u;
    ((sNvmStorage_t *) s_nvmData)->dbgThreadName[0] = 0;
}

/********************************************************************/
/** @brief Get wdgForceDisable field of #sNvmStorage_t
 *
 * 	@return wdgForceDisable value or 0 if NVM is not initialized
 * */
/********************************************************************/
uint8_t NVM_ForcedWdgFlag_Get(void)
{
    uint8_t val = 0;
    uint8_t *wdgCodePtr = getWdgCodePtr();

    if (s_isInit)
    {
        val = *wdgCodePtr;
    }

    return val;

}

/********************************************************************/
/** @brief Set wdgForceDisable field of #sNvmStorage_t if NVM is initialized
 *
 * 	@retval None
 * */
/********************************************************************/
void NVM_ForcedWdgFlag_Set(void)
{
    uint8_t *wdgCodePtr = getWdgCodePtr();

    if (s_isInit)
    {
        if (wdgCodePtr != NULL)
        {
            *wdgCodePtr  = 0x1; /* Set flag */
        }
    }
}

/********************************************************************/
/** @brief Clear wdgForceDisable field of #sNvmStorage_t if NVM is initialized
 *
 * 	@retval None
 * */
/********************************************************************/
void NVM_ForcedWdgFlag_Clear(void)
{
    uint8_t *wdgCodePtr = getWdgCodePtr();
    if (s_isInit)
    {
        if (wdgCodePtr != NULL)
        {
            *wdgCodePtr  = 0x0; /* Clear flag */
        }
    }
}


/*************************************************************************************************************/
/**
 * @}
 **/
/*************************************************************************************************************/
