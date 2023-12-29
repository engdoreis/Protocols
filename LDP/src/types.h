/*
 * Protocol_types.h
 *
 *  Created on: Sep 25, 2019
 *      Eng. Douglas Reis
 */

#ifndef SRC_LDP_TYPES_H_
#define SRC_LDP_TYPES_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <Frame.h>
#include <LDP.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#endif
#endif


struct LDP_PacketHandler
{
	LDP_Callback handler;
	void * arg;
};
/*!
 * @internal
 * @private
 */
typedef struct
{
	TP_Obj * tp;
	struct LDP_PacketHandler handlerTable[LDP_FrameCount];
	LDP_Frame * frame;
	uint32_t payloadSize;
	uint8_t transferSeq;
	uint8_t address;
	uint8_t * workBuffer;
	uint16_t  workBufferLen;
	bool waitingResponse;
}LDP_Context;


#define LDP_SET_FRAME_HEADER(f, t, i, s) \
	f->type = t;\
	f->id = i;\
	f->statusCode = s;

#define LDP_FRAME_HEADER_SIZE (uintptr_t)(&(((LDP_Frame *)0)->payload))
#define LDP_MAX_PAYLOAD_LEN (uint32_t)128

#ifdef __LITTLE_ENDIAN__



#else


#endif

#endif /* SRC_LDP_TYPES_H_ */
