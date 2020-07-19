/*
 * Protocol_types.h
 *
 *  Created on: Sep 25, 2019
 *      Eng. Douglas Reis
 */

#ifndef SRC_DVP_TYPES_H_
#define SRC_DVP_TYPES_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <Frame.h>
#include <DVP.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#endif
#endif


struct DVP_PacketHandler
{
	DVP_Callback handler;
	void * arg;
};
/*!
 * @internal
 * @private
 */
typedef struct
{
	TP_Obj * tp;
	struct DVP_PacketHandler handlerTable[DVP_FrameCount];
	DVP_Frame * frame;
	uint32_t payloadSize;
	uint8_t transferSeq;
	uint8_t address;
	uint8_t * workBuffer;
	uint16_t  workBufferLen;
	bool waitingResponse;
}DVP_Context;


#define DVP_SET_FRAME_HEADER(f, t, i, s) \
	f->type = t;\
	f->id = i;\
	f->statusCode = s;

#define DVP_FRAME_HEADER_SIZE (uint32_t)(&(((DVP_Frame *)0)->payload))
#define DVP_MAX_PAYLOAD_LEN (uint32_t)300

#ifdef __LITTLE_ENDIAN__



#else


#endif

#endif /* SRC_DVP_TYPES_H_ */
