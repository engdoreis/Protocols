/*==================================================================================================
  INCLUDE FILES
==================================================================================================*/
#include "circular_buffer.h"

#include <stdio.h>
#include <math.h>
/*==================================================================================================
  DEFINES
==================================================================================================*/

/*=================================================================================================
  GLOBAL VARIABLES
=================================================================================================*/

/*=================================================================================================
  STRUCTS
=================================================================================================*/

/*=================================================================================================
  LOCAL VARIABLES
=================================================================================================*/


inline static volatile uint8_t * Increment(volatile uint8_t * value, volatile uint8_t * max, volatile uint8_t * min)
{
	return (uint8_t * )((value < max) ? ++value : min);
}

inline uint32_t CircBufFilledLenGet(circularBuffer_t* ctx)
{
	return ((ctx->head >= ctx->tail) ? (ctx->head - ctx->tail) : ((ctx->arrayEnd - ctx->tail) + (ctx->head - ctx->arrayInit) + 1) );
}

inline static void CriticalCopy(circularBuffer_t* ctx, volatile void *dest, volatile void *src)
{
	if(ctx->irqEnableSet)
		ctx->irqEnableSet(false);

	*((uintptr_t*)dest) = *((uintptr_t *)src);

	if(ctx->irqEnableSet)
		ctx->irqEnableSet(true);
}

inline unsigned int CircBufFreeLenGet(circularBuffer_t* ctx)
{
	return ctx->arraySize - CircBufFilledLenGet(ctx);
}

unsigned int CircBufPut( circularBuffer_t * ctx,unsigned char* addData, unsigned int addSize)
{
	unsigned int freeSize;
	unsigned char* pEndBuf, *localPointer;
	volatile unsigned char *head;

 	if (addData == NULL || ctx->arrayInit == NULL) return 0;

	freeSize = CircBufFreeLenGet(ctx);
	addSize = MIN(freeSize, addSize);

	pEndBuf = addData + addSize;	//calc last address to write in the buffer

	//Douglas
	CriticalCopy(ctx, &head, &ctx->head);

	localPointer = addData;
	while(localPointer < pEndBuf){
		*head = *(localPointer++);
		head = Increment(head, ctx->arrayEnd, ctx->arrayInit);    //increment
	}

	CriticalCopy(ctx, &ctx->head, &head);
	return addSize;
}

unsigned int CircBufGet( circularBuffer_t * ctx,unsigned char* takeData, unsigned int takeSize)
{
	int filled = 0;
	unsigned char* pEndBuf, *localPointer;
	volatile unsigned char *tail;

	if (takeData == NULL || ctx->arrayInit == NULL) return 0;

	filled = CircBufFilledLenGet(ctx);
	if(filled > 0)
	{
		takeSize = MIN(filled, takeSize);
		pEndBuf = takeData + takeSize;	//calc last address to write in the buffer

		CriticalCopy(ctx, &tail, &ctx->tail);

		localPointer = takeData;
		while(localPointer < pEndBuf)
		{
			*(localPointer++) = *tail;
			tail = Increment(tail, ctx->arrayEnd, ctx->arrayInit);    //increment
		}

		CriticalCopy(ctx, &ctx->tail, &tail);
	} else {
		takeSize = 0;
	}
	return takeSize;
}

unsigned int CircBufRead( circularBuffer_t * ctx,unsigned char* readData, unsigned int readSize)
{
	volatile unsigned char* oldTail = ctx->tail;

	int iRet = CircBufGet(ctx, readData, readSize);

	ctx->tail = oldTail;
	CircBufFilledLenGet(ctx);
	return iRet;
}

void CircBufClear( circularBuffer_t * ctx)
{
	ctx->locked = 0;
	ctx->head = ctx->tail = ctx->arrayInit;
}

char CircBufDrop( circularBuffer_t * ctx, unsigned int dataSize)
{
	unsigned int index = 0;
	unsigned int discartSize;
	CircBufFilledLenGet(ctx);
	if (ctx->filled == 0 || dataSize == 0 || ctx->arrayInit == NULL)
	{
		return (char)-1;
	}

	discartSize = ctx->filled < dataSize ? ctx->filled : dataSize; //
	//retira elementos no buffer
	while(index < discartSize)
	{
		Increment(ctx->tail, ctx->arrayEnd, ctx->arrayInit);//apaga dados
	}
	return 0;
}

void CircBufInit(circularBuffer_t * ctx, unsigned char * buffer, unsigned int arraySize, IrqEnableSet irqEnableSet)
{
	ctx->arraySize = arraySize - 1;
	ctx->arrayInit = buffer;
	ctx->arrayEnd = ctx->arrayInit + ctx->arraySize;
	CircBufClear(ctx);
	ctx->irqEnableSet = irqEnableSet;
}
