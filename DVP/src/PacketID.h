/*
 * DVP_CommandID.h
 *
 *  Created on: 5 de abr de 2019
 *      Eng. Douglas Reis
 */

#ifndef SRC_DVP_FRAMEID_H_
#define SRC_DVP_FRAMEID_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef __SHORT_ENUM__
	#define __SHORT_ENUM__ __attribute__ ((__packed__))
#endif
#pragma pack(push,1)
/*!
 * @defgroup PacketID Packet ID
 */

/**
 * |Size     |Name            |Read/Write|Description                                                                                  |
 * |:--:     |:--:            |:--:      |:--                                                                                          |
 * |1        |Wheel Lock      |Read/Write|True wheel is locked, False wheel is free.                                                   |
 * |1        |Powered On      |Read/Write|True vehicle enabled and display is on, False vehicle is disabled and display is off .        |
 * |1        |Head Light On   |Read/Write|True head light is on, False head light is off .                                          |
 * |1        |Tail Light On   |Read/Write|True tail light is on, False tail light is off .                                          |
 * |1        |Buzzer On       |Read/Write|True buzzer is beeping, False buzzer is off .                                             |
 * |1        |Cruise On       |Read/Write|True cruise is activated, False cruise is deactivated .                                   |
 * |2        |Speed           |Read      |Indicates the vehicle speed in km/h. The value must be divided by 100. Ex. 2515 => 25,15 km/h |
 */
typedef struct
{
	bool wheelLock;          /*!< True wheel is locked, False wheel is free.                                              */
	bool poweredOn;          /*!< True vehicle enabled and display is on, False vehicle is disabled and display is off .   */
	bool headLightOn;        /*!< True head light is on, False head light is off .                                     */
	bool tailLightOn;        /*!< True tail light is on, False tail light is off .                                     */
	bool buzzerOn;           /*!< True buzzer is beeping, False buzzer is is off .                                        */
	bool cruiseOn;           /*!< True cruise is activated, False cruise is deactivated.                                    */
	uint16_t speed;          /*!< Indicates the vehicle speed in km/h. The value must be divided by 100. Ex. 2515 => 25,15 km/h*/
} DVP_VehicleStatus;

/**
 * |Size     |Name            |Read/Write|Description                                                                                   |
 * |:--:     |:--:            |:--:      |:--                                                                                           |
 * |1        |Cruise Enabled  |Read/Write|True the wheel will activate the cruise when the conditions are met, False Cruise is disabled |
 * |1        |Throttle Level  |Read/Write|Indicates the acceleration level from 1 to 10.                                                |
 * |1        |Brake Level     |Read/Write|Indicates the braking level from 1 to 10.                                                     |
 * |2        |Start speed     |Read/Write|Indicates the speed required to the controller energize the motor. Under this value the throttle doesn't work.The value must be divided by 100. Ex. 0300 => 3 km/h. The default value is 3 km/h|
 * |2        |Speed Limit     |Read/Write|Indicates the speed limit for the vehicle no metter whether it's going down in a hill or not. The value must be divided by 100. Ex. 2200 => 22 km/h. The default value is 25 km/h|
 */
typedef struct 
{	
	bool cruiseEnabled;
	uint8_t throttleLevel;
	uint8_t brakeLevel;
	uint16_t startSpeed;	
	uint16_t speedLimit;	
}DVP_VehicleConfig;

/**
 * |Size     |Name            |Read/Write|Description                                                                          |
 * |:--:     |:--:            |:--:      |:--                                                                                  |
 * |1        |Serial Number   |Read      |Indicates the controller serial number. If the serial number has less then 20 bytes, a head padding must be used filled with 0x00.|
 * |1        |Firmware Version|Read      |Indicates the controller firmware version. Four bytes long left aligned.Ex. 0x01025600 => 01.02.56.00.|
 */
typedef struct 
{
	uint8_t serialnumber[20];
	uint8_t firmwareVersion[4];	
}DVP_Info;

/**
 * |Size        |Name            |Read/Write|Description                                                                                   |
 * |:--:        |:--:            |:--:      |:--                                                                                           |
 * |1           |Is Charging     |Read      |True the battery is charging, False the battery is discharging.                               |
 * |1           |Charge          |Read      |Indicates the battery charge from 0% to 100%.                                                 |
 * |1           |Temperature     |Read      |Indicates the battery temperature offset from -20�C. Ex. 45 means 25�C.                      |
 * |2           |Voltage         |Read      |Indicates the battery voltage. The value must be divided by 100. Ex. 2515 => 25,15V           |
 */
