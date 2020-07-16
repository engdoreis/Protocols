/*
 ============================================================================
 Name        : T_NinebotServer.c
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
#include "Template.h"

void TP_Callback(void *param, uint8_t address, uint16_t size, uint8_t *payload)
{
	T_Context * ctx = param;
	ctx->frame = (T_Frame *) payload;
	ctx->payloadSize = size - T_FRAME_HEADER_SIZE;

	if(!ctx->waitingResponse || ctx->frame->type == T_FrameEvent)
	{
		if(ctx->frame->type < T_FrameCount)
		{
			if(ctx->handlerTable[ctx->frame->type].handler)
			{
				ctx->handlerTable[ctx->frame->type].handler(ctx->handlerTable[ctx->frame->type].arg, address, ctx->frame);
			}
		}
	}

	ctx->waitingResponse = false;
}

bool T_Init(T_Obj *obj, const void *port, T_Driver *driver, uint8_t * buffer, uint32_t size)
{
	TP_ASSERT(obj == NULL || port == NULL || buffer == NULL);

	uint32_t offset = 0;
	obj->handle = buffer;
	T_Context * ctx = obj->handle;
	offset += sizeof(T_Context);
	TP_ASSERT(offset >= size);

	ctx->workBufferLen = T_FRAME_HEADER_SIZE + T_MAX_PAYLOAD_LEN;
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

bool T_RegisterEventCallback(T_Obj *obj, T_Callback callback, void *arg)
{
	bool ret = false;
	T_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[T_FrameEvent].handler = callback;
		ctx->handlerTable[T_FrameEvent].arg = arg;
		ret = true;
	}
	return ret;
}

bool T_RegisterCommandCallback(T_Obj *obj, T_Callback callback, void *arg)
{
	bool ret = false;
	T_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[T_FrameCommand].handler = callback;
		ctx->handlerTable[T_FrameCommand].arg = arg;
		ret = true;
	}
	return ret;
}

bool T_RegisterResponseCallback(T_Obj *obj, T_Callback callback, void *arg)
{
	bool ret = false;
	T_Context * ctx = obj->handle;
	if(obj && callback)
	{
		ctx->handlerTable[T_FrameResponse].handler = callback;
		ctx->handlerTable[T_FrameResponse].arg = arg;
		ret = true;
	}
	return ret;
}

bool T_Run (T_Obj *obj)
{
	T_Context * ctx = obj->handle;
	TP_Process(ctx->tp);
	return true;
}

static Template_StatusCode T_SendGeneric(T_Obj *obj, bool sync, uint32_t type, uint32_t id, uint32_t statusCode, void * payload, uint32_t size, void * resp, uint32_t *respSize )
{
	Template_StatusCode result = T_ParameterError;
	T_Context * ctx = obj->handle;

	uint32_t frameSize = size + T_FRAME_HEADER_SIZE;

	if(obj && ctx && frameSize <= ctx->workBufferLen)
	{
		T_Frame * frame = (T_Frame *) ctx->workBuffer;
		T_SET_FRAME_HEADER(frame, type, id, statusCode);
		memcpy(frame->payload.raw, payload, size);
		result = T_ProtocolError;

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
				if(!ctx->waitingResponse && ctx->frame && ctx->frame->type == T_FrameResponse && ctx->frame->id == id)
				{
					result = (Template_StatusCode)ctx->frame->statusCode;

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

Template_StatusCode T_Command1(T_Obj *obj, st_cmd1* data, st_cmd1* out)
{
	uint32_t size = (out==NULL)?0:sizeof(st_cmd1);
	return T_SendGeneric(obj, true, T_FrameCommand, T_Cmd1, T_OK, data, (data==NULL)?0:sizeof(st_cmd1), out, &size);
}

Template_StatusCode T_Command1Async(T_Obj *obj, st_cmd1* data)
{
	return T_SendGeneric(obj, false, T_FrameCommand, T_Cmd1, T_OK, data, (data==NULL)?0:sizeof(st_cmd1), NULL, 0);
}


Template_StatusCode T_Command2(T_Obj *obj, st_cmd2 *cmd)
{
	uint32_t size = sizeof(st_cmd2);
	return T_SendGeneric(obj, true, T_FrameCommand, T_Cmd2, T_OK, NULL, 0, cmd, &size);
}

Template_StatusCode T_Response1(T_Obj *obj, Template_StatusCode statusCode, st_cmd1* data)
{
	return T_SendGeneric(obj, false, T_FrameResponse, T_Cmd1, statusCode, data, (data==NULL)?0:sizeof(st_cmd1), NULL, 0);
}

Template_StatusCode T_Response2(T_Obj *obj, Template_StatusCode statusCode, st_cmd2* data)
{
	return T_SendGeneric(obj, false, T_FrameResponse, T_Cmd2, statusCode, data, (data==NULL)?0:sizeof(st_cmd2), NULL, 0);
}

Template_StatusCode T_Event1(T_Obj *obj, st_cmd1* data)
{
	return T_SendGeneric(obj, false, T_FrameEvent, T_Evt1, T_OK, data, (data==NULL)?0:sizeof(st_cmd1), NULL, 0);
}

Template_StatusCode T_Event2(T_Obj *obj, st_cmd2* data)
{
	return T_SendGeneric(obj, false, T_FrameEvent, T_Evt2, T_OK, data, (data==NULL)?0:sizeof(st_cmd2), NULL, 0);
}


