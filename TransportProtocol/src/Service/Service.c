/**
 * @file   TPService.c
 * @Author Douglas Reis
 * @date   02/10/2014
 */

#include "Service.h"


bool TP_Init(TP_Obj *obj, TP_Driver *driver, ITPCallback callback, void *param, const void * port, uint32_t timeout, uint8_t *buffer, uint32_t size)
{
	TP_ASSERT(obj == NULL || driver == NULL || driver->Open == NULL || driver->Read == NULL || driver->Write == NULL || driver->Close == NULL || buffer == NULL);

	uint32_t offset = 0;
	obj->handle = buffer;
	TP_Context *context = obj->handle;

	offset += sizeof(TP_Context);
	TP_ASSERT(offset >= size);

	memcpy(&context->driver, driver, sizeof(TP_Driver));

	context->port = port;
	context->control.timeoutConfig = timeout;

	context->control.maxPayloadSize = (size - offset);
	context->response.data = buffer + offset;

	offset += context->control.maxPayloadSize;
	TP_ASSERT(offset > size);

	context->callback = callback;
	context->param = param;

	context->control.handle = context->driver.Open(context->port);
	if(context->control.handle != NULL)
	{
		return true;
	}

	end:
	return false;
}


bool TP_Send(TP_Obj *obj, uint8_t address, const uint8_t *payload, uint32_t size)
{
	TP_Context *context = obj->handle;
	TPResetContext(context);

	context->driver.Flush(context->control.handle);

	context->command.data = (uint8_t*)payload;

	SET_STX(context->command.stx);
	TP_Int16ToArray(size, context->command.size);
	context->command.address = address;

	/*Calcula CRC16*/
	TPCalculateCRC(&context->command);

	/* Envia comando */
	return TPSendFrame(context, &context->command);
}

void TP_Process(TP_Obj *obj)
{
	TP_Context *context = obj->handle;
	TPResetContext(context);
	memset(&context->response, 0, TP_STARTING_FRAME_SIZE);

	context->control.timeout = context->driver.Tick();
	context->control.bytesRead = 0;

	context->state = TPIdleState;

	do
	{
		(*context->state)(context);
	} while(context->state != NULL);

	if(context->control.status == TPSuccess && context->callback)
	{
		context->callback(context->param, context->response.address, TPGetSize(context->response.size), context->response.data);
	}
}

