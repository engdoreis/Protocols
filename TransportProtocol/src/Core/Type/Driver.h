/**
 * @file    TP_Driver.h
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

#ifndef TP_Driver_H_
#define TP_Driver_H_

#include <stdint.h>

#include "../Settings.h"

/*!
 * @brief Callback struct to provide access to platform resources.
 */
typedef struct
{
	/*!
	 * @brief Open the port and return a handler to used by the functions T_Driver::Write T_Driver::Read T_Driver::Close
	 *
	 * @param[in] portName Name or identification to port to be opened.
	 *
	 * @return Handler to used to used by the functions T_Driver::Write T_Driver::Read T_Driver::Close
	 */
	void *   ( *Open )(const void *portName);

	/*!
	 * @brief Write raw data to port opened by T_Driver::Open
	 *
	 * @param[in] handle Returned by T_Driver::Open.
	 * @param[in] buffer Buffer with the data to be send.
	 * @param[in] size   Amount of data so be send.
	 *
	 * @return Amount of data written.
	 */
	uint16_t ( *Write)(void *handle, const void *buffer, uint16_t size);

	/*!
	 * @brief Read raw data from port opened by T_Driver::Open
	 *
	 * @param[in] handle Returned by T_Driver::Open.
	 * @param[in] buffer Memory buffer to receive the data read.
	 * @param[in] size   Total space available in the buffer.
	 *
	 * @return Amount of data read.
	 */
	uint16_t ( *Read )(void *handle, void *buffer, uint16_t size);

	/*!
	 * @brief Close the port opened by T_Driver::Open
	 *
	 * @param[in] handle Returned by T_Driver::Open.
	 *
	 * @return 0 if ok.
	 */
	uint16_t ( *Close)(void *handle);

	/*!
	 * @brief Flush the port opened by T_Driver::Open
	 *
	 * @param[in] handle Returned by T_Driver::Open.
	 *
	 */
	void     ( *Flush)(void *handle);

	/*!
	 * @brief Read the system tick.
	 *
	 * @return System tick.
	 */
	uint32_t ( *Tick )();

	/*!
	 * @brief Sleep during the time specified.
	 *
	 * @param[in] time to sleep
	 */
	void (*Sleep) (uint32_t time);
}TP_Driver;

#endif /* TP_Driver_H_ */