typedef struct
{
	bool isCharging;           /*!<True the battery is charging, False the battery is discharging */	
	uint8_t charge;            /*!<Indicates the battery charge from 0% to 100%.*/
	uint8_t temperature;       /*!<Indicates the battery temperature offset from -20�C. Ex.  45 means 25�C.*/
	uint16_t voltage;          /*!<Indicates the battery voltage. The value must be divided by 100. Ex. 2515 => 25,15V */	
} DVP_BatteryStatus;

/*!
 * |Size     |Name            |Read/Write|Description                                                                          |
 * |:--:     |:--:            |:--:      |:--                                                                                  |
 * |4        |Firmware size   |Write     |Indicates the total size of the binary that will be sent by the next commands.       | 
 */
typedef struct 
{	
	uint32_t firmwareSize;  /*!<Indicates the total size of the binary that will be sent by the next commands.*/
}DVP_FirmwareUpdateStartPacket;

/*!
 * |Size     |Name       |Read/Write|Description                                                       |
 * |:--:     |:--:       |:--:      |:--                                                               |
 * |2        |Sequence   |Write     |Indicates the incremental sequence number, starting from 0.       | 
 * |2        |size       |Write     |Indicates the size in bytes of the packet content.                | 
 * |var      |Content    |Write     |Carries one binary packet.                                        | 
 */
typedef struct 
{	
	uint16_t sequence;      /*!<Indicates the incremental sequence number, starting from 0.*/
	uint16_t size;          /*!<Indicates the size in bytes of the packet content.        */
	uint8_t content[256];   /*!<Carries one binary packet.*/
}DVP_FirmwareUpdateLoadPacket;

/*!
 * |Size     |Name            |Read/Write|Description                                                                          |
 * |:--:     |:--:            |:--:      |:--                                                                                  |
 * |4        |crc             |Write     |Indicates the CRC32 IEEE for the whole binary transfered. | 
 * |4        |version         |Write     |Indicates the transfered firmware version. Four bytes long right aligned.Ex. 0x01025600 => 01.02.56.00. | 
 */
typedef struct 
{	
	uint8_t crc[4];       /*!<Indicates the CRC32 IEEE for the whole binary transfered.*/
	uint8_t version[4];   /*!<Indicates the transfered firmware version. Four bytes long left aligned.Ex. 0x01025600 => 01.02.56.00.*/
}DVP_FirmwareUpdateFinishPacket;

/*!
 * |Size     |Name       |Read/Write|Description                                                       |
 * |:--:     |:--:       |:--:      |:--                                                               |
 * |1        |size       |Read/Write|Indicates the size in bytes of the packet content.                |
 * |var      |AuthData   |Read/Write|Carries the authentication Data.                                  |
 */
typedef struct
{
	uint8_t size;            /*!<Indicates the size in bytes of the packet content.*/
	uint8_t AuthData[256];   /*!<Carries the authentication Data.*/
}DVP_AuthenticationData;

#pragma pack(pop)


