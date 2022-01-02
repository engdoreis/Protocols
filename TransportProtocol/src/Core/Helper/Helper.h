/**
 * @file    TPHelper.h
 * @author  Douglas Reis
 * @date    07/07/2014
 * @version 1.0
 *
 * @section LICENSE
 *
 *
 * @section DESCRIPTION
 *
 *
 */

#ifndef TPHELPER_H_
#define TPHELPER_H_

#include <string.h>

#include "../Type/Context.h"
#include "../Settings.h"

#define TP_ArrayToInt32(TPArray)			(uint32_t)((TPArray[0] << 24) | (TPArray[1] << 16) | (TPArray[2] << 8) | (TPArray[3] & 0xFF))
#define TP_Int32ToArray(Int32, TPArray)	    TPArray[0] = (Int32 >> 24) & 0xFF; TPArray[1] = (Int32 >> 16) & 0xFF;TPArray[2] = (Int32 >> 8) & 0xFF;TPArray[3] = (Int32 >> 0) & 0xFF;
#define TP_ArrayToInt16(TPArray)			(uint16_t) ((TPArray[0] << 8) | (TPArray[1] & 0xFF))
#define TP_Int16ToArray(Int16, TPArray)	    TPArray[0] = (Int16 >> 8) & 0xFF;TPArray[1] = (Int16 >> 0) & 0xFF;


#define TP_CRC_ArrayToInt(TPArray)			(uint16_t)((TPArray[0] << 8) | (TPArray[1] & 0xFF))
#define TP_CRC_IntToArray(TPValue, TPArray)	(TPArray[0] = TPValue >> 8; TPArray[1] = TPValue & 0xFF)
#define TPGetSize(TPSize)	                (uint16_t)((TP_ArrayToInt16(TPSize)))

#define TP_ASSERT(test)	                    if(test) goto end


/*!
 * @internal
 * @private
 * Metodo auxiliar para verificar se e comando de escrita.
 *
 * @param value Valor recebido do codigo do comando.
 * @return Retorna true se for comando de escrita ou false se for comando de leitura.
 */
bool TPIsWriteCommand(uint8_t value);

/*!
 * @internal
 * Metodo auxiliar para envio do comando solicitado.
 *
 * @param frame Referencia da estrutura do comando.
 * @return Retorna true em caso de sucesso ou false caso contr�rio.
 */
bool TPSendFrame(TP_Context *context, const Frame *frame);

/*!
 * @internal
 * Funcao para verificar se o comando/resposta esta integro.
 *
 * @param Referencia do comando/resposta para verificacao.
 * @return Retorna true se for valido ou false caso contrario.
 */
bool TPIsValidCRC(const Frame *frame);

/*!
 * @internal
 *
 * @param frame
 * @param array
 */
void TPParseArrayToFrame(Frame *frame, uint8_t *array);

/*!
 * @internal
 * Funcao para calculo e concatenacao do valor de CRC16 ao frame do comando/resposta.
 *
 * @param Referencia do comando/resposta para o calculo do CRC16.
 */
void TPCalculateCRC(Frame *frame);

/*!
 * @internal
 *
 * @param context
 * @return
 */
bool TPIsTimeout(TP_Context *context);


#endif /* TPHELPER_H_ */
