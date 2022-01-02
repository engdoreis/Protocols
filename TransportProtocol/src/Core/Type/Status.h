/**
 * @file    TPStatus.h
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

#ifndef TPStatus_H_
#define TPStatus_H_


typedef enum
{
	TPNone = -1,
	TPFailure,
	TPSuccess,
	TPInactivity,
	TPIsRunning,
	TPCommandIDUnknown,
	TPCommandNotImplemented,
	TPErrorChecksum,
	TPErrorSending,
	TPTimeout,
	TPFrameError,
	TPBufferOverflow
} TPStatus;


#endif /* TPStatus_H_ */
