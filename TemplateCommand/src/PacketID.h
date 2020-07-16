/*
 * T_CommandID.h
 *
 *  Created on: 5 de abr de 2019
 *      Eng. Douglas Reis
 */

#ifndef SRC_T_FRAMEID_H_
#define SRC_T_FRAMEID_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef __SHORT_ENUM__
	#define __SHORT_ENUM__ __attribute__ ((__packed__))
#endif

/*!
 * @defgroup PacketID Packet ID
 */

/**
 * |Size     |Name           |Description                                          |
 * |:--:     |:--:           |:--                                                  |
 * |1        |field1         |This field is used for nothing                       |
 * |1        |field2         |This field is used for something                    |
 */
typedef struct
{
	uint8_t field1;
	uint8_t field2;
} st_cmd1;

/**
 * |Size     |Name           |Description                                          |
 * |:--:     |:--:           |:--                                                  |
 * |1        |field1         |This field is used for nothing                       |
 * |1        |field2         |This field is used for something                    |
 * |2        |field3         |This field is may be used for something             |
 */
typedef struct 
{	uint8_t field1;
	uint8_t field2;
	uint16_t field3;
}st_cmd2;

typedef enum __SHORT_ENUM__
{
	/*!
	 * @addtogroup Commands Command/Response ID list
	 * @ingroup PacketID
	 * @{
	 * @par 0xC0 - Command1
	 * @copybrief T_Command1
	 * * Command payload
	 *   @copydoc st_cmd1
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	T_Cmd1 = 0xC0,

	/*!
	 * @addtogroup Commands
	 * @ingroup PacketID
	 * @{
	 * @par 0xC1 - Command2
	 * @copybrief T_Command2
	 * * Command payload
	 *   @copydoc st_cmd2
	 * 
	 * * Status code See @ref StatusCode
	 * @}
	 */
	T_Cmd2 = 0xC1,

	/*--------------------------Events-----------------------------------*/
	/*!
	 * @addtogroup Events Event ID List
	 * @ingroup PacketID
	 * @{
	 * @par 0xE0 - Event1
	 * @copybrief T_Event1
	 * * Command payload
	 *   @copydoc st_cmd1
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	T_Evt1 = 0xE0,

	/*!
	 * @addtogroup Events
	 * @ingroup PacketID
	 * @{
	 * @par 0xE1 - Event 2
	 * @copybrief T_Event2
	 * * Command payload
	 *   @copydoc st_cmd2
	 *
	 * * Status code See @ref StatusCode
	 * @}
	 */
	T_Evt2 = 0xE1,
}T_FrameID;

#define BTP_ArrayToInt32(BTPArray)              (uint32_t)((BTPArray[0] << 24) | (BTPArray[1] << 16) | (BTPArray[2] << 8) | (BTPArray[3] & 0xFF))
#define BTP_Int32ToArray(Int32, BTPArray)       BTPArray[0] = (Int32 >> 24) & 0xFF; BTPArray[1] = (Int32 >> 16) & 0xFF;BTPArray[2] = (Int32 >> 8) & 0xFF;BTPArray[3] = (Int32 >> 0) & 0xFF;
#define BTP_ArrayToInt16(BTPArray)              (uint16_t) ((BTPArray[0] << 8) | (BTPArray[1] & 0xFF))
#define BTP_Int16ToArray(Int16, BTPArray)       BTPArray[0] = (Int16 >> 8) & 0xFF;BTPArray[1] = (Int16 >> 0) & 0xFF;
	
#endif /* SRC_T_FRAMEID_H_ */
