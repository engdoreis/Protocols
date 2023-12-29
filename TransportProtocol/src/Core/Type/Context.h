/**
 * @file    TP_Context.h
 * @author  Douglas Reis
 * @date    05/06/2014
 * @version 1.0
 *
 * @section LICENSE
 *
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef TPCONTEXT_H_
#define TPCONTEXT_H_

#include <string.h>

#include "../States/IState.h"
#include "Driver.h"
#include "Frame.h"
#include "Status.h"

typedef void (*ITPCallback)(void *param, uint8_t address, uint16_t size, uint8_t *payload);

typedef struct TPControl
{
	void *handle;
	uint32_t timeoutConfig;
	uint32_t timeout;
	uint32_t bytesRead;
	uint8_t trialsAmount;
	uint32_t size;	             			/*!< Maximum payload size */
	uint32_t maxPayloadSize;				/*!< Maximum payload size */
	TPStatus status;
} TPControl;


struct TP_Context
{
	const void *port;
	TP_Driver driver;
	Frame command;
	Frame response;
	TPControl control;
	ITPCallback callback;
	void *param;
	ITPState state;
};

/*!
 * Metodo para reinicializar os atributos de controle da maquina de estado do protocolo.
 *
 * @param context Referencia do contexto da comunicacao para reinicializacao.
 *
 * @internal  
 */
void TPResetContext(TP_Context *context);


#endif /* TPCONTEXT_H_ */
