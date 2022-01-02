/**
 * @file    TPSettings.h
 * @author  Douglas Reis
 * @version 1.0
 * @date    04/06/2014
 * @section LICENSE
 *
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef TPSETTINGS_H_
#define TPSETTINGS_H_


#include <stdint.h>
#include <stdbool.h>
//#define TP_DEBUG

#if defined _8051_
	#define EDE_CALLBACK __near_func
#else
	#define EDE_CALLBACK
#endif

#define __SHORT_ENUM__ __attribute__ ((__packed__))

#define TP_COMMAND_AMOUNT		40
#define TP_COMMAND_UNSUPPORTED	0

#define TP_TIMEOUT              2500
#define TP_WAIT_TIMEOUT	        50
#define TP_READ_COMMAND	        0
#define TP_WRITE_COMMAND		1

#define TP_TRIALS_AMOUNT	  	3
#define TP_BUFFER_SIZE		  	(uint32_t)(1 * 1024)
#define TP_PARAMETER_SIZE		2

#define SET_BIG_ENDIAN_INT16(_x, array) array[0] = (_x >> 8) & 0xFF; array[1] = (_x >> 0) & 0xFF
#define SET_LITTLE_ENDIAN_INT16(_x, array) array[1] = (_x >> 8) & 0xFF; array[0] = (_x >> 0) & 0xFF

#define GET_BIG_ENDIAN_INT16(array) ((array[0] & 0xFF) << 8) | (array[1] & 0xFF)
#define GET_LITTLE_ENDIAN_INT16(array) ((array[1]  & 0xFF) << 8) | (array[0] & 0xFF)


#define SET_STX(array) memcpy(array, TP_STX, TP_STX_SIZE)


#if defined TP_DEBUG
	#define TPLog(TPCode, TPMessage) printf("Codigo %d: %s: %d, %s.\n", TPCode, __FILE__, __LINE__, TPMessage)
#else
	#define TPLog(TPCode, TPMessage) (void) 0;
#endif


#endif /* TPSETTINGS_H_ */
