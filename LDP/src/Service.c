/*
 ============================================================================
 Name        : LDP_NinebotServer.c
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
#include "LDP.h"

void TP_Callback(void *param, uint8_t address, uint16_t size, uint8_t *payload)
{
	LDP_Context * ctx = param;
	ctx->frame = (LDP_Frame *) payload;
	ctx->payloadSize = size - LDP_FRAME_HEADER_SIZE;

	if(!ctx->waitingResponse || ctx->frame->type == LDP_FrameEvent)
	{
		if(ctx->frame->type < LDP_FrameCount)
		{
			if(ctx->handlerTable[ctx->frame->type].handler)
			{
				ctx->handlerTable[ctx->frame->type].handler(ctx->handlerTable[ctx->frame->type].arg, address, ctx->frame);
			}
		}
	}

	ctx->waitingResponse = false;
}

bool LDP_Init(LDP_Obj *obj, const void *port, LDP_Driver *driver, uint8_t * buffer, uint32_t size)
{
	TP_ASSERT(obj == NULL || port == NULL || buffer == NULL);

	uint32_t offset = 0;
	obj->handle = buffer;
	LDP_Context * ctx = obj->handle;
	offset += sizeof(LDP_Context);
	TP_ASSERT(offset >= size);

	ctx->workBufferLen = LDP_FRAME_HEADER_SIZE + LDP_MAX_PAYLOAD_LEN;
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

bool LDP_RegisterEventCallback(LDP_Obj *obj, LDP_Callback callback, void *arg)
{
	bool ret = false;
	LDP_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[LDP_FrameEvent].handler = callback;
		ctx->handlerTable[LDP_FrameEvent].arg = arg;
		ret = true;
	}
	return ret;
}

bool LDP_RegisterCommandCallback(LDP_Obj *obj, LDP_Callback callback, void *arg)
{
	bool ret = false;
	LDP_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[LDP_FrameCommand].handler = callback;
		ctx->handlerTable[LDP_FrameCommand].arg = arg;
		ret = true;
	}
	return ret;
}

bool LDP_RegisterResponseCallback(LDP_Obj *obj, LDP_Callback callback, void *arg)
{
	bool ret = false;
	LDP_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[LDP_FrameResponse].handler = callback;
		ctx->handlerTable[LDP_FrameResponse].arg = arg;
		ret = true;
	}
	return ret;
}

bool LDP_Run (LDP_Obj *obj)
{
	LDP_Context * ctx = obj->handle;
	TP_Process(ctx->tp);
	return true;
}

static LDP_StatusCode LDP_SendGeneric(LDP_Obj *obj, bool sync, uint32_t type, uint32_t id, uint32_t statusCode, void * payload, uint32_t size, void * resp, uint32_t *respSize )
{
	LDP_StatusCode result = LDP_ParameterError;
	LDP_Context * ctx = obj->handle;

	uint32_t frameSize = size + LDP_FRAME_HEADER_SIZE;

	if(obj && ctx && frameSize <= ctx->workBufferLen)
	{
		LDP_Frame * frame = (LDP_Frame *) ctx->workBuffer;
		LDP_SET_FRAME_HEADER(frame, type, id, statusCode);
		memcpy(frame->payload.raw, payload, size);
		result = LDP_ProtocolError;

		if(TP_Send(ctx->tp, ctx->address, (uint8_t *)frame, frameSize) == false)
		{
			goto exit;
		}

		if(sync)
		{
			uint32_t tries = 5;
			ctx->frame = NULL;
			ctx->waitingResponse = true;
			do
			{
				TP_Process(ctx->tp);
				if(!ctx->waitingResponse && ctx->frame && ctx->frame->type == LDP_FrameResponse && ctx->frame->id == id)
				{
					result = (LDP_StatusCode)ctx->frame->statusCode;

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

LDP_StatusCode LDP_Command1(LDP_Obj *obj, st_cmd1* data, st_cmd1* out)
{
	uint32_t size = (out==NULL)?0:sizeof(st_cmd1);
	return LDP_SendGeneric(obj, true, LDP_FrameCommand, LDP_Cmd1, LDP_OK, data, (data==NULL)?0:sizeof(st_cmd1), out, &size);
}

LDP_StatusCode LDP_Command1Async(LDP_Obj *obj, st_cmd1* data)
{
	return LDP_SendGeneric(obj, false, LDP_FrameCommand, LDP_Cmd1, LDP_OK, data, (data==NULL)?0:sizeof(st_cmd1), NULL, 0);
}


LDP_StatusCode LDP_Command2(LDP_Obj *obj, st_cmd2 *cmd)
{
	uint32_t size = sizeof(st_cmd2);
	return LDP_SendGeneric(obj, true, LDP_FrameCommand, LDP_Cmd2, LDP_OK, NULL, 0, cmd, &size);
}

LDP_StatusCode LDP_Response1(LDP_Obj *obj, LDP_StatusCode statusCode, st_cmd1* data)
{
	return LDP_SendGeneric(obj, false, LDP_FrameResponse, LDP_Cmd1, statusCode, data, (data==NULL)?0:sizeof(st_cmd1), NULL, 0);
}

LDP_StatusCode LDP_Response2(LDP_Obj *obj, LDP_StatusCode statusCode, st_cmd2* data)
{
	return LDP_SendGeneric(obj, false, LDP_FrameResponse, LDP_Cmd2, statusCode, data, (data==NULL)?0:sizeof(st_cmd2), NULL, 0);
}

LDP_StatusCode LDP_Event1(LDP_Obj *obj, st_cmd1* data)
{
	return LDP_SendGeneric(obj, false, LDP_FrameEvent, LDP_Evt1, LDP_OK, data, (data==NULL)?0:sizeof(st_cmd1), NULL, 0);
}

LDP_StatusCode LDP_Event2(LDP_Obj *obj, st_cmd2* data)
{
	return LDP_SendGeneric(obj, false, LDP_FrameEvent, LDP_Evt2, LDP_OK, data, (data==NULL)?0:sizeof(st_cmd2), NULL, 0);
}


