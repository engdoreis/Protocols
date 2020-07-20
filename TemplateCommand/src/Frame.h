/*!
 * @file Frame.h
 *
 *  @date Jul 11, 2020
 *  @author: Douglas Reis
 */

#ifndef SRC_T_FRAME_H_
#define SRC_T_FRAME_H_

#include <PacketID.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <TransportProtocol.h>
#include <StatusCode.h>

/*!
 * @defgroup CommandProtocol Enlace Layer
 */

/*!
 * @addtogroup PacketFrame Packet Frame
 * @ingroup CommandProtocol
 * @{
 * @par Packet Frame
 * | Field         | Sise  | Offset  | Description                                                |
 * |:--:           |:--:   |:--:     |:--                                                         |
 * |**Packet Type**|1      |0        |Describes the @ref PacketType.                                   |
 * |**Packet ID**  |1      |2        |Identify the packet. The possible values are defined by @ref PacketID  |
 * |**Status Code**|1      |3        |Used to sinalize errors occurred during the execution of command. The possible values are defined by the command protocol |
 * |**Payload**    |var    |5        |The Payload will depend on the Packet ID. See @ref PacketID|
 * @}
 */

/**
 * @addtogroup PacketType Packet Type
 * @ingroup CommandProtocol
 * @{
 * @par Packet Type
 * | Type          | Value | Description                                                |
 * |:--:           |:--:   |:--                                                         |
 * |**COMMAND**    |0      |The master will send a COMMAND the slave requesting actions or informations. Any command will generate a response using RESPONSE packet.|
 * |**RESPONSE**   |1      |The slave will reply a COMMAND with a RESPONSE.                                                                        |
 * |**EVENT**      |2      |A EVENT can be send by master or slave and does not generate another Packet.                                            |
 *
 * @}
 */


/**
 * @enum Frame Type
 */
typedef enum __SHORT_ENUM__
{
	T_FrameCommand  = 0,  /*!< The master of the communication will send COMMANDs the slave requesting actions or informations.*/
	T_FrameResponse = 1,  /*!< The slave of the communication will reply to master COMMANDs with RESPONSE.                       */
	T_FrameEvent    = 2,  /*!< The slave of the communication can send EVENTs to the master.                                     */
	T_FrameCount
}T_PacketType;

#pragma pack(push,1)
typedef struct
{
	T_PacketType type;
	T_FrameID id;
	Template_StatusCode statusCode;
	union
	{
		uint8_t raw[1024];
		st_cmd1 cmd1;
		st_cmd2 cmd2;
	}payload;
} T_Frame;
#pragma pack(pop)

#endif //SRC_T_FRAME_H_




