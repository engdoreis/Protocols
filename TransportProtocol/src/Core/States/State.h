/**
 * @file    TPState.h
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

#ifndef TPSTATE_H_
#define TPSTATE_H_

#include "../Type/Context.h"


/* Prototipos de funcoes implementados pelo servidor e cliente */

/*!
 * Estado de leitura dos bytes iniciais.
 * @param context Contexto atual da comunicacao do protocolo TP.
 */
void TPServerStartingFrameState(TP_Context *context);

void TPStartingFrameState(TP_Context *context);

/*!
 * Estado de leitura dos bytes finais de acordo com o size do comando/resposta.
 * @param context Contexto atual da comunicacao do protocolo TP.
 */
void TPServerEndingFrameState(TP_Context *context);

/*!
 * Receives Frame payload.
 *
 * @param context Current context of the TP protocol communication.
 */
void TPServerPayloadState(TP_Context *context);


void TPPayloadState(TP_Context *context);

/*!
 * Estado de espera de retorno de sincronismo ap�s envio do comando/resposta.
 * @param context Contexto atual da comunicacao do protocolo TP.
 */
void TPServerAwaitingReturnState(TP_Context *context);
void TPIdleState(TP_Context *context);

/*!
 * Estado de envio de NAK ap�s a verifica��o da integridade do comando/resposta.
 * @param context Contexto atual da comunicacao do protocolo TP.
 */
void TPServerNAKRequestState(TP_Context *context);
void TPNAKRequestState(TP_Context *context);

/*!
 * Estado de tratamento ap�s receber NAK ap�s envio do comando/resposta.
 * @param context Contexto atual da comunicacao do protocolo TP.
 */
void TPServerNAKResponseState(TP_Context *context);
void TPNAKResponseState(TP_Context *context);

/*!
 * Receives Frame Chechsum.
 *
 * @param context Current context of the TP protocol communication.
 */
void TPServerChecksumState(TP_Context *context);


void TPChecksumState(TP_Context *context);


#endif /* TPSTATE_H_ */
