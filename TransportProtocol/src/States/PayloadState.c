/**
 * @file    TPPayloadState.h
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


void TPPayloadState(TP_Context *context)
{
	uint16_t size = TPGetSize(context->response.size);
	uint32_t index = context->control.bytesRead - TP_STARTING_FRAME_SIZE;

	context->control.bytesRead += context->driver.Read(context->control.handle,
			&context->response.data[index], size - index);

	if(context->control.bytesRead == (TP_STARTING_FRAME_SIZE + size))
	{
		context->state = TPChecksumState;
	}
	else if (TPIsTimeout(context) == true)
	{
		TPLog(TPTimeout, "Timeout");

		TPResetContext(context);

		context->control.status = TPTimeout;
		context->state = NULL;
	}
}
