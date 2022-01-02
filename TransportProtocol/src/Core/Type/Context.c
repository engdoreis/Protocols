/**
 * @file    TP_Context.c
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

#include "Context.h"


void TPResetContext(TP_Context *context)
{
	void *handle = context->control.handle;
	uint32_t size = context->control.size;
	uint32_t maxPayload = context->control.maxPayloadSize;
	uint32_t timeout = context->control.timeoutConfig;

	memset((void *)&context->control, '\0', sizeof(TPControl));

	context->control.handle = handle;
	context->control.size = size;
	context->control.maxPayloadSize = maxPayload;
	context->control.timeoutConfig = timeout;
}
