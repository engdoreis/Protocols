/*
 * Porting.h
 *
 *  Created on: May 20, 2019
 *      Author: Douglas Reis
 */

#ifndef SRC_PORTING_PORTING_H_
#define SRC_PORTING_PORTING_H_

#ifdef __cplusplus
extern "C" {
#endif

#define __USE_MISC

#include <stdint.h>
void * UART_Open(const void *port);
uint16_t UART_Write(void *handle, const void *buffer, uint16_t size);
uint16_t UART_Read(void *handle, void *buffer, uint16_t size);
uint16_t UART_Close(void *handle);
void     UART_Flush(void *handle);
uint32_t SYS_Tick();
void SYS_Sleep(uint32_t time);


#ifdef __cplusplus
}
#endif

#endif /* SRC_PORTING_PORTING_H_ */
