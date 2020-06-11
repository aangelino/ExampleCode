/**
 @file NVM.h
 @SWVersion @SW_VERSION

 @brief    	 NVM Module
 @details    Declaration required by the Non Volatile Memory implementation.
 Project ATB
 Platform STM32L451xx

 (c) Copyright 2017 Vodafone Automotive - All Rights Reserved.
 */
#ifndef APP_NVM_NVM_H_
#define APP_NVM_NVM_H_

#include <stdint.h>
#include <stdbool.h>

#define DID_PARTNUMBER_MAXLEN               11u
#define DID_SWVERSIONNUMBER_MAXLEN          4u
#define DID_HWVERSIONNUMBER_MAXLEN          3u
#define DID_SERIALNUMBER_MAXLEN             14u

#define CONFIG_SMSCALLNUMBER_MAXLEN         50u
#define CONFIG_NUM_SMSCALLNUMBER            5u
#define CONFIG_NETWORKAPN_MAXLEN            32u
#define CONFIG_NETWORK_PASSWORD_MAXLEN      16u
#define CONFIG_NETWORK_LOGIN_MAXLEN         16u
#define CONFIG_NET_GATEWAYURL_MAXLEN   		50u

/* LIMITS */
#define CONFIG_RF_TRX_PWR_RANGE_MIN     			0u
#define CONFIG_RF_TRX_CHANNEL_RANGE_MIN    			0u
#define CONFIG_RF_TRANSM_CH_RANGE_MIN    			1u
#define CONFIG_RF_DUTY_ON_RANGE_MIN                 100u
#define CONFIG_RF_DUTY_OFF_RANGE_MIN                100u
#define CONFIG_WAKEUP_PERIOD_RANGE_MIN              1u /*  60 default */
#define CONFIG_WAKEUP_TIME_RANGE_MIN                60u /* (seconds) */
#define CONFIG_WAKEUP_COUNT_RANGE_MIN               0u
#define CONFIG_COMM_TIMEOUT_RANGE_MIN      			60u
#define CONFIG_TRACK_INTERVAL_RANGE_MIN          	0
#define CONFIG_TRACKING_TIME_RANGE_MIN              0
#define CONFIG_NET_TIMEOUT_RANGE_MIN           		0
#define CONFIG_GPRS_GATEWAY_PORT_MIN                0

#define CONFIG_RF_TRANSM_PWR_RANGE_MAX      		100u
#define CONFIG_RF_TRANSM_CH_RANGE_MAX    			68u
#define CONFIG_RF_DUTY_ON_RANGE_MAX                 65535u
#define CONFIG_RF_DUTY_OFF_RANGE_MAX                65535u
#define CONFIG_WAKEUP_PERIOD_RANGE_MAX              65535u
#define CONFIG_WAKEUP_TIME_RANGE_MAX                65535u
#define CONFIG_WAKEUP_COUNT_RANGE_MAX               255u
#define CONFIG_COMM_TIMEOUT_RANGE_MAX               65535u
#define CONFIG_TRACK_INTERVAL_RANGE_MAX             65535u
#define CONFIG_TRACKING_TIME_RANGE_MAX              65535u
#define CONFIG_NET_TIMEOUT_RANGE_MAX                65535u
#define CONFIG_GPRS_GATEWAY_PORT_MAX                65535u

#define DIAG_SERIALNUMBER_MAXLEN                    12u
#define DIAG_SOFTWAREVERSION_MAXLEN                 4u
#define DIAG_BOARDREVISION_MAXLEN                   2u

/** @brief Configuration parameters */
typedef struct
{
    uint8_t rfTransmissionPower;														/**< Transmission power */
    uint8_t rfTransmissionChannel;                                                      /**< Transmission channel */
    uint16_t rfDutyOn;                                                                  /**<  */
    uint16_t rfDutyOff;                                                                 /**<  */
    uint16_t wakeupPeriod;                                                              /**< Wakeuo period */
    uint16_t wakeupTime;                                                                /**< Wakeup time */
    uint8_t wakeupCount;                                                                /**<  */
    uint16_t communicationTimeout;                                                      /**< Communication timeout */
    char smsCallNumber[CONFIG_NUM_SMSCALLNUMBER][CONFIG_SMSCALLNUMBER_MAXLEN + 1u];     /**<  */
    char networkApn[CONFIG_NETWORKAPN_MAXLEN + 1u];                                      /**< APN */
    char networkPassword[CONFIG_NETWORK_PASSWORD_MAXLEN + 1u];                           /**< Network password */
    char networkLogin[CONFIG_NETWORK_LOGIN_MAXLEN + 1u];                                 /**< Network login */
    uint16_t networkTimeout;                                                            /**< Network timeout */
    char gprsGatewayUrl[CONFIG_NET_GATEWAYURL_MAXLEN + 1u];                              /**< URL of GPRS gateway */
    uint16_t gprsGatewayPort;                                                           /**< Port of GPRS gateway */
    uint8_t transmissionPolicy;                                                         /**< Transmission policy see #eTransmissionChannelPolicy_t */
} sConfigParameters_t;

/** @brief Diagnostic parameters */
typedef struct
{
    uint8_t serialNumber[DIAG_SERIALNUMBER_MAXLEN + 1u];                                 /**< Serial number  */
    uint8_t softwareVersion[DIAG_SOFTWAREVERSION_MAXLEN + 1u];                           /**< Software version */
    uint8_t boardRevision[DIAG_BOARDREVISION_MAXLEN + 1u];                               /**< HW Revision */
} sDiagParameters_t;

/** @brief Identifier parameters */
typedef struct
{
    char partNumber[DID_PARTNUMBER_MAXLEN + 1u];                                         /**< Part number */
    char appSwVersionNumber[DID_SWVERSIONNUMBER_MAXLEN + 1u];                            /**< Application software version */
    char hwVersionNumber[DID_HWVERSIONNUMBER_MAXLEN + 1u];                               /**< Hardware version */
    char serialNumberDID[DID_SERIALNUMBER_MAXLEN + 1u];                                  /**< Serial number */
} sDataIdentifiers_t;

/** @brief Battery parameters */
typedef struct
{
    uint32_t running;                                                                   /**<  */
    uint32_t lowPower;                                                                  /**<  */
} sBattData_t;


bool NVM_Init(void);
bool NVM_SaveStorage(void);
bool NVM_RestoreDefaults(void);

sConfigParameters_t *NVM_GetConfig(void);
sDataIdentifiers_t *NVM_GetDID(void);
sDiagParameters_t *NVM_GetDiagParam(void);
sBattData_t *NVM_GetBattData(void);

extern uint8_t NVM_GetResetCode(void);
extern void NVM_SetResetCode(uint8_t code);
extern void NVM_SaveDbgErr(uint8_t module, uint8_t code);

extern uint8_t NVM_GetErr_ModId(void);
extern uint8_t NVM_GetErr_Code(void);
extern uint8_t * NVM_GetErr_ThreadName(void);
extern void NVM_ClearErrorCodes(void);
extern uint8_t NVM_ForcedWdgFlag_Get(void);
extern void NVM_ForcedWdgFlag_Set(void);
extern void NVM_ForcedWdgFlag_Clear(void);


#endif /* APP_NVM_NVM_H_ */
