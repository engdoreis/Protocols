/*
 ============================================================================
 Name        : DVP_ProtocolServer.c
 Author      : Douglas Reis
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <TransportProtocol.h>

#include "PacketID.h"

#include "types.h"
#include "DVP.h"

void TP_Callback(void *param, uint8_t address, uint16_t size, uint8_t *payload)
{
	DVP_Context * ctx = param;
	ctx->frame = (DVP_Frame *) payload;
	ctx->payloadSize = size - DVP_FRAME_HEADER_SIZE;

	if(!ctx->waitingResponse || ctx->frame->type == DVP_FrameEvent)
	{
		if(ctx->frame->type < DVP_FrameCount)
		{
			if(ctx->handlerTable[ctx->frame->type].handler)
			{
				ctx->handlerTable[ctx->frame->type].handler(ctx->handlerTable[ctx->frame->type].arg, address, ctx->frame);
			}
		}
	}

	ctx->waitingResponse = false;
}

bool DVP_Init(DVP_Obj *obj, const void *port, DVP_Driver *driver, uint8_t * buffer, uint32_t size)
{
	TP_ASSERT(obj == NULL || port == NULL || buffer == NULL);

	uint32_t offset = 0;
	obj->handle = buffer;
	DVP_Context * ctx = obj->handle;
	offset += sizeof(DVP_Context);
	TP_ASSERT(offset >= size);

	ctx->workBufferLen = DVP_FRAME_HEADER_SIZE + DVP_MAX_PAYLOAD_LEN;
	ctx->workBuffer = buffer + offset;
	offset += ctx->workBufferLen;
	TP_ASSERT(offset >= size);

	ctx->tp = (TP_Obj * )buffer + offset;
	offset += sizeof(TP_Obj);
	TP_ASSERT(offset >= size);

	memset(&ctx->handlerTable, 0, sizeof(ctx->handlerTable));

	bool ret = TP_Init(ctx->tp, (TP_Driver *)driver, TP_Callback, ctx, port, 2000, buffer + offset, size - offset);
	if(ret)
	{
		ctx->address = 0;
	}

	end:
	return ret;
}

bool DVP_RegisterEventCallback(DVP_Obj *obj, DVP_Callback callback, void *arg)
{
	bool ret = false;
	DVP_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[DVP_FrameEvent].handler = callback;
		ctx->handlerTable[DVP_FrameEvent].arg = arg;
		ret = true;
	}
	return ret;
}

bool DVP_RegisterCommandCallback(DVP_Obj *obj, DVP_Callback callback, void *arg)
{
	bool ret = false;
	DVP_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[DVP_FrameCommand].handler = callback;
		ctx->handlerTable[DVP_FrameCommand].arg = arg;
		ret = true;
	}
	return ret;
}

bool DVP_RegisterResponseCallback(DVP_Obj *obj, DVP_Callback callback, void *arg)
{
	bool ret = false;
	DVP_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[DVP_FrameResponse].handler = callback;
		ctx->handlerTable[DVP_FrameResponse].arg = arg;
		ret = true;
	}
	return ret;
}

bool DVP_Run (DVP_Obj *obj)
{
	DVP_Context * ctx = obj->handle;
	TP_Process(ctx->tp);
	return true;
}

DVP_StatusCode DVP_SendGeneric(DVP_Obj *obj, bool sync, uint32_t type, uint32_t id, uint32_t statusCode, void * payload, uint32_t size, void * resp, uint32_t *respSize )
{
	DVP_StatusCode result = DVP_ParameterError;
	DVP_Context * ctx = obj->handle;

	uint32_t frameSize = size + DVP_FRAME_HEADER_SIZE;

	if(obj && ctx && frameSize <= ctx->workBufferLen)
	{
		DVP_Frame * frame = (DVP_Frame *) ctx->workBuffer;
		DVP_SET_FRAME_HEADER(frame, type, id, statusCode);
		memcpy(frame->payload.raw, payload, size);
		result = DVP_ProtocolError;

		if(TP_Send(ctx->tp, ctx->address, (uint8_t *)frame, frameSize) == false)
		{
			goto exit;
		}

		result = DVP_OK;
		if(sync)
		{
			uint32_t tries = 5;
			ctx->frame = NULL;
			ctx->waitingResponse = true;
			do
			{
				TP_Process(ctx->tp);
				if(!ctx->waitingResponse && ctx->frame && ctx->frame->type == DVP_FrameResponse && ctx->frame->id == id)
				{
					result = (DVP_StatusCode)ctx->frame->statusCode;

					if(resp && (*respSize >= ctx->payloadSize))
					{
						memset(resp, 0, *respSize);
						*respSize = ctx->payloadSize;
						memcpy(resp, ctx->frame->payload.raw, *respSize);
					}
					break;
				}
				tries--;
			}while(tries);
		}
	}

	exit:
	return result;
}
