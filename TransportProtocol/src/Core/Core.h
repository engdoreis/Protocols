/**
 * @file    TPCore.h
 * @brief   
 * @author  Douglas Reis
 * @date    26/06/2014
 * @version 1.0
 *
 * @section LICENSE
 *
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef TPCORE_H_
#define TPCORE_H_

#include <stdio.h>

#include "Settings.h"

typedef void (EDE_CALLBACK *TPGetMethod)(void *buffer, uint16_t parameter, uint8_t *size);
typedef void (EDE_CALLBACK *TPSetMethod)(const void *data, uint16_t parameter, uint8_t size);

#include "Helper/Helper.h"
#include "States/State.h"


/*!
 * @internal
 * Prototype to calculate hash CRC16 Modbus.
 *
 * @param data Data buffer used to calculate hash value.
 * @param size Data buffer size.
 *
 * @return Returns CRC16 Modbus value.
 */
//uint16_t TPGetCRC16(uint8_t *data, uint32_t size);

/*!
 * @internal
 * Stub used to get timestamp from device.
 *
 * @param value Timestamp value.
 */
void TPGetTick(uint32_t *value);

#define TPSetTimeout(context, timeout) ((TP_Context *)context)->control.timeoutConfig = timeout



#endif /* TPCORE_H_ */
