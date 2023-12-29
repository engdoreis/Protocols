#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_
/*==================================================================================================
  INCLUDE FILES
==================================================================================================*/
#include <stdint.h>
/*==================================================================================================
  DEFINES
==================================================================================================*/

/*================================================================================================== 
  MACROS 
==================================================================================================*/
#define MIN(X,Y) X<Y?X:Y
#define MAX(X,Y) X>Y?X:Y
/*=================================================================================================
  GLOBAL VARIABLES
=================================================================================================*/

/*=================================================================================================
  STRUCTS
=================================================================================================*/
#include <stdbool.h>

typedef void (*IrqEnableSet)(bool enable);
typedef struct circularBuffer
{
	volatile unsigned char *tail;
	volatile unsigned char *head;
	volatile unsigned int filled;
	volatile unsigned char locked;
	unsigned int arraySize;
	unsigned char *arrayInit;
	unsigned char *arrayEnd;
	IrqEnableSet irqEnableSet;
} circularBuffer_t;
/*=================================================================================================
  LOCAL VARIABLES
=================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
  Function    : CircBufPutGet

  Description : tira dados no buffer circular

  Parameters  : dado [IN] ponteiro de bytes contedo os dados
                tamanho [IN],[OUT] tamanho do ponteiro de entrada, retorna com quantidade de dados lidos

  Returns     : ok se sucesso e buffer_vazio se nao ouver dados no buffer
==================================================================================================*/
unsigned int CircBufPut( circularBuffer_t * ctx,unsigned char* addData, unsigned int addSize);


unsigned int CircBufGet( circularBuffer_t * ctx,unsigned char* takeData, unsigned int takeSize);
/*==================================================================================================
  Function    : CircBufRead

  Description : tira dados no buffer circular

  Parameters  : dado [IN] ponteiro de bytes contedo os dados
                tamanho [IN],[OUT] tamanho do ponteiro de entrada, retorna com quantidade de dados lidos

  Returns     : ok se sucesso e buffer_vazio se nÃ£o ouver dados no buffer
==================================================================================================*/
//char CircBufRead( circularBuffer_t * ctx,unsigned char* getData, unsigned int *getSize);

/*==================================================================================================
  Function    : CircBufClear

  Description : Limpa buffer

  Parameters  : void

  Returns     :
==================================================================================================*/
void CircBufClear( circularBuffer_t * ctx);

/*==================================================================================================
  Function    : CircBufDrop

  Description : Discard older data from the buffer

  Parameters  : dataSize [IN] Amount of data to be discarded

==================================================================================================*/
char CircBufDrop( circularBuffer_t * ctx, unsigned int dataSize);

/*==================================================================================================
  Function    : CircBufInit

  Description : Initialize the circular buffer

  Parameters  : circBuf  [IN]   buffer context

==================================================================================================*/
void CircBufInit(circularBuffer_t * ctx, unsigned char * buffer, unsigned int arraySize, IrqEnableSet irqEnableSet);


uint32_t CircBufFilledLenGet(circularBuffer_t* ctx);

unsigned int CircBufFreeLenGet(circularBuffer_t* ctx);


#ifdef __cplusplus
}
#endif

#endif	//CIRCULAR_BUFFER_H_
