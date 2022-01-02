/**
 * @file    Helper.c
 * @author  Douglas Reis
 * @date    07/07/2014
 * @version 1.0
 *
 * @section LICENSE
 *
 *
 * @section DESCRIPTION
 *
 */

#include "Helper.h"

#include "../Hash/CRC/CRC16.h"

bool TPSendFrame(TP_Context *context, const Frame *frame)
{
	void *handle = context->control.handle;

	uint16_t _size = TPGetSize(frame->size);

	uint8_t *auxiliar = (uint8_t *)frame;

	if(context->driver.Write(handle, (const void *)auxiliar, TP_STARTING_FRAME_SIZE) == TP_STARTING_FRAME_SIZE)
	{
		if(context->driver.Write(handle, frame->data, _size) == _size)
		{
			if(context->driver.Write(context->control.handle, &frame->crc, TP_CRC_SIZE) == TP_CRC_SIZE)
				return true;
		}
	}

	return false;
}

//static uint16_t TPSum(uint16_t start, const void * data, uint32_t size)
//{
//	for(int i = 0; i < size; i++)
//	{
//		start += ((uint8_t *)data)[i];
//	}
//	return start;
//}

bool TPIsValidCRC(const Frame *frame)
{
	uint16_t _size = TPGetSize(frame->size);

	uint16_t crc = 0;

//	crc = TPSum(crc, frame, TP_STARTING_FRAME_SIZE);
	crc = TP_CRC16((uint8_t *)frame, TP_STARTING_FRAME_SIZE);
//	crc = TPSum(crc, frame->data, _size);
	crc = TP_CRC16Add((uint8_t *)frame->data, _size, crc);

	crc = ~crc;
	uint16_t _crc = GET_LITTLE_ENDIAN_INT16(frame->crc);
	/* Checar os valores de CRC */
	if(crc == _crc)
		return true;

	return false;
}


void TPCalculateCRC(Frame *frame)
{
	uint16_t _size = TPGetSize(frame->size);

	uint32_t crc = 0;

	//	crc = TPSum(crc, frame, TP_STARTING_FRAME_SIZE);
		crc = TP_CRC16((uint8_t *)frame, TP_STARTING_FRAME_SIZE);
	//	crc = TPSum(crc, frame->data, _size);
		crc = TP_CRC16Add((uint8_t *)frame->data, _size, crc);

	SET_LITTLE_ENDIAN_INT16(~crc,frame->crc);
}


void TPParseArrayToFrame(Frame *frame, uint8_t *array)
{
	uint16_t _size = TPGetSize(frame->size);

	memcpy((void *)frame, (void *)array, TP_STARTING_FRAME_SIZE);
	memcpy((void *)frame->data, (void *)&array[TP_STARTING_FRAME_SIZE],
			_size - TP_CRC_SIZE);
	memcpy((void *)&frame->crc, (void *)&array[TP_STARTING_FRAME_SIZE + (_size - TP_CRC_SIZE)], TP_CRC_SIZE);
}


bool TPIsTimeout(TP_Context *context)
{
	uint32_t currentTime = 0;

	currentTime = context->driver.Tick();

	if ((currentTime - context->control.timeout) > context->control.timeoutConfig)
	{
		TPLog(0, "Timeout");

		return true;
	}

	return false;
}
