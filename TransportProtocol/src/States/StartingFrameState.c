/**
 * @file    TPStartingFrameState.h
 * @author  Douglas Reis
 * @date    02/10/2014
 * @version 1.0
 *
 * @section LICENSE
 *
 *
 * @section DESCRIPTION
 *
 * 
 */

#include "../../Core/Helper/Helper.h"
#include "../../Core/States/State.h"


void TPStartingFrameState(TP_Context *context)
{
	uint8_t *buffer = (uint8_t *)&context->response;

	context->control.bytesRead += context->driver.Read(context->control.handle,
			(void *)&buffer[context->control.bytesRead], TP_STARTING_FRAME_SIZE - context->control.bytesRead);

	if(context->control.bytesRead >= TP_STARTING_FRAME_SIZE)
	{
		if(TPGetSize(context->response.size) <= context->control.maxPayloadSize)
		{
			context->state = TPPayloadState;
		}
		else
		{
			TPResetContext(context);

			context->control.status = TPBufferOverflow;
			context->state = NULL;
		}
	}

	else if (TPIsTimeout(context) == true)
	{
		TPLog(TPTimeout, "Timeout");

		TPResetContext(context);

		context->control.status = TPTimeout;
		context->state = NULL;
	}

}
