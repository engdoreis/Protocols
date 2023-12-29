/**
 * @file    TPAwaitingReturn.h
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
 
#include "../../Core/States/State.h"
#include "../../Core/Helper/Helper.h"

void TPIdleState(TP_Context *context)
{
	uint8_t *buffer = (uint8_t *)&context->response;

	context->control.bytesRead += context->driver.Read(context->control.handle,
				(void *)&buffer[context->control.bytesRead], TP_STX_SIZE - context->control.bytesRead);

	if(context->control.bytesRead >= TP_STX_SIZE)
	{
		if(memcmp(buffer, TP_STX, TP_STX_SIZE) == 0)
		{
			context->state = TPStartingFrameState;
		}
		else
		{
			if(buffer[1] == TP_STX[0])
			{
				buffer[0] = buffer[1];
				context->control.bytesRead = 1;
			}
			else
			{
				context->control.bytesRead = 0;
			}
		}
	}

	if (context->state == TPIdleState && TPIsTimeout(context) == true)
	{
		TPLog(TPTimeout, "Timeout");

		TPResetContext(context);

		context->control.status = TPTimeout;
		context->state = NULL;
	}
}
