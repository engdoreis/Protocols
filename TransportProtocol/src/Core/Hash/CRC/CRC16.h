/*
 * CRC16.h
 *
 *  Created on: 19/05/2014
 *      Eng. Douglas Reis
 */

#ifndef CRC16_H_
#define CRC16_H_

#include "../../Settings.h"
/*!
 * @internal  
 * Metodo para calculo de CRC16-Modbus.
 *
 * @param data
 * @param offset
 * @param lenght
 * 
 */
int16_t TP_CRC16(uint8_t *data, int32_t length);

/**
 * @internal 
 */ 
int16_t TP_CRC16Add(uint8_t *data, int32_t length, int16_t crc);


#endif /* CRC16_H_ */