typedef enum __SHORT_ENUM__
{
	/*!
	 * @addtogroup Commands Command/Response ID list
	 * @ingroup PacketID
	 * @{
	 * @par 0xC0 - Read vehicle status
	 * @copybrief DVP_ReadVehicleStatus
	 * * Command payload
	 *   > None
	 * * Response payload
	 *   @copydoc DVP_VehicleStatus
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eReadVehicleStatus = 0xC0,

	/*!
	 * @addtogroup Commands Command/Response ID list
	 * @ingroup PacketID
	 * @{
	 * @par 0xC1 - Write vehicle status
	 * @copybrief DVP_WriteVehicleStatus
	 * * Command payload
	 *   @copydoc DVP_VehicleStatus
	 * * Response payload
	 *   > None
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eWriteVehicleStatus,

	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xC2 - Read vehicle configuration.
	 * @copybrief DVP_ReadVehicleConfig
	 * * Command payload
	 *  > None
	 * * Response payload
	 *   @copydoc DVP_VehicleConfig
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eReadVehicleConfig,

	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xC3 - Write vehicle configuration.
	 * @copybrief DVP_WriteVehicleConfig
	 * * Command payload
	 *   @copydoc DVP_VehicleConfig
	 * * Response payload
	 *  > None
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eWriteVehicleConfig,

	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xC4 - Read vehicle information
	 * @copybrief DVP_ReadVehicleInfo
	 * * Command payload
	 * > None
	 * * Response payload
	 *   @copydoc DVP_Info
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eReadVehicleInfo,

	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xC5 - Read BatteryStatus
	 * @copybrief DVP_ReadBatteryStatus
	 * * Command payload
	 * > None
	 * * Response payload
	 *   @copydoc DVP_BatteryStatus
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eReadBatteryStatus,
	
	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xC6 - Read battery information
	 * @copybrief DVP_ReadBatteryInfo
	 * * Command payload
	 * > None
	 * * Response payload
	 *   @copydoc DVP_Info
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eReadBatteryInfo,
	
	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xCA - Firmware Update Start
	 * @copybrief DVP_FirmwareUpdateStart
	 * * Command payload
	 *   @copydoc DVP_FirmwareUpdateStartPacket
	 * * Response payload
	 * > None
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eFirmwareUpdateStart = 0xCA,
	
	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xCB - Firmware Update Transfer
	 * @copybrief DVP_FirmwareUpdateLoad
	 * * Command payload
	 *   @copydoc DVP_FirmwareUpdateLoadPacket
	 * * Response payload
	 * > None
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eFirmwareUpdateLoad = 0xCB,
	
	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xCC - Firmware Update Finish
	 * @copybrief DVP_FirmwareUpdateFinish
	 * * Command payload
	 *   @copydoc DVP_FirmwareUpdateFinishPacket
	 * * Response payload
	 * > None
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eFirmwareUpdateFinish = 0xCC,

	/*!
	 * @addtogroup Commands Command/Response ID list
	 * @ingroup PacketID
	 * @{
	 * @par 0xA0 - Start authentication
	 * @copybrief DVP_StartAuthentication
	 * * Command payload
	 *   > None
	 * * Response payload
	 *   @copydoc DVP_AuthenticationData
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eStartAuthentication = 0xA0,

	/*!
	 * @addtogroup Commands Command/Response ID list
	 * @ingroup PacketID
	 * @{
	 * @par 0xA1 - Authenticate
	 * @copybrief DVP_Authenticate
	 * * Command payload
	 *   @copydoc DVP_AuthenticationData
	 * * Response payload
	 *   > None
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eAuthenticate,

	/*!
	 * @addtogroup Commands Command/Response ID list
	 * @ingroup PacketID
	 * @{
	 * @par 0xA2 - Update Public Key
	 * @copybrief DVP_UpdatePublicKey
	 * * Command payload
	 *   @copydoc DVP_AuthenticationData
	 * * Response payload
	 *   > None
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eUpdatePublicKey,

	/*--------------------------Events-----------------------------------*/
	/*!
	 * @addtogroup Events Event ID List
	 * @ingroup PacketID
	 * @{
	 * @par 0xE0 - Keep Alive
	 * @copybrief DVP_AuthenticationData
	 * * Payload
	 *  > None
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	DVP_eKeepAlive = 0xE0,
	

}DVP_FrameID;

#define BTP_ArrayToInt32(BTPArray)              (uint32_t)((BTPArray[0] << 24) | (BTPArray[1] << 16) | (BTPArray[2] << 8) | (BTPArray[3] & 0xFF))
#define BTP_Int32ToArray(Int32, BTPArray)       BTPArray[0] = (Int32 >> 24) & 0xFF; BTPArray[1] = (Int32 >> 16) & 0xFF;BTPArray[2] = (Int32 >> 8) & 0xFF;BTPArray[3] = (Int32 >> 0) & 0xFF;
#define BTP_ArrayToInt16(BTPArray)              (uint16_t) ((BTPArray[0] << 8) | (BTPArray[1] & 0xFF))
#define BTP_Int16ToArray(Int16, BTPArray)       BTPArray[0] = (Int16 >> 8) & 0xFF;BTPArray[1] = (Int16 >> 0) & 0xFF;
	
#endif /* SRC_DVP_FRAMEID_H_ */
