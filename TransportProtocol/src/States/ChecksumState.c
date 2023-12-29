/*
 * TPChecksumState.c
 *
 *  Created on: 01/02/2017
 *      Eng. Douglas Reis
 */

#include "../../Core/Helper/Helper.h"
#include "../../Core/States/State.h"


void TPChecksumState(TP_Context *context)
{
	uint16_t size = TPGetSize(context->response.size);
	uint32_t index = context->control.bytesRead - TP_STARTING_FRAME_SIZE - size;

	context->control.bytesRead += context->driver.Read(context->control.handle,
			&context->response.crc[index], TP_CRC_SIZE - index);

	index = context->control.bytesRead - TP_STARTING_FRAME_SIZE - size;
	if(index == TP_CRC_SIZE)
	{
		context->control.status = TPErrorChecksum;
		if(TPIsValidCRC(&context->response) == true)
		{
			context->control.status = TPSuccess;
		}
		context->state = NULL;
	}
	else if (TPIsTimeout(context) == true)
	{
		TPResetContext(context);

		context->control.status = TPTimeout;
		context->state = NULL;
	}

}
